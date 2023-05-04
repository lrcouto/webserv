/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:53 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/03 21:19:44 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(void)
{
    ResponseTools::initStatusCodes(this->_statusCodes);
    ResponseTools::initContentTypes(this->_contentTypes);
    return;
}

Response::Response(Request request) : _request(request)
{
    ResponseTools::initStatusCodes(this->_statusCodes);
    ResponseTools::initContentTypes(this->_contentTypes);
    return;
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
    std::string                                  statusCode, statusMessage, protocol;
    std::map<std::string, std::string>::iterator it = this->_statusCodes.find(this->_status);

    protocol      = "HTTP/1.1";
    statusCode    = it->first;
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

    std::string                                  contentType;
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
    std::string resourcePath, requestURI, root, resource;

    requestURI   = this->_request.getRequestURI();
    root         = this->_serverData->getValue("root")[0];
    resourcePath = ResponseTools::assemblePath(root, requestURI);

    std::vector<std::string> indexes
        = this->_serverData->getValue("index"); // not looking into locations yet.
    for (size_t i = 0; i < indexes.size(); i++) {
        resource = resourcePath + indexes[i];
        if (ResponseTools::fileExists(resource)) {
            this->_status = "200";
            break;
        }
        this->_status = "404";

        if (!indexes.empty()) {
            for (size_t i = 0; i < indexes.size(); i++) {
                resource = ResponseTools::endsWith(resourcePath, indexes[i])
                    ? resourcePath
                    : resourcePath + indexes[i];
                if (ResponseTools::fileExists(resource)) {
                    this->_status = "200";
                    break;
                }
            }
        } else {
            resource = resourcePath;
            if (ResponseTools::fileExists(resource)) {
                this->_status = "200";
            }
        }
    }

    if (ResponseTools::isDirectory(resource))
        this->_status = "404";

    if (this->_status == "404") {
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

    if (body.length() == 0)
        this->_status = "204";

    this->_body = body;
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
