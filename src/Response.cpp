/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:53 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/28 02:04:17 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Response.hpp"

Response::Response(void)
{
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
    }
    return *this;
}

std::string Response::getResponseString(void)
{
    return this->_responseString;
}

void Response::setRequest(Request request)
{
    this->_request = request;
    return ;
}

void Response::assembleResponseString(void)
{
    std::string responseString;

    this->_responseString = responseString;
}

void Response::clear(void)
{
    this->_request.clear();
    this->_responseString.clear();
    this->_statusLine.clear();
    this->_headers.clear();
    this->_body.clear();
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
    
}