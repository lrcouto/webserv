/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:28 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/25 18:51:00 by maolivei         ###   ########.fr       */
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

std::string Request::str(void) const
{
    std::string __str, formattedRaw;
    size_t      crlfcrlf;

    crlfcrlf = _raw.find("\r\n\r\n");
    if (crlfcrlf != std::string::npos)
        formattedRaw.assign(_raw.begin(), (_raw.begin() + crlfcrlf));
    __str += BLUE "Raw Request:" RESET "\n" + (formattedRaw.empty() ? _raw : formattedRaw) + "\n\n";
    __str += BLUE "Method: " RESET + _method + "\n";
    __str += BLUE "Request URI: " RESET + _requestURI + "\n";
    __str += BLUE "Query String: " RESET + _queryString + "\n";
    __str += BLUE "Protocol: " RESET + _protocol + "\n";
    __str += BLUE "Headers:" RESET "\n";
    std::map<std::string, std::string>::const_iterator it;
    for (it = _headers.begin(); it != _headers.end(); ++it)
        __str += it->first + ": " + it->second + "\n";
    __str += "\n" BLUE "Body:" RESET;
    if (!_body.empty()) {
        if (_body.size() > 512)
            __str += " Suppressed (body too large)";
        else
            __str += "\n" + _body;
    } else {
        __str += " Empty";
    }
    __str += "\n";
    return (__str);
}

std::ostream &operator<<(std::ostream &out, Request const &in)
{
    out << in.str();
    return (out);
}

std::ostream &operator<<(std::stringstream &ss, Request const &in)
{
    ss << in.str();
    return (ss);
}
