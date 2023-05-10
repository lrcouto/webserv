/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 18:25:27 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/10 16:27:44 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestTools.hpp"

RequestTools::RequestTools(void) : _done_parsing(false), _chunk_size(-1) {}

RequestTools::RequestTools(std::string &raw) :
    _raw(raw),
    _chunk_size(-1),
    _http_major(-1),
    _http_minor(-1),
    _position(_raw.begin()),
    _last(_raw.end())
{
}

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

void RequestTools::parseRequestLine(void)
{
    enum {
        WSV_REQUEST_START,
        WSV_METHOD,
        WSV_URI_START,
        WSV_URI,
        WSV_HTTP_START,
        WSV_HTTP_H,
        WSV_HTTP_HT,
        WSV_HTTP_HTT,
        WSV_HTTP_HTTP,
        WSV_FIRST_MAJOR_DIGIT,
        WSV_MAJOR_DIGIT,
        WSV_FIRST_MINOR_DIGIT,
        WSV_MINOR_DIGIT,
        WSV_REQUEST_ALMOST_DONE,
    } state;

    state = WSV_REQUEST_START;

    std::string::const_iterator it;
    for (it = _position; it != _last; ++it) {
        switch (state) {

            case WSV_REQUEST_START:
                if (*it < 'A' || *it > 'Z')
                    throw RequestParsingException(NOT_IMPLEMENTED);
                _method_begin = it;
                state         = WSV_METHOD;
                break;

            case WSV_METHOD:
                if (*it == WHITESPACE) {
                    std::string method(_method_begin, it);
                    if (!_isValidMethod(method))
                        throw RequestParsingException(NOT_IMPLEMENTED);
                    _request._method = method;
                    state            = WSV_URI_START;
                }
                break;

            case WSV_URI_START:
                if (*it != '/')
                    throw RequestParsingException(BAD_REQUEST);
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
                if (_isControlCharacter(*it))
                    throw RequestParsingException(BAD_REQUEST);
                break;

            case WSV_HTTP_START:
                if (*it != 'H')
                    throw RequestParsingException(BAD_REQUEST);
                _protocol_begin = it;
                state           = WSV_HTTP_H;
                break;

            case WSV_HTTP_H:
                if (*it != 'T')
                    throw RequestParsingException(BAD_REQUEST);
                state = WSV_HTTP_HT;
                break;

            case WSV_HTTP_HT:
                if (*it != 'T')
                    throw RequestParsingException(BAD_REQUEST);
                state = WSV_HTTP_HTT;
                break;

            case WSV_HTTP_HTT:
                if (*it != 'P')
                    throw RequestParsingException(BAD_REQUEST);
                state = WSV_HTTP_HTTP;
                break;

            case WSV_HTTP_HTTP:
                if (*it != '/')
                    throw RequestParsingException(BAD_REQUEST);
                state = WSV_FIRST_MAJOR_DIGIT;
                break;

            case WSV_FIRST_MAJOR_DIGIT:
                if (*it < '1' || *it > '9')
                    throw RequestParsingException(BAD_REQUEST);
                _http_major = *it - '0';
                if (_http_major > 1)
                    throw RequestParsingException(HTTP_VERSION_NOT_SUPPORTED);
                state = WSV_MAJOR_DIGIT;
                break;

            case WSV_MAJOR_DIGIT:
                if (*it == '.') {
                    state = WSV_FIRST_MINOR_DIGIT;
                    break;
                }
                if (*it < '0' || *it > '9')
                    throw RequestParsingException(BAD_REQUEST);
                _http_major = (_http_major * 10) + (*it - '0');
                if (_http_major > 1)
                    throw RequestParsingException(HTTP_VERSION_NOT_SUPPORTED);
                break;

            case WSV_FIRST_MINOR_DIGIT:
                if (*it < '0' || *it > '9')
                    throw RequestParsingException(BAD_REQUEST);
                _http_minor = *it - '0';
                if (_http_minor > 1)
                    throw RequestParsingException(HTTP_VERSION_NOT_SUPPORTED);
                state = WSV_MINOR_DIGIT;
                break;

            case WSV_MINOR_DIGIT:
                if (*it == CR) {
                    std::string protocol(_protocol_begin, it);
                    _request._protocol = protocol;
                    state              = WSV_REQUEST_ALMOST_DONE;
                    break;
                }
                if (*it < '0' || *it > '9')
                    throw RequestParsingException(BAD_REQUEST);
                _http_minor = (_http_minor * 10) + (*it - '0');
                if (_http_minor > 1)
                    throw RequestParsingException(HTTP_VERSION_NOT_SUPPORTED);
                break;

            case WSV_REQUEST_ALMOST_DONE:
                if (*it != LF)
                    throw RequestParsingException(BAD_REQUEST);
                _position = it + 1;
                return;
        }
    }
    throw RequestParsingException(BAD_REQUEST);
}

void RequestTools::parseHeaderLines(void)
{
    enum {
        WSV_HEADER_START,
        WSV_HEADER_KEY,
        WSV_HEADER_SPACE_BEFORE_VALUE,
        WSV_HEADER_VALUE,
        WSV_HEADER_ALMOST_DONE,
        WSV_HEADER_FINAL_ALMOST_DONE,
    } state;

    state = WSV_HEADER_START;

    std::string::const_iterator it;
    for (it = _position; it != _last; ++it) {
        switch (state) {

            case WSV_HEADER_START:
                switch (*it) {
                    case CR:
                        state = WSV_HEADER_FINAL_ALMOST_DONE;
                        break;
                    default:
                        if (!std::isalnum(*it))
                            throw RequestParsingException(BAD_REQUEST);
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
                    throw RequestParsingException(BAD_REQUEST);
                break;

            case WSV_HEADER_SPACE_BEFORE_VALUE:
                switch (*it) {
                    case WHITESPACE:
                        break;
                    case CR:
                    case LF:
                        throw RequestParsingException(BAD_REQUEST);
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
                        throw RequestParsingException(BAD_REQUEST);
                        break;
                }
                break;

            case WSV_HEADER_ALMOST_DONE:
                if (*it != LF)
                    throw RequestParsingException(BAD_REQUEST);
                _request._headers[_header_key] = _header_value;
                state                          = WSV_HEADER_START;
                break;

            case WSV_HEADER_FINAL_ALMOST_DONE:
                if (*it != LF)
                    throw RequestParsingException(BAD_REQUEST);
                _done_parsing = true;
                _position     = it + 1;
                return;
        }
    }
    throw RequestParsingException(BAD_REQUEST);
}

void RequestTools::parseChunkedBody(void)
{
    size_t chunk_data_size_at_most, hex = (size_t)-1;

    enum {
        WSV_CHUNK_START,
        WSV_CHUNK_SIZE,
        WSV_CHUNK_EXTENSION,
        WSV_CHUNK_EXTENSION_ALMOST_DONE,
        WSV_CHUNK_DATA,
        WSV_CHUNK_AFTER_DATA,
        WSV_CHUNK_LAST_EXTENSION,
        WSV_CHUNK_LAST_EXTENSION_ALMOST_DONE,
        WSV_CHUNK_TRAILER,
        WSV_CHUNK_TRAILER_ALMOST_DONE,
        WSV_CHUNK_TRAILER_HEADER,
        WSV_CHUNK_TRAILER_HEADER_ALMOST_DONE,
    } state;

    state = WSV_CHUNK_START;

    std::string::const_iterator it;
    for (it = _position; it != _last; ++it) {
        switch (state) {

            case WSV_CHUNK_START:
                hex = _getHexValue(*it);
                if (hex == (size_t)-1)
                    throw RequestParsingException(BAD_REQUEST);
                _chunk_size = hex;
                state       = WSV_CHUNK_SIZE;
                break;

            case WSV_CHUNK_SIZE:
                if (_chunk_size > (size_t)-1) // TODO: CLIENT MAX BODY SIZE
                    throw RequestParsingException(BAD_REQUEST);

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
                            throw RequestParsingException(BAD_REQUEST);
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
                        throw RequestParsingException(BAD_REQUEST);
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
                    throw RequestParsingException(BAD_REQUEST);
                state = WSV_CHUNK_DATA;
                break;

            case WSV_CHUNK_DATA:
                if (!_chunk_size) {
                    if (*it != CR)
                        throw RequestParsingException(BAD_REQUEST);
                    state = WSV_CHUNK_AFTER_DATA;
                    break;
                }
                chunk_data_size_at_most = std::min(_chunk_size, (size_t)(_last - it));
                _chunk_data.insert(_chunk_data.end(), it, (it + chunk_data_size_at_most));
                _chunk_size -= chunk_data_size_at_most;
                it += chunk_data_size_at_most - 1;
                break;

            case WSV_CHUNK_AFTER_DATA:
                if (*it != LF)
                    throw RequestParsingException(BAD_REQUEST);
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
                throw RequestParsingException(BAD_REQUEST);

            case WSV_CHUNK_TRAILER:
                switch (*it) {
                    case CR:
                        state = WSV_CHUNK_TRAILER_ALMOST_DONE;
                        break;
                    case LF:
                        _position = it + 1;
                        return;
                    default:
                        state = WSV_CHUNK_TRAILER_HEADER;
                }
                break;

            case WSV_CHUNK_TRAILER_ALMOST_DONE:
                if (*it != LF)
                    throw RequestParsingException(BAD_REQUEST);
                _position = it + 1;
                return;

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
                throw RequestParsingException(BAD_REQUEST);
        }
    }
    throw RequestParsingException(BAD_REQUEST);
}

void RequestTools::parseRequest(void)
{
    try {
        parseRequestLine();
        parseHeaderLines();
        parseChunkedBody();
    } catch (RequestParsingException const &e) {
        std::cerr << e.get_error() << ' ' << e.what() << '\n';
    } catch (std::exception const &e) {
        std::cerr << e.what() << '\n';
    }
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
    if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    return (-1);
}

/************************************ REQUEST PARSING EXCEPTION ***********************************/

RequestTools::RequestParsingException::RequestParsingException(RequestParsingErrorCode error) :
    _error(error)
{
}

char const *RequestTools::RequestParsingException::what() const throw()
{
    static std::map<int, std::string> codes;

    if (codes.empty()) {
        codes[BAD_REQUEST]                = "Bad Request";
        codes[NOT_IMPLEMENTED]            = "Not Implemented";
        codes[HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
    }

    return (codes[_error].c_str());
}

RequestParsingErrorCode RequestTools::RequestParsingException::get_error() const { return _error; }
