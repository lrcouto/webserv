/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:53 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/06 21:10:17 by lcouto           ###   ########.fr       */
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
    std::string responseString, headersString;

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
                    this->_status = "405";
                    this->_type = "txt";
                    this->_body = "Method not Allowed\n";
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
        this->_status = "501";
        this->_type = "txt";
        this->_body = "Not Implemented\n";
    }
}

void Response::getResource(std::string requestURI)
{
    std::string resourcePath, root, resource;
    std::vector<std::string> indexes;
    
    std::vector<Location> locations = this->_serverData->getLocations();
    if (!locations.empty()) {
        for (size_t i = 0; i < locations.size(); i++) {

            std::string locationPath = locations[i].getPath();

            indexes = locations[i].getValue("index");
            root = locations[i].getValue("root").empty() ? this->_serverData->getValue("root")[0] : locations[i].getValue("root")[0];
            resourcePath = ResponseTools::assemblePath(root, requestURI);
            
            resource = findResourceByIndex(indexes, resourcePath);
            if ((!resource.empty()) && resource.find(locationPath) != std::string::npos)
                break ;
        }
    }
    
    if (resource.empty()) {
        indexes = this->_serverData->getValue("index");
        root = this->_serverData->getValue("root")[0];
        resourcePath = ResponseTools::assemblePath(root, requestURI);

        resource = findResourceByIndex(indexes, resourcePath);
    }

    if (ResponseTools::isDirectory(resource) || resource.empty()) {
        this->_status = "404";
        this->_type = "html";
        resource    = "./examples/404.html";
        std::ifstream error404(resource.c_str());
        std::string   body((std::istreambuf_iterator<char>(error404)),
                         std::istreambuf_iterator<char>());
        this->_body = body;
        return;
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

    this->_status = "200";
    this->_body = body;
}

void Response::postResource(std::string requestURI)
{
    std::string root, resource;

    std::vector<std::string> maxSizeVector = this->_serverData->getValue("client_max_body_size");
    if (!maxSizeVector.empty()) {
        size_t maxSize = ResponseTools::convertMaxBodySizeToNumber(maxSizeVector[0]);

        if (this->_request.getBody().length() > maxSize) {
            this->_status = "413";
            this->_type = "txt";
            this->_body = "Payload Too Large\n";
        }   
    }

    std::vector<Location> locations = this->_serverData->getLocations();
    if (!locations.empty()) {
        for (size_t i = 0; i < locations.size(); i++) {

            std::string locationPath = locations[i].getPath();

            root = locations[i].getValue("root").empty() ? this->_serverData->getValue("root")[0] : locations[i].getValue("root")[0];
            resource = ResponseTools::assemblePath(root, requestURI);
            
            if ((!resource.empty()) && resource.find(locationPath) != std::string::npos)
                break ;
        }
    }
    if (resource.empty()) {
        root = this->_serverData->getValue("root")[0];
        resource = ResponseTools::assemblePath(root, requestURI);
    }

    std::ofstream newFile;
    std::string requestBody = this->_request.getBody();

    newFile.open(resource.c_str(), std::ios::binary);

    if (newFile.fail()) {
        this->_status = "500";
        this->_type = "txt";
        this->_body = "Internal Server Error\n";
        return;
    }

    newFile.write(requestBody.c_str(), requestBody.length());
    newFile.close();

    this->_status = "201";
    this->_type = "txt";
    this->_body = "Created Successfully\n";
    this->_headers.insert(std::make_pair("Location", resource));
}

void Response::deleteResource(std::string requestURI)
{
    std::string root, resource;

    std::vector<Location> locations = this->_serverData->getLocations();
    if (!locations.empty()) {
        for (size_t i = 0; i < locations.size(); i++) {

            std::string locationPath = locations[i].getPath();

            root = locations[i].getValue("root").empty() ? this->_serverData->getValue("root")[0] : locations[i].getValue("root")[0];
            resource = ResponseTools::assemblePath(root, requestURI);
            
            if ((!resource.empty()) && resource.find(locationPath) != std::string::npos)
                break ;
        }
    }
    if (resource.empty()) {
        root = this->_serverData->getValue("root")[0];
        resource = ResponseTools::assemblePath(root, requestURI);
    }

    if (!ResponseTools::fileExists(resource)) {
        this->_status = "404";
        this->_type = "html";
        resource = "./examples/404.html";
        std::ifstream error404(resource.c_str());
        std::string body((std::istreambuf_iterator<char>(error404)), std::istreambuf_iterator<char>());
        this->_body = body;
        return;
    }

    if (ResponseTools::isDirectory(resource)) {
        this->_status = "403";
        this->_type = "txt";
        this->_body = "Forbidden\n";
        return;
    }

    std::remove(resource.c_str());

    this->_status = "200";
    this->_type = "txt";
    this->_body = "Deleted Successfully\n";
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

void Response::clear(void)
{
    this->_request.clear();
    this->_responseString.clear();
    this->_statusLine.clear();
    this->_headers.clear();
    this->_body.clear();
    this->_status.clear();
    this->_type.clear();
}
