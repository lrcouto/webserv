/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:53 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/14 17:32:49 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void)
{
    ResponseTools::initStatusCodes(this->_statusCodes);
    ResponseTools::initContentTypes(this->_contentTypes);
    return ;
}

Response::Response(Request request) : _request(request)
{
    ResponseTools::initStatusCodes(this->_statusCodes);
    ResponseTools::initContentTypes(this->_contentTypes);
    return ;
}

Response::~Response(void) { clear(); }

Response::Response(Response const &other) { *this = other; }

Response &Response::operator=(Response const &other)
{
    if (this != &other) {
        this->_request        = other._request;
        this->_responseString = other._responseString;
        this->_statusLine     = other._statusLine;
        this->_headers        = other._headers;
        this->_body           = other._body;
        this->_status         = other._status;
        this->_type           = other._type;
        this->_serverData     = other._serverData;
    }
    return *this;
}

std::string Response::getResponseString(void) { return this->_responseString; }

void Response::setServerData(Server *serverData) { this->_serverData = serverData; }

void Response::setRequest(Request request) { this->_request = request; }

void Response::assembleResponseString(void)
{
    if (!this->_serverData->getValue("server_name").empty())
        validateServerName();

    std::string responseString, headersString;

    if (this->_status.empty())
        assembleBody();
    assembleHeaders();
    assembleStatusLine();

    std::map<std::string, std::string>::iterator it;
    for (it = this->_headers.begin(); it != this->_headers.end(); ++it) {
        headersString = headersString + it->first + ": " + it->second + CRLF;
    }

    responseString = this->_statusLine + CRLF + headersString + CRLF + this->_body;

    this->_responseString = responseString;
}

void Response::assembleStatusLine()
{
    std::string statusCode, statusMessage, protocol;
    std::map<std::string, std::string>::iterator it = this->_statusCodes.find(this->_status);
    protocol = "HTTP/1.1";
    statusCode = it->first;
    statusMessage = it->second;

    this->_statusLine = protocol + SP + statusCode + SP + statusMessage;
}

void Response::assembleHeaders()
{
    if (this->_body.length() > 0) {
        std::stringstream ss;
        ss << this->_body.length();
        this->_headers.insert(std::make_pair("Content-Length", ss.str()));
    } else {
        this->_headers.insert(std::make_pair("Content-Length", "0"));
    }

    std::string contentType;
    std::map<std::string, std::string>::iterator it = this->_contentTypes.find(this->_type);
    if (it->second == "") {
        contentType = "application/octet-stream";
    } else {
        contentType = it->second;
    }
    this->_headers.insert(std::make_pair("Content-Type", contentType));

    this->_headers.insert(std::make_pair("Date", ResponseTools::getCurrentDate()));
    this->_headers.insert(std::make_pair("Server", "Webserv-42SP"));
    this->_headers.insert(std::make_pair("Set-Cookie", "session_id= "+ this->_serverData->getSessionId() + "; path=/"));
}

void Response::assembleBody()
{
    std::vector<Location> locations = this->_serverData->getLocations();
    std::string requestURI = this->_request.getRequestURI();
    std::vector<std::string> limitExcept;

    if (!locations.empty()) {
        for (size_t i = 0; i < locations.size(); i++) {

            std::string locationPath = locations[i].getPath();
            limitExcept = locations[i].getValue("limit_except");
            if (!limitExcept.empty() && requestURI.find(locationPath) != std::string::npos)
                if (!ResponseTools::isRequestMethodAllowed(this->_request.getMethod(), limitExcept)) {
                    HTTPError("405");
                    return ;
                }
        }
    }

    if (this->_request.getMethod() == "GET") {
        getResource(requestURI);
    } else if (this->_request.getMethod() == "POST") {
        postResource(requestURI);
    } else if (this->_request.getMethod() == "DELETE") {
        deleteResource(requestURI);
    } else {
        HTTPError("501");
    }
}

void Response::getResource(std::string requestURI)
{
    std::string resourcePath, root, resource;
    std::vector<std::string> indexes, autoindex, redirect;
    bool redirected = false;

    std::vector<Location> locations = this->_serverData->getLocations();
    if (!locations.empty()) {
        for (size_t i = 0; i < locations.size(); i++) {

            std::string locationPath = locations[i].getPath();

            indexes = locations[i].getValue("index");
            root = locations[i].getValue("root").empty() ? this->_serverData->getValue("root")[0] : locations[i].getValue("root")[0];
            resourcePath = ResponseTools::assemblePath(root, requestURI);

            resource = findResourceByIndex(indexes, resourcePath);
            autoindex = locations[i].getValue("autoindex");
            if (ResponseTools::isDirectory(resourcePath) && !autoindex.empty() && resourcePath.find(locationPath) != std::string::npos) {
                if (autoindex[0] == "on" && resource.empty()) {
                    this->_body = ResponseTools::autoindex(resourcePath, this->_serverData->getValue("listen")[1]);
                    if (this->_body.empty())
                        HTTPError("404");
                    this->_status = "200";
                    this->_type = "html";
                    return ;
                }
            }

            if ((!resource.empty()) && resource.find(locationPath) != std::string::npos)
                break ;
        }
    }

    if (resource.empty()) {
        indexes = this->_serverData->getValue("index");
        redirect = this->_serverData->getValue("redirect");
        root = redirect.empty() ? this->_serverData->getValue("root")[0] : this->_serverData->getValue("redirect")[0];
        redirected = redirect.empty() ? false : true;
        resourcePath = ResponseTools::assemblePath(root, requestURI);

        resource = findResourceByIndex(indexes, resourcePath);
        if (resource.empty() && ResponseTools::fileExists(resourcePath))
            resource = resourcePath;
    }

    autoindex = verifyLocationAutoindexOverride(resourcePath);

    if (ResponseTools::isDirectory(resourcePath) && !autoindex.empty()) {
        if (autoindex[0] == "on" && resource.empty()) {
            this->_body = ResponseTools::autoindex(resourcePath, this->_serverData->getValue("listen")[1]);
            if (this->_body.empty())
                HTTPError("404");
            this->_status = redirected ? "301" : "200";
            this->_type = "html";
            return ;
        }
    }

    if (ResponseTools::isDirectory(resource) || resource.empty()) {
        HTTPError("404");
        return ;
    }

    this->_type = ResponseTools::getFileExtension(resource);
    std::ifstream resourceContent(resource.c_str());
    std::string   body((std::istreambuf_iterator<char>(resourceContent)),
        std::istreambuf_iterator<char>());

    if (body.length() == 0) {
        this->_status = "204";
        this->_type = "txt";
        this->_body = "No Content\n";
    }

    this->_status = redirected ? "301" : "200";
    this->_body = body;
}

void Response::postResource(std::string requestURI)
{
    std::string root, resource;
    bool redirected = false;

    std::vector<std::string> maxSizeVector = this->_serverData->getValue("client_max_body_size");
    if (!maxSizeVector.empty()) {
        size_t maxSize = ResponseTools::convertMaxBodySizeToNumber(maxSizeVector[0]);

        if (this->_request.getBody().length() > maxSize) {
            HTTPError("413");
            return ;
        }
    }

    std::vector<Location> locations = this->_serverData->getLocations();
    if (!locations.empty()) {
        for (size_t i = 0; i < locations.size(); i++) {

            std::string locationPath = locations[i].getPath();

            root = locations[i].getValue("root").empty() ? this->_serverData->getValue("root")[0] : locations[i].getValue("root")[0];
            resource = ResponseTools::assemblePath(root, requestURI);

            if ((!resource.empty()) && resource.find(locationPath) != std::string::npos) {
                break ;
            } else {
                resource.clear();
            }
        }
    }
    if (resource.empty()) {
        root = this->_serverData->getValue("redirect").empty() ? this->_serverData->getValue("root")[0] : this->_serverData->getValue("redirect")[0];
        redirected = this->_serverData->getValue("redirect").empty() ? false : true;
        resource = ResponseTools::assemblePath(root, requestURI);
    }

    std::ofstream newFile;
    std::string requestBody = this->_request.getBody();

    newFile.open(resource.c_str(), std::ios::binary);

    if (newFile.fail()) {
        HTTPError("500");
        return;
    }

    newFile.write(requestBody.c_str(), requestBody.length());
    newFile.close();

    this->_status = redirected ? "301" : "201";
    this->_type = "txt";
    this->_body = redirected ? "Created in Redirected Location\n" : "Created Successfully\n";
    this->_headers.insert(std::make_pair("Location", resource));
}

void Response::deleteResource(std::string requestURI)
{
    std::string root, resource;
    bool redirected = false;

    std::vector<Location> locations = this->_serverData->getLocations();
    if (!locations.empty()) {
        for (size_t i = 0; i < locations.size(); i++) {

            std::string locationPath = locations[i].getPath();

            root = locations[i].getValue("root").empty() ? this->_serverData->getValue("root")[0] : locations[i].getValue("root")[0];
            resource = ResponseTools::assemblePath(root, requestURI);

            if ((!resource.empty()) && resource.find(locationPath) != std::string::npos) {
                break ;
            } else {
                resource.clear();
            }
        }
    }
    if (resource.empty()) {
        root = this->_serverData->getValue("redirect").empty() ? this->_serverData->getValue("root")[0] : this->_serverData->getValue("redirect")[0];
        redirected = this->_serverData->getValue("redirect").empty() ? false : true;
        resource = ResponseTools::assemblePath(root, requestURI);
    }

    if (ResponseTools::isDirectory(resource)) {
        HTTPError("403");
        return;
    }

    if (!ResponseTools::fileExists(resource)) {
        HTTPError("404");
        return;
    }

    std::remove(resource.c_str());

    this->_status = redirected ? "301" : "200";
    this->_type = "txt";
    this->_body = redirected ? "Deleted in Redirected Location\n" : "Deleted Successfully\n";
}

std::string Response::findResourceByIndex(std::vector<std::string> indexes, std::string resourcePath)
{
    std::string resource;

    if (!indexes.empty()) {
        for (size_t i = 0; i < indexes.size(); i++) {
            resource = ResponseTools::endsWith(resourcePath, indexes[i]) ? resourcePath : resourcePath + indexes[i];
                if (ResponseTools::fileExists(resource)) {
                    return resource;
            }
        }
    } else {
        resource = resourcePath;
        if (ResponseTools::fileExists(resource)) {
            return resource;
        }
    }
    return "";
}

void Response::HTTPError(std::string status)
{
    std::string errorPagePath;
    std::string requestURI = this->_request.getRequestURI();
    std::vector<std::string> errorPages;

    std::vector<Location> locations = this->_serverData->getLocations();

    for (size_t i = 0; i < locations.size(); i++) {
        errorPages = locations[i].getValue("error_page");
        std::string locationPath = locations[i].getPath();

        if (!errorPages.empty() && requestURI.find(locationPath) != std::string::npos) {
            for (size_t j = 0; j < errorPages.size(); j++) {
                std::string errorPage = errorPages[j];
                size_t delimiter = errorPage.find(':');
                std::string errorCode = errorPage.substr(0, delimiter);
                errorPagePath = errorPage.substr(delimiter + 1);

                if (errorCode == status) {
                    std::string root = locations[i].getValue("root").empty() ? this->_serverData->getValue("root")[0] : locations[i].getValue("root")[0];
                    root = ResponseTools::assemblePath(root, locationPath);
                    errorPagePath = ResponseTools::assemblePath(root, errorPagePath);

                    if (ResponseTools::fileExists(errorPagePath) && !errorPagePath.empty() && errorPagePath.find(locationPath) != std::string::npos) {
                        setErrorPage(status, errorPagePath);
                        return;
                    }
                }
            }
        }
    }

    std::string root = this->_serverData->getValue("root")[0];
    errorPages = this->_serverData->getValue("error_page");

    for (size_t i = 0; i < errorPages.size(); ++i) {
        std::string errorPage = errorPages[i];
        size_t delimiter = errorPage.find(':');
        std::string errorCode = errorPage.substr(0, delimiter);
        errorPagePath = errorPage.substr(delimiter + 1);

        if (errorCode == status) {
            errorPagePath = ResponseTools::assemblePath(root, errorPagePath);

            if (ResponseTools::fileExists(errorPagePath)) {
                setErrorPage(status, errorPagePath);
                return ;
            }
        }
    }

    errorPagePath = "./examples/error/" + status + ".html";
    if (ResponseTools::fileExists(errorPagePath)) {
        setErrorPage(status, errorPagePath);
        return ;
    }
}

void Response::setErrorPage(std::string status, std::string path)
{
    this->_status = status;
    this->_type = "html";
    std::ifstream error(path.c_str());
    std::string body((std::istreambuf_iterator<char>(error)), std::istreambuf_iterator<char>());
    this->_body = body;
}

std::vector<std::string> Response::verifyLocationAutoindexOverride(std::string resourcePath)
{
    std::vector<Location> locations = this->_serverData->getLocations();
    if (!locations.empty()) {
        for (size_t i = 0; i < locations.size(); i++) {
            if (resourcePath.find(locations[i].getPath()) != std::string::npos) {
                std::vector<std::string> autoindex = locations[i].getValue("autoindex");
                if (!autoindex.empty())
                    return autoindex;
            }
        }
    }
    return this->_serverData->getValue("autoindex");
}

void Response::validateServerName(void)
{
    std::vector<std::string> serverNames = this->_serverData->getValue("server_name");
    std::map<std::string, std::string> headers = this->_request.getHeaders();
    std::string host, name;
    int pos;

    host = headers.find("host")->second;
    pos = host.find(':');
    name = host.substr(0, pos);

    for (size_t i = 0; i < serverNames.size(); i++) {
        if (serverNames[i] == name)
            return;
    }
    HTTPError("404");
}


void Response::clear(void)
{
    this->_responseString.clear();
    this->_statusLine.clear();
    this->_headers.clear();
    this->_body.clear();
    this->_status.clear();
    this->_type.clear();
}
