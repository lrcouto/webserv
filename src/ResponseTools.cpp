/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:33:38 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/02 20:43:35 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseTools.hpp"

std::string ResponseTools::assemblePath(std::string root, std::string requestURI)
{
    std::string path      = ".";
    std::string delimiter = "/";

    ftstring::trim(root, "./\t\v\f\r\n");
    ftstring::trim(requestURI, "/\t\v\f\r\n");

    if (!root.empty() && !requestURI.empty()) {
        path += delimiter + root + requestURI;
    } else if (!root.empty()) {
        path += delimiter + root;
    } else if (!requestURI.empty()) {
        path += delimiter + requestURI;
    }
    return path;
}

bool ResponseTools::fileExists(std::string path)
{
    struct stat buffer;

    return ((stat(path.c_str(), &buffer) == 0) && (!S_ISDIR(buffer.st_mode)));
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
