/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/14 15:55:37 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSETOOLS_HPP
#define RESPONSETOOLS_HPP

#include "libs.hpp"

class ResponseTools {
    public:
        static std::string  assemblePath(std::string root, std::string requestURI);
        static bool         fileExists(std::string path);
        static std::string  getFileExtension(std::string path);
        static bool         endsWith(const std::string& str, const std::string& suffix);
        static bool         startsWith(const std::string& str, const std::string& prefix);
        static std::string  removeOverlap(const std::string& requestURI, const std::string& root);
        static bool         isDirectory(std::string path);
        static bool         isRequestMethodAllowed(std::string method, std::vector<std::string> limitExcept);
        static size_t       convertMaxBodySizeToNumber(std::string maxSize);
        static std::string  autoindex(std::string path, std::string port);
        static std::string  getCurrentDate(void);
        static void         initStatusCodes(std::map<std::string, std::string> &statusCodes);
        static void         initContentTypes(std::map<std::string, std::string> &contentTypes);
};

#endif