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
    this->_statusCodes.insert(std::make_pair("100", "Continue"));
    this->_statusCodes.insert(std::make_pair("101", "Switching Protocols"));
    this->_statusCodes.insert(std::make_pair("102", "Processing"));
    this->_statusCodes.insert(std::make_pair("200", "OK"));
    this->_statusCodes.insert(std::make_pair("201", "Created"));
    this->_statusCodes.insert(std::make_pair("202", "Accepted"));
    this->_statusCodes.insert(std::make_pair("203", "Non-Authoritative Information"));
    this->_statusCodes.insert(std::make_pair("204", "No Content"));
    this->_statusCodes.insert(std::make_pair("205", "Reset Content"));
    this->_statusCodes.insert(std::make_pair("206", "Partial Content"));
    this->_statusCodes.insert(std::make_pair("207", "Multi-Status"));
    this->_statusCodes.insert(std::make_pair("208", "Already Reported"));
    this->_statusCodes.insert(std::make_pair("226", "IM Used"));
    this->_statusCodes.insert(std::make_pair("300", "Multiple Choices"));
    this->_statusCodes.insert(std::make_pair("301", "Moved Permanently"));
    this->_statusCodes.insert(std::make_pair("302", "Found"));
    this->_statusCodes.insert(std::make_pair("303", "See Other"));
    this->_statusCodes.insert(std::make_pair("304", "Not Modified"));
    this->_statusCodes.insert(std::make_pair("305", "Use Proxy"));
    this->_statusCodes.insert(std::make_pair("307", "Temporary Redirect"));
    this->_statusCodes.insert(std::make_pair("308", "Permanent Redirect"));
    this->_statusCodes.insert(std::make_pair("400", "Bad Request"));
    this->_statusCodes.insert(std::make_pair("401", "Unauthorized"));
    this->_statusCodes.insert(std::make_pair("402", "Payment Required"));
    this->_statusCodes.insert(std::make_pair("403", "Forbidden"));
    this->_statusCodes.insert(std::make_pair("404", "Not Found"));
    this->_statusCodes.insert(std::make_pair("405", "Method Not Allowed"));
    this->_statusCodes.insert(std::make_pair("406", "Not Acceptable"));
    this->_statusCodes.insert(std::make_pair("407", "Proxy Authentication Required"));
    this->_statusCodes.insert(std::make_pair("408", "Request Timeout"));
    this->_statusCodes.insert(std::make_pair("409", "Conflict"));
    this->_statusCodes.insert(std::make_pair("410", "Gone"));
    this->_statusCodes.insert(std::make_pair("411", "Length Required"));
    this->_statusCodes.insert(std::make_pair("412", "Precondition Failed"));
    this->_statusCodes.insert(std::make_pair("413", "Payload Too Large"));
    this->_statusCodes.insert(std::make_pair("414", "URI Too Long"));
    this->_statusCodes.insert(std::make_pair("415", "Unsupported Media Type"));
    this->_statusCodes.insert(std::make_pair("416", "Range Not Satisfiable"));
    this->_statusCodes.insert(std::make_pair("417", "Expectation Failed"));
    this->_statusCodes.insert(std::make_pair("418", "I'm a teapot"));
    this->_statusCodes.insert(std::make_pair("421", "Misdirected Request"));
    this->_statusCodes.insert(std::make_pair("422", "Unprocessable Entity"));
    this->_statusCodes.insert(std::make_pair("423", "Locked"));
    this->_statusCodes.insert(std::make_pair("424", "Failed Dependency"));
    this->_statusCodes.insert(std::make_pair("425", "Too Early"));
    this->_statusCodes.insert(std::make_pair("426", "Upgrade Required"));
    this->_statusCodes.insert(std::make_pair("428", "Precondition Required"));
    this->_statusCodes.insert(std::make_pair("429", "Too Many Requests"));
    this->_statusCodes.insert(std::make_pair("431", "Request Header Fields Too Large"));
    this->_statusCodes.insert(std::make_pair("451", "Unavailable For Legal Reasons"));
    this->_statusCodes.insert(std::make_pair("500", "Internal Server Error"));
    this->_statusCodes.insert(std::make_pair("501", "Not Implemented"));
    this->_statusCodes.insert(std::make_pair("502", "Bad Gateway"));
    this->_statusCodes.insert(std::make_pair("503", "Service Unavailable"));
    this->_statusCodes.insert(std::make_pair("504", "Gateway Timeout"));
    this->_statusCodes.insert(std::make_pair("505", "HTTP Version Not Supported"));
    this->_statusCodes.insert(std::make_pair("506", "Variant Also Negotiates"));
    this->_statusCodes.insert(std::make_pair("507", "Insufficient Storage"));
    this->_statusCodes.insert(std::make_pair("508", "Loop Detected"));
    this->_statusCodes.insert(std::make_pair("510", "Not Extended"));
    this->_statusCodes.insert(std::make_pair("511", "Network Authentication Required"));
}

void Response::_initContentTypes(void)
{
    this->_contentTypes.insert(std::make_pair("txt", "text/plain"));
    this->_contentTypes.insert(std::make_pair("html", "text/html"));
    this->_contentTypes.insert(std::make_pair("css", "text/css"));
    this->_contentTypes.insert(std::make_pair("js", "text/javascript"));
    this->_contentTypes.insert(std::make_pair("json", "application/json"));
    this->_contentTypes.insert(std::make_pair("xml", "application/xml"));
    this->_contentTypes.insert(std::make_pair("pdf", "application/pdf"));
    this->_contentTypes.insert(std::make_pair("zip", "application/zip"));
    this->_contentTypes.insert(std::make_pair("gzip", "application/gzip"));
    this->_contentTypes.insert(std::make_pair("tar", "application/x-tar"));
    this->_contentTypes.insert(std::make_pair("png", "image/png"));
    this->_contentTypes.insert(std::make_pair("jpg", "image/jpeg"));
    this->_contentTypes.insert(std::make_pair("jpeg", "image/jpeg"));
    this->_contentTypes.insert(std::make_pair("gif", "image/gif"));
    this->_contentTypes.insert(std::make_pair("svg", "image/svg+xml"));
    this->_contentTypes.insert(std::make_pair("ico", "image/x-icon")); 
}