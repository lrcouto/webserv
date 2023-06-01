/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/30 16:30:14 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSETOOLS_HPP
#define RESPONSETOOLS_HPP

#include "libs.hpp"

class ResponseTools {
    public:
        static std::string assemblePath(std::string root, std::string requestURI);
        static std::string removeOverlap(std::string const &requestURI, std::string const &root);
        static std::string autoindex(std::string path, std::string port);
        static size_t      convertMaxBodySizeToNumber(std::string maxSize);
        static bool        fileExists(std::string path);
        static bool        endsWith(std::string const &str, std::string const &suffix);
        static bool        startsWith(std::string const &str, std::string const &prefix);
        static bool        isDirectory(std::string path);
        static bool        isRequestMethodAllowed(std::string              method,
                                                  std::vector<std::string> limitExcept);

        static std::string getFileExtension(std::string path);
        static std::string getCurrentDate(void);

        static std::map<std::string, std::string> initStatusCodes(void);
        static std::map<std::string, std::string> initContentTypes(void);
};

#endif
