/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:33:38 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/01 01:49:34 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ResponseTools.hpp"

std::string ResponseTools::assemblePath(std::string root, std::string requestURI)
{
    std::string path = ".";
    std::string delimiter = "/";

    ftstring::trim(root, "./\t\v\f\r\n");
    ftstring::trim(requestURI, "/\t\v\f\r\n");

    if (root != "" && requestURI != "") {
        path += delimiter + root + delimiter + requestURI;
    } else if (root != "") {
        path += delimiter + root;
    } else if (requestURI != "") {
        path += delimiter + requestURI;
    }

    if (path[path.size() - 1] != '/') {
        path += delimiter;
    }

    return path;
}

bool ResponseTools::fileExists(std::string path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

std::string ResponseTools::getFileExtension(std::string path)
{
    size_t period = path.rfind(".");
    if (period == std::string::npos) {
        return "";
    } else {
        return path.substr(period + 1);
    }
}

