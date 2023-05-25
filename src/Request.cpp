/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:28 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/24 20:52:29 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(void) : _hasError(false) {}

Request::Request(std::map<std::string, std::string> headers,
                 std::string                        method,
                 std::string                        requestURI,
                 std::string                        queryString,
                 std::string                        protocol,
                 std::string                        body,
                 std::string                        raw,
                 std::string                        errorCode,
                 bool                               hasError) :
    _headers(headers),
    _method(method),
    _requestURI(requestURI),
    _queryString(queryString),
    _protocol(protocol),
    _body(body),
    _raw(raw),
    _hasError(hasError),
    _errorCode(errorCode)
{
}

Request::~Request(void) {}

Request::Request(Request const &other) { *this = other; }

Request &Request::operator=(Request const &other)
{
    if (this != &other) {
        this->_raw         = other._raw;
        this->_method      = other._method;
        this->_requestURI  = other._requestURI;
        this->_queryString = other._queryString;
        this->_protocol    = other._protocol;
        this->_headers     = other._headers;
        this->_body        = other._body;
        this->_hasError    = other._hasError;
        this->_errorCode   = other._errorCode;
    }
    return *this;
}

std::string const &Request::getRawRequest(void) const { return this->_raw; }

std::string const &Request::getMethod(void) const { return this->_method; }

std::string const &Request::getRequestURI(void) const { return this->_requestURI; }

std::string const &Request::getQueryString(void) const { return this->_queryString; }

std::string const &Request::getProtocol(void) const { return this->_protocol; }

std::string const &Request::getBody(void) const { return this->_body; }

std::string const &Request::getErrorCode(void) const { return this->_errorCode; }

std::map<std::string, std::string> const &Request::getHeaders(void) const { return this->_headers; }

void Request::setError(std::string const &errorCode) { this->_errorCode = errorCode; }

bool Request::hasError(void) const { return this->_hasError; }

std::ostream &operator<<(std::ostream &out, Request const &in)
{
    out << BLUE "Raw Request:" RESET << std::endl << in.getRawRequest() << std::endl;
    out << BLUE "Method:" RESET << in.getMethod() << std::endl;
    out << BLUE "Request URI:" RESET << in.getRequestURI() << std::endl;
    out << BLUE "Query String:" RESET << in.getQueryString() << std::endl;
    out << BLUE "Protocol:" RESET << in.getProtocol() << std::endl;
    out << BLUE "Headers:" RESET << std::endl;
    std::map<std::string, std::string>                 headers = in.getHeaders();
    std::map<std::string, std::string>::const_iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
        out << it->first << ": " << it->second << std::endl;
    out << BLUE "Body:" RESET << std::endl << in.getBody() << std::endl;
    return (out);
}

std::ostream &operator<<(std::stringstream &out, Request const &in)
{
    out << BLUE "Raw Request:" RESET << std::endl << in.getRawRequest();
    out << BLUE "Method: " RESET << in.getMethod() << std::endl;
    out << BLUE "Request URI: " RESET << in.getRequestURI() << std::endl;
    out << BLUE "Query String: " RESET << in.getQueryString() << std::endl;
    out << BLUE "Protocol: " RESET << in.getProtocol() << std::endl;
    out << BLUE "Headers:" RESET << std::endl;
    std::map<std::string, std::string>                 headers = in.getHeaders();
    std::map<std::string, std::string>::const_iterator it;
    for (it = headers.begin(); it != headers.end(); ++it)
        out << it->first << ": " << it->second << std::endl;
    out << BLUE "Body:" RESET << std::endl << in.getBody() << std::endl;
    return (out);
}
