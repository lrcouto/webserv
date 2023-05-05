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

RequestTools::RequestTools(void) : _done_parsing(false) {}

RequestTools::RequestTools(std::string &raw) : _raw(raw), _done_parsing(false) {}

RequestTools::~RequestTools(void) {}

RequestTools::RequestTools(RequestTools const &src) { *this = src; }

RequestTools &RequestTools::operator=(RequestTools const &src)
{
    if (this != &src) {
        _raw          = src._raw;
        _request      = src._request;
        _done_parsing = src._done_parsing;
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
                break;

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

            case WSV_REQUEST_LF: // TODO: Do something when parsing finishes
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                state = WSV_REQUEST_DONE;
                break;

            default:
                throw std::exception(); // TODO: Custom exception
                break;
        }
        ++it;
        // TODO: Do something if end() is reached and parsing is not complete
    }
}

void RequestTools::parseHeaderLine(std::string &rawRequest)
{
    std::string::const_iterator it    = rawRequest.begin();
    HeaderLineState             state = WSV_HEADER_START;

    while (it != rawRequest.end()) {

        switch (state) {

            case WSV_HEADER_START:
                _header_key_begin = it;

                switch (*it) {
                    case CR:
                        state = WSV_HEADER_FINAL_LF;
                        break;
                    default:
                        if (!std::isalnum(*it))
                            throw std::exception(); // TODO: 400 Bad Request
                        state = WSV_HEADER_KEY;
                        break;
                }
                break;

            case WSV_HEADER_KEY:
                if (*it == ':') {
                    _header_key.clear();
                    _header_key.insert(_header_key.begin(), _header_key_begin, it);
                    state = WSV_HEADER_SPACE_BEFORE_VALUE;
                    break;
                }
                if (!std::isalnum(*it) && *it != '-')
                    throw std::exception(); // TODO: 400 Bad Request
                break;

            case WSV_HEADER_SPACE_BEFORE_VALUE:
                switch (*it) {
                    case WHITESPACE:
                        break;
                    case CR:
                    case LF:
                        throw std::exception(); // TODO: 400 Bad Request
                        break;
                    default:
                        _header_value_begin = it;
                        state               = WSV_HEADER_VALUE;
                        break;
                }
                break;

            case WSV_HEADER_VALUE:
                switch (*it) {
                    case CR:
                        _header_value.clear();
                        _header_value.insert(_header_value.begin(), _header_value_begin, it);
                        state = WSV_HEADER_LF;
                        break;
                    case LF:
                        throw std::exception(); // TODO: 400 Bad Request
                        break;
                }
                break;

            case WSV_HEADER_LF:
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                _request._headers[_header_key] = _header_value;
                state                          = WSV_HEADER_START;
                break;

            case WSV_HEADER_FINAL_LF:
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                _done_parsing = true;
                state         = WSV_HEADER_DONE;
                break;

            default:
                throw std::exception(); // TODO: Custom exception
                break;
        }
        ++it;
    }
    // TODO: Do something if end() is reached and parsing is not complete
}

/******************************************** PRIVATE ********************************************/

bool RequestTools::_isControlCharacter(int c) { return ((c >= 0 && c < 32) || c == 127); }

bool RequestTools::_isValidMethod(std::string &method)
{
    return (method == "GET" || method == "POST" || method == "DELETE");
}

bool RequestTools::_isValidProtocol(std::string &protocol) { return (protocol == "HTTP/1.1"); }
