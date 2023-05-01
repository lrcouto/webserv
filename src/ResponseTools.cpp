/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:33:38 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/01 17:22:06 by lcouto           ###   ########.fr       */
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

void ResponseTools::initStatusCodes(std::map<std::string, std::string> &statusCodes)
{
    statusCodes["100"] = "Continue";
    statusCodes["101"] = "Switching Protocols";
    statusCodes["102"] = "Processing";
    statusCodes["200"] = "OK";
    statusCodes["201"] = "Created";
    statusCodes["202"] = "Accepted";
    statusCodes["203"] = "Non-Authoritative Information";
    statusCodes["204"] = "No Content";
    statusCodes["205"] = "Reset Content";
    statusCodes["206"] = "Partial Content";
    statusCodes["207"] = "Multi-Status";
    statusCodes["208"] = "Already Reported";
    statusCodes["226"] = "IM Used";
    statusCodes["300"] = "Multiple Choices";
    statusCodes["301"] = "Moved Permanently";
    statusCodes["302"] = "Found";
    statusCodes["303"] = "See Other";
    statusCodes["304"] = "Not Modified";
    statusCodes["305"] = "Use Proxy";
    statusCodes["307"] = "Temporary Redirect";
    statusCodes["308"] = "Permanent Redirect";
    statusCodes["400"] = "Bad Request";
    statusCodes["401"] = "Unauthorized";
    statusCodes["402"] = "Payment Required";
    statusCodes["403"] = "Forbidden";
    statusCodes["404"] = "Not Found";
    statusCodes["405"] = "Method Not Allowed";
    statusCodes["406"] = "Not Acceptable";
    statusCodes["407"] = "Proxy Authentication Required";
    statusCodes["408"] = "Request Timeout";
    statusCodes["409"] = "Conflict";
    statusCodes["410"] = "Gone";
    statusCodes["411"] = "Length Required";
    statusCodes["412"] = "Precondition Failed";
    statusCodes["413"] = "Payload Too Large";
    statusCodes["414"] = "URI Too Long";
    statusCodes["415"] = "Unsupported Media Type";
    statusCodes["416"] = "Range Not Satisfiable";
    statusCodes["417"] = "Expectation Failed";
    statusCodes["418"] = "I'm a teapot";
    statusCodes["421"] = "Misdirected Request";
    statusCodes["422"] = "Unprocessable Entity";
    statusCodes["423"] = "Locked";
    statusCodes["424"] = "Failed Dependency";
    statusCodes["425"] = "Too Early";
    statusCodes["426"] = "Upgrade Required";
    statusCodes["428"] = "Precondition Required";
    statusCodes["429"] = "Too Many Requests";
    statusCodes["431"] = "Request Header Fields Too Large";
    statusCodes["451"] = "Unavailable For Legal Reasons";
    statusCodes["500"] = "Internal Server Error";
    statusCodes["501"] = "Not Implemented";
    statusCodes["502"] = "Bad Gateway";
    statusCodes["503"] = "Service Unavailable";
    statusCodes["504"] = "Gateway Timeout";
    statusCodes["505"] = "HTTP Version Not Supported";
    statusCodes["506"] = "Variant Also Negotiates";
    statusCodes["507"] = "Insufficient Storage";
    statusCodes["508"] = "Loop Detected";
    statusCodes["510"] = "Not Extended";
    statusCodes["511"] = "Network Authentication Required";
}

void ResponseTools::initContentTypes(std::map<std::string, std::string> &contentTypes)
{
    contentTypes["txt"] = "text/plain";
    contentTypes["html"] = "text/html";
    contentTypes["css"] = "text/css";
    contentTypes["js"] = "text/javascript";
    contentTypes["json"] = "application/json";
    contentTypes["xml"] = "application/xml";
    contentTypes["pdf"] = "application/pdf";
    contentTypes["zip"] = "application/zip";
    contentTypes["gzip"] = "application/gzip";
    contentTypes["tar"] = "application/x-tar";
    contentTypes["png"] = "image/png";
    contentTypes["jpg"] = "image/jpeg";
    contentTypes["jpeg"] = "image/jpeg";
    contentTypes["gif"] = "image/gif";
    contentTypes["svg"] = "image/svg+xml";
    contentTypes["ico"] = "image/x-icon";
}
