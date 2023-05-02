/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:53 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/02 20:43:52 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

std::map<std::string, std::string> Response::_statusCodes  = Response::_initStatusCodes();
std::map<std::string, std::string> Response::_contentTypes = Response::_initContentTypes();

Response::Response(void) {}

Response::Response(Request request) : _request(request) {}

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
    std::string                                  statusCode, statusPhrase, protocol;
    std::map<std::string, std::string>::iterator it = this->_statusCodes.find(this->_status);

    protocol          = "HTTP/1.1";
    statusCode        = it->first;
    statusPhrase      = it->second;
    this->_statusLine = protocol + SP + statusCode + SP + statusPhrase;
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

    // not looking into locations yet.
    if (ResponseTools::fileExists(resourcePath)) {
        this->_status = "200";
        resource      = resourcePath;
    } else {

        if (resourcePath[resourcePath.size() - 1] != '/')
            resourcePath += '/';
        std::vector<std::string> indexes = this->_serverData->getValue("index");
        for (size_t i = 0; i < indexes.size(); i++) {
            resource = resourcePath + indexes[i];
            if (ResponseTools::fileExists(resource)) {
                this->_status = "200";
                break;
            }
            this->_status = "404";
        }
    }
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

std::map<std::string, std::string> Response::_initStatusCodes(void)
{
    std::map<std::string, std::string> statusCodes;

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
    return (statusCodes);
}

std::map<std::string, std::string> Response::_initContentTypes(void)
{
    std::map<std::string, std::string> contentTypes;

    contentTypes["txt"]  = "text/plain";
    contentTypes["html"] = "text/html";
    contentTypes["css"]  = "text/css";
    contentTypes["js"]   = "text/javascript";
    contentTypes["json"] = "application/json";
    contentTypes["xml"]  = "application/xml";
    contentTypes["pdf"]  = "application/pdf";
    contentTypes["zip"]  = "application/zip";
    contentTypes["gzip"] = "application/gzip";
    contentTypes["tar"]  = "application/x-tar";
    contentTypes["png"]  = "image/png";
    contentTypes["jpg"]  = "image/jpeg";
    contentTypes["jpeg"] = "image/jpeg";
    contentTypes["gif"]  = "image/gif";
    contentTypes["svg"]  = "image/svg+xml";
    contentTypes["ico"]  = "image/x-icon";
    return (contentTypes);
}
