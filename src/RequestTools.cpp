/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 18:25:27 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/09 21:15:41 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestTools.hpp"

RequestTools::RequestTools(void) : _done_parsing(false), _chunk_size(-1) {}

RequestTools::RequestTools(std::string &raw) : _raw(raw), _done_parsing(false), _chunk_size(-1) {}

RequestTools::~RequestTools(void) {}

RequestTools::RequestTools(RequestTools const &src) { *this = src; }

RequestTools &RequestTools::operator=(RequestTools const &src)
{
    if (this != &src) {
        _raw          = src._raw;
        _request      = src._request;
        _done_parsing = src._done_parsing;
        _chunk_size   = src._chunk_size;
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
                    state              = WSV_REQUEST_ALMOST_DONE;
                }
                break;

            case WSV_REQUEST_ALMOST_DONE: // TODO: Do something when parsing finishes
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                return;                     // TODO: Done
        }
        ++it;
    }
    throw std::exception(); // TODO: 400 Bad Request
    // TODO: Do something if end() is reached and parsing is not complete
}

void RequestTools::parseHeaderLine(std::string &rawRequest)
{
    std::string::const_iterator it    = rawRequest.begin();
    HeaderLineState             state = WSV_HEADER_START;

    while (it != rawRequest.end()) {

        switch (state) {

            case WSV_HEADER_START:
                switch (*it) {
                    case CR:
                        state = WSV_HEADER_FINAL_ALMOST_DONE;
                        break;
                    default:
                        if (!std::isalnum(*it))
                            throw std::exception(); // TODO: 400 Bad Request
                        _header_key_begin = it;
                        state             = WSV_HEADER_KEY;
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
                        state = WSV_HEADER_ALMOST_DONE;
                        break;
                    case LF:
                        throw std::exception(); // TODO: 400 Bad Request
                        break;
                }
                break;

            case WSV_HEADER_ALMOST_DONE:
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                _request._headers[_header_key] = _header_value;
                state                          = WSV_HEADER_START;
                break;

            case WSV_HEADER_FINAL_ALMOST_DONE:
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                _done_parsing = true;
                return; // TODO: done
        }
        ++it;
    }
    throw std::exception(); // TODO: 400 Bad Request
    // TODO: Do something if end() is reached and parsing is not complete
}

void RequestTools::parseChunkedBody(std::string &rawRequest)
{
    std::string::const_iterator it    = rawRequest.begin();
    ChunkedState                state = WSV_CHUNK_START;
    size_t                      hex   = (size_t)-1;
    size_t                      remaining_size, chunk_data_size_at_most;

    while (it != rawRequest.end()) {

        switch (state) {

            case WSV_CHUNK_START:
                hex = _getHexValue(*it);
                if (hex == (size_t)-1)
                    throw std::exception(); // TODO: 400 Bad Request
                _chunk_size = hex;
                state       = WSV_CHUNK_SIZE;
                break;

            case WSV_CHUNK_SIZE:
                if (_chunk_size > (size_t)-1) // TODO: CLIENT MAX BODY SIZE
                    throw std::exception();   // TODO: 400 Bad Request

                hex = _getHexValue(*it);
                if (hex != (size_t)-1) {
                    _chunk_size = _chunk_size * 16 + hex;
                    break;
                }

                if (!_chunk_size) {
                    switch (*it) {
                        case CR:
                            state = WSV_CHUNK_LAST_EXTENSION_ALMOST_DONE;
                            break;
                        case LF:
                            state = WSV_CHUNK_TRAILER;
                            break;
                        case ';':
                        case ' ':
                        case '\t':
                            state = WSV_CHUNK_LAST_EXTENSION;
                            break;
                        default:
                            throw std::exception(); // TODO: 400 Bad Request
                    }
                    break;
                }

                // TODO: Chunk size summed up surpasses client max body size
                // if (read_body_bytes > client_max_body_size) [...]

                switch (*it) {
                    case CR:
                        state = WSV_CHUNK_EXTENSION_ALMOST_DONE;
                        break;
                    case LF:
                        state = WSV_CHUNK_DATA;
                        break;
                    case ';':
                    case ' ':
                    case '\t':
                        state = WSV_CHUNK_EXTENSION;
                        break;
                    default:
                        throw std::exception(); // TODO: 400 Bad Request
                }
                break;

            case WSV_CHUNK_EXTENSION:
                switch (*it) {
                    case CR:
                        state = WSV_CHUNK_EXTENSION_ALMOST_DONE;
                        break;
                    case LF:
                        state = WSV_CHUNK_DATA;
                }
                break;

            case WSV_CHUNK_EXTENSION_ALMOST_DONE:
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                state = WSV_CHUNK_DATA;
                break;

            case WSV_CHUNK_DATA:
                if (!_chunk_size) {
                    if (*it != CR)
                        throw std::exception(); // TODO: 400 Bad Request
                    state = WSV_CHUNK_AFTER_DATA;
                    break;
                }
                remaining_size          = rawRequest.end() - it;
                chunk_data_size_at_most = std::min(_chunk_size, remaining_size);
                _chunk_data.insert(_chunk_data.end(), it, (it + chunk_data_size_at_most));
                _chunk_size -= chunk_data_size_at_most;
                it += chunk_data_size_at_most - 1;
                break;

            case WSV_CHUNK_AFTER_DATA:
                if (*it != LF)
                    throw std::exception(); // TODO: 400 Bad Request
                state = WSV_CHUNK_START;
                break;

            case WSV_CHUNK_LAST_EXTENSION:
                switch (*it) {
                    case CR:
                        state = WSV_CHUNK_LAST_EXTENSION_ALMOST_DONE;
                        break;
                    case LF:
                        state = WSV_CHUNK_TRAILER;
                }
                break;

            case WSV_CHUNK_LAST_EXTENSION_ALMOST_DONE:
                if (*it == LF) {
                    state = WSV_CHUNK_TRAILER;
                    break;
                }
                throw std::exception(); // TODO: 400 Bad Request

            case WSV_CHUNK_TRAILER:
                switch (*it) {
                    case CR:
                        state = WSV_CHUNK_TRAILER_ALMOST_DONE;
                        break;
                    case LF:
                        return; // TODO: Chunk parsing done;
                    default:
                        state = WSV_CHUNK_TRAILER_HEADER;
                }
                break;

            case WSV_CHUNK_TRAILER_ALMOST_DONE:
                if (*it == LF)
                    return;             // TODO: Chunk parsing done
                throw std::exception(); // TODO: 400 Bad Request

            case WSV_CHUNK_TRAILER_HEADER:
                switch (*it) {
                    case CR:
                        state = WSV_CHUNK_TRAILER_HEADER_ALMOST_DONE;
                        break;
                    case LF:
                        state = WSV_CHUNK_TRAILER;
                }
                break;

            case WSV_CHUNK_TRAILER_HEADER_ALMOST_DONE:
                if (*it == LF) {
                    state = WSV_CHUNK_TRAILER;
                    break;
                }
                throw std::exception(); // TODO: 400 Bad Request
        }
        ++it;
    }
    throw std::exception(); // TODO: 400 Bad Request
}

/******************************************** PRIVATE ********************************************/

bool RequestTools::_isControlCharacter(int c) { return ((c >= 0 && c < 32) || c == 127); }

bool RequestTools::_isValidMethod(std::string &method)
{
    return (method == "GET" || method == "POST" || method == "DELETE");
}

bool RequestTools::_isValidProtocol(std::string &protocol) { return (protocol == "HTTP/1.1"); }

size_t RequestTools::_getHexValue(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    if ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
        return (c - 'a' + 10);
    return (-1);
}
