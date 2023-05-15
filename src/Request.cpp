/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:28 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/15 00:50:53 by lcouto           ###   ########.fr       */
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
        this->_raw          = other._raw;
        this->_method       = other._method;
        this->_requestURI   = other._requestURI;
        this->_queryString  = other._queryString;
        this->_protocol     = other._protocol;
        this->_headers      = other._headers;
        this->_body         = other._body;
        this->_hasError     = other._hasError;
        this->_errorCode    = other._errorCode;
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

std::ostream &operator<<(std::ostream &out, Request &in)
{
    out << "\n\e[1;32mRaw Request:\e[0m" << std::endl << in.getRawRequest() << std::endl;
    out << "\e[1;32mMethod: \e[0m" << in.getMethod() << std::endl;
    out << "\e[1;32mRequest URI\e[0m: " << in.getRequestURI() << std::endl;
    out << "\e[1;32mQuery String\e[0m: " << in.getQueryString() << std::endl;
    out << "\e[1;32mProtocol\e[0m: " << in.getProtocol() << std::endl;
    out << "\e[1;32mHeaders:\e[0m" << std::endl;
    std::map<std::string, std::string>                 headers = in.getHeaders();
    std::map<std::string, std::string>::const_iterator it;
    for (it = headers.begin(); it != headers.end(); ++it) {
        out << it->first << ": " << it->second << std::endl;
    }
    out << "\e[1;32mBody:\e[0m" << std::endl << in.getBody() << std::endl;
    return out;
}
