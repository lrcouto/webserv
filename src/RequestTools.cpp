/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42WHITESPACE.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 14:35:17 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/04 18:11:08 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestTools.hpp"

RequestTools::RequestTools(void) {}

RequestTools::RequestTools(std::string &raw) : _raw(raw) {}

RequestTools::~RequestTools(void) {}

RequestTools::RequestTools(RequestTools const &src) { *this = src; }

RequestTools &RequestTools::operator=(RequestTools const &src)
{
    if (this != &src) {
        _raw     = src._raw;
        _request = src._request;
    }
    return (*this);
}

void RequestTools::parseRequestLine(std::string &rawRequest)
{
    std::string::const_iterator it    = rawRequest.begin();
    RequestLineState            state = WSV_REQUEST_START;

    while (it != rawRequest.end()) {

        switch (state) {

            case WSV_REQUEST_START:
                if (*it < 'A' || *it > 'Z')
                    throw std::exception(); // TODO: 501 Not Implemented
                _method_begin = it;
                state         = WSV_METHOD;
                break;

            case WSV_METHOD:
                if (*it == WHITESPACE) {
                    std::string method(_method_begin, it);
                    if (!_isValidMethod(method))
                        throw std::exception(); // TODO: 501 Not Implemented
                    _request._method = method;
                    state            = WSV_URI_START;
                }
                break;

            case WSV_URI_START:
                if (*it != '/')
                    throw std::exception(); // TODO: 400 Bad Request
                _uri_begin = it;
                state      = WSV_URI;
                break;

            case WSV_URI:
                if (*it == WHITESPACE) {
                    std::string uri(_uri_begin, it);
                    _request._requestURI = uri;
                    state                = WSV_HTTP_START;
                    break;
                }
                if (!_isControlCharacter(*it))
                    break;
                throw std::exception(); // TODO: 400 Bad Request

            case WSV_HTTP_START:
                if (*it != 'H')
                    throw std::exception(); // TODO: 400 Bad Request
                _protocol_begin = it;
                state           = WSV_HTTP;
                break;

            case WSV_HTTP:
                if (*it == CR) {
                    std::string protocol(_protocol_begin, it);
                    if (!_isValidProtocol(protocol))
                        throw std::exception(); // TODO: 400 Bad Request
                    _request._protocol = protocol;
                    state              = WSV_REQUEST_LF;
                }
                break;

            case WSV_REQUEST_CR:
                if (*it != CR)
                    throw std::exception(); // TODO: 400 Bad Request
                state = WSV_REQUEST_LF;
                break;

            case WSV_REQUEST_LF:
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                state = WSV_REQUEST_DONE;
                break;

            default:
                throw std::exception(); // TODO: Custom exception
        }
        ++it;
    }
}

/******************************************** PRIVATE ********************************************/

bool RequestTools::_isControlCharacter(int c) { return ((c >= 0 && c < 32) || c == 127); }

bool RequestTools::_isValidMethod(std::string &method)
{
    return (method == "GET" || method == "POST" || method == "DELETE");
}

bool RequestTools::_isValidProtocol(std::string &protocol) { return (protocol == "HTTP/1.1"); }
