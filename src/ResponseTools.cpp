/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseTools.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:33:38 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/09 00:48:43 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseTools.hpp"

std::string ResponseTools::assemblePath(std::string first, std::string second)
{
    std::string path      = ".";
    std::string delimiter = "/";

    first = ftstring::reduce(first, "./\t\v\f\r\n", "/");
    second = ftstring::reduce(second, "/\t\v\f\r\n", "/");

    second = removeOverlap(first, second);

    if (first != "" && second != "") {
        path += delimiter + first + delimiter + second;
    } else if (first != "") {
        path += delimiter + first;
    } else if (second != "") {
        path += delimiter + second;
    }

    std::string extension = getFileExtension(path.substr(1));
    if (path[path.size() - 1] != '/' && extension.empty()) {
        path += delimiter;
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

bool ResponseTools::endsWith(const std::string& str, const std::string& suffix) {
    std::string reducedStr = ftstring::reduce(str, "/");
    std::string reducedSuffix = ftstring::reduce(suffix, "/");
    if (reducedSuffix.size() > reducedStr.size()) {
        return false;
    }
    return reducedStr.substr(reducedStr.size() - reducedSuffix.size()) == reducedSuffix;
}

bool ResponseTools::startsWith(const std::string& str, const std::string& prefix) {
    if (prefix.size() > str.size()) {
        return false;
    }
    return str.substr(0, prefix.size()) == prefix;
}

std::string ResponseTools::removeOverlap(const std::string& first, const std::string& second) {
    size_t overlap = second.find(first);
    if (overlap != std::string::npos) {
        std::string newPath = second;
        newPath.erase(overlap, first.size());
        newPath = ftstring::reduce(newPath, "/\t\v\f\r\n", "/");
        return newPath;
    }
    return second;
}

bool ResponseTools::isDirectory(std::string path)
{
    struct stat buffer;
    if (stat(path.c_str(), &buffer) == 0) {
        return S_ISDIR(buffer.st_mode);
    }
    return false;
}

size_t ResponseTools::convertMaxBodySizeToNumber(std::string maxSize)
{
    std::string::const_iterator it = maxSize.begin();
    size_t value = 0;
    while (it != maxSize.end() && std::isdigit(*it)) {
        value = value * 10 + (*it - '0');
        ++it;
    }
    if (it == maxSize.end()) {
        return value;
    }
    if (*it == 'm' || *it == 'M') {
        return value * 1024 * 1024;
    }
    if (*it == 'g' || *it == 'G') {
        return value * 1024 * 1024 * 1024;
    }
    return 0;
}

bool ResponseTools::isRequestMethodAllowed(std::string method, std::vector<std::string> limitExcept)
{
    std::vector<std::string>::const_iterator it = std::find(limitExcept.begin(), limitExcept.end(), method);
    return it != limitExcept.end();
}

std::string ResponseTools::autoindex(std::string path, std::string port)
{
    DIR *dir = opendir(path.c_str());
    if (!dir)
        return "";

    std::string indexPage = "<!DOCTYPE html>\n\
                            <html>\n\
                            <head>\n\
                            <title>" + path + "</title>\n\
                            </head>\n\
                            <body>\n\
                            <h1>INDEX</h1>\n\
                            <p>\n";

    for (struct dirent *dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir)) {
        std::stringstream ss;
        ss << "\t\t<p><a href=\"http://localhost" << ":" << port
        << path.substr(1) + (path[path.size() - 1] != '/' ? "/" : "") + dirEntry->d_name + "\">" + dirEntry->d_name + "</a></p>\n";
        indexPage += ss.str();
    }
    indexPage += "</p>\n</body>\n</html>\n";
    closedir(dir);
    return indexPage;
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
