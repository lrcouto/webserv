/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:53 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/01 01:52:05 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Response.hpp"

Response::Response(void)
{
    _initStatusCodes();
    _initContentTypes();
    return ;
}

Response::Response(Request request) : _request(request)
{
    _initStatusCodes();
    _initContentTypes();
    return ;
}

Response::~Response(void)
{
    clear();
    return ;
}

Response::Response(Response const &other)
{
    *this = other;
	return ;
}

Response &Response::operator=(Response const &other)
{
    if (this != &other) {
        this->_request = other._request;
        this->_responseString = other._responseString;
        this->_statusLine = other._statusLine;
        this->_headers = other._headers;
        this->_body = other._body;
        this->_status = other._status;
        this->_type = other._type;
        this->_serverData = other._serverData;
    }
    return *this;
}

std::string Response::getResponseString(void)
{
    return this->_responseString;
}

void  Response::setServerData(Server *serverData)
{
    this->_serverData = serverData;
}

void Response::setRequest(Request request)
{
    this->_request = request;
    return ;
}

void Response::assembleResponseString(void)
{
    std::string responseString, headersString;

    assembleBody();
    assembleHeaders();
    assembleStatusLine();
    
    for (std::map<std::string, std::string>::iterator it = this->_headers.begin(); it != this->_headers.end(); ++it) {
        headersString = headersString + it->first + ": " + it->second + CRLF;
    }

    responseString = this->_statusLine + CRLF + headersString + CRLF + this->_body;

    this->_responseString = responseString;
}

void Response::assembleStatusLine()
{
    std::string statusCode, protocol;
    std::map<std::string, std::string>::iterator it = this->_statusCodes.find(this->_status);
    protocol = "HTTP/1.1";
    this->_statusLine = protocol + SP + it->first + SP + it->second;
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
    std::string resourcePath, requestURI, root, resource;
    
    requestURI = this->_request.getRequestURI();
    root = this->_serverData->getValue("root")[0];
    resourcePath = ResponseTools::assemblePath(root, requestURI);

    std::vector<std::string> indexes = this->_serverData->getValue("index");   // not looking into locations yet.
    for (size_t i = 0; i < indexes.size(); i++) {
        resource = resourcePath + indexes[i];
        if (ResponseTools::fileExists(resource)) {
            this->_status = "200";
            break ;
        }
        this->_status = "404";
    }

    if (this->_status == "404")
    {
        this->_type = "html";
        resource = "./examples/404.html";
        std::ifstream error404(resource.c_str());
        std::string body((std::istreambuf_iterator<char>(error404)), std::istreambuf_iterator<char>());
        this->_body = body;
        return ;
    }
    
    this->_type = ResponseTools::getFileExtension(resource);
    std::ifstream resourceContent(resource.c_str());
    std::string body((std::istreambuf_iterator<char>(resourceContent)), std::istreambuf_iterator<char>());

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

void Response::_initStatusCodes(void)
{
    this->_statusCodes["100"] = "Continue";
    this->_statusCodes["101"] = "Switching Protocols";
    this->_statusCodes["102"] = "Processing";
    this->_statusCodes["200"] = "OK";
    this->_statusCodes["201"] = "Created";
    this->_statusCodes["202"] = "Accepted";
    this->_statusCodes["203"] = "Non-Authoritative Information";
    this->_statusCodes["204"] = "No Content";
    this->_statusCodes["205"] = "Reset Content";
    this->_statusCodes["206"] = "Partial Content";
    this->_statusCodes["207"] = "Multi-Status";
    this->_statusCodes["208"] = "Already Reported";
    this->_statusCodes["226"] = "IM Used";
    this->_statusCodes["300"] = "Multiple Choices";
    this->_statusCodes["301"] = "Moved Permanently";
    this->_statusCodes["302"] = "Found";
    this->_statusCodes["303"] = "See Other";
    this->_statusCodes["304"] = "Not Modified";
    this->_statusCodes["305"] = "Use Proxy";
    this->_statusCodes["307"] = "Temporary Redirect";
    this->_statusCodes["308"] = "Permanent Redirect";
    this->_statusCodes["400"] = "Bad Request";
    this->_statusCodes["401"] = "Unauthorized";
    this->_statusCodes["402"] = "Payment Required";
    this->_statusCodes["403"] = "Forbidden";
    this->_statusCodes["404"] = "Not Found";
    this->_statusCodes["405"] = "Method Not Allowed";
    this->_statusCodes["406"] = "Not Acceptable";
    this->_statusCodes["407"] = "Proxy Authentication Required";
    this->_statusCodes["408"] = "Request Timeout";
    this->_statusCodes["409"] = "Conflict";
    this->_statusCodes["410"] = "Gone";
    this->_statusCodes["411"] = "Length Required";
    this->_statusCodes["412"] = "Precondition Failed";
    this->_statusCodes["413"] = "Payload Too Large";
    this->_statusCodes["414"] = "URI Too Long";
    this->_statusCodes["415"] = "Unsupported Media Type";
    this->_statusCodes["416"] = "Range Not Satisfiable";
    this->_statusCodes["417"] = "Expectation Failed";
    this->_statusCodes["418"] = "I'm a teapot";
    this->_statusCodes["421"] = "Misdirected Request";
    this->_statusCodes["422"] = "Unprocessable Entity";
    this->_statusCodes["423"] = "Locked";
    this->_statusCodes["424"] = "Failed Dependency";
    this->_statusCodes["425"] = "Too Early";
    this->_statusCodes["426"] = "Upgrade Required";
    this->_statusCodes["428"] = "Precondition Required";
    this->_statusCodes["429"] = "Too Many Requests";
    this->_statusCodes["431"] = "Request Header Fields Too Large";
    this->_statusCodes["451"] = "Unavailable For Legal Reasons";
    this->_statusCodes["500"] = "Internal Server Error";
    this->_statusCodes["501"] = "Not Implemented";
    this->_statusCodes["502"] = "Bad Gateway";
    this->_statusCodes["503"] = "Service Unavailable";
    this->_statusCodes["504"] = "Gateway Timeout";
    this->_statusCodes["505"] = "HTTP Version Not Supported";
    this->_statusCodes["506"] = "Variant Also Negotiates";
    this->_statusCodes["507"] = "Insufficient Storage";
    this->_statusCodes["508"] = "Loop Detected";
    this->_statusCodes["510"] = "Not Extended";
    this->_statusCodes["511"] = "Network Authentication Required";
}

void Response::_initContentTypes(void)
{
    this->_contentTypes["txt"] = "text/plain";
    this->_contentTypes["html"] = "text/html";
    this->_contentTypes["css"] = "text/css";
    this->_contentTypes["js"] = "text/javascript";
    this->_contentTypes["json"] = "application/json";
    this->_contentTypes["xml"] = "application/xml";
    this->_contentTypes["pdf"] = "application/pdf";
    this->_contentTypes["zip"] = "application/zip";
    this->_contentTypes["gzip"] = "application/gzip";
    this->_contentTypes["tar"] = "application/x-tar";
    this->_contentTypes["png"] = "image/png";
    this->_contentTypes["jpg"] = "image/jpeg";
    this->_contentTypes["jpeg"] = "image/jpeg";
    this->_contentTypes["gif"] = "image/gif";
    this->_contentTypes["svg"] = "image/svg+xml";
    this->_contentTypes["ico"] = "image/x-icon";
}