/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/01 17:20:06 by lcouto           ###   ########.fr       */
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
        static void         initStatusCodes(std::map<std::string, std::string> &statusCodes);
        static void         initContentTypes(std::map<std::string, std::string> &contentTypes);
};

#endif