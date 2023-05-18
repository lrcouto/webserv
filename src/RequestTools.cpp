/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 18:25:27 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/18 18:35:45 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestTools.hpp"

RequestTools::RequestTools(std::string &raw, Server *server) :
    _server(server),
    _raw(raw),
    _max_body_size(-1),
    _chunk_size(-1),
    _http_major(-1),
    _http_minor(-1),
    _has_body(false),
    _has_chunked_body(false),
    _ignore_content_length(false),
    _position(_raw.begin()),
    _last(_raw.end()),
    _hasError(false)
{
    std::vector<std::string> max_body_size_vec;

    max_body_size_vec = _server->getValue("client_max_body_size");
    if (!max_body_size_vec.empty())
        _max_body_size = ftstring::strtoi(max_body_size_vec[0]);
}

RequestTools::~RequestTools(void) {}

RequestTools::RequestTools(RequestTools const &src) { *this = src; }

RequestTools &RequestTools::operator=(RequestTools const &src)
{
    if (this != &src) {
        _server                = src._server;
        _raw                   = src._raw;
        _max_body_size         = src._max_body_size;
        _chunk_size            = src._chunk_size;
        _http_major            = src._http_major;
        _http_minor            = src._http_minor;
        _has_body              = src._has_body;
        _has_chunked_body      = src._has_chunked_body;
        _ignore_content_length = src._ignore_content_length;
        _position              = src._position;
        _last                  = src._last;
    }
    return (*this);
}

void RequestTools::parseRequest(void)
{
    try {
        _parseRequestLine();
        _parseHeaderLines();

        if (_has_body) {
            std::map<std::string, std::string>::const_iterator it;
            it = _headers.find("transfer-encoding");
            if (it != _headers.end()) {
                std::string              te_values = ftstring::reduce(it->second, ", ");
                std::vector<std::string> te_split  = ftstring::split(te_values, ' ');
                if (te_split.back() == "chunked")
                    _has_chunked_body = true;
            }
        }

        if (_has_chunked_body) {
            _parseChunkedBody();
            _body = _chunk_data;
        } else if (_has_body) {
            _parseRegularBody();
            _body = _body_data;
        }
    } catch (RequestParsingException const &e) {
        std::cerr << e.get_error() << ' ' << e.what() << '\n';
        _hasError  = true;
        _errorCode = e.get_error();
    } catch (std::exception const &e) {
        std::cerr << e.what() << '\n';
    }
}

Request RequestTools::buildRequest(void) const
{
    return (Request(_headers, _method, _uri, _query_string, _protocol, _body, _raw, _errorCode, _hasError));
}

/******************************************** PRIVATE ********************************************/

void RequestTools::_parseRequestLine(void)
{
    enum {
        WSV_REQUEST_START,
        WSV_METHOD,
        WSV_URI_START,
        WSV_URI,
        WSV_QUERY_STRING_START,
        WSV_QUERY_STRING,
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
                    _method.assign(_method_begin, it);
                    if (!_isValidMethod(_method))
                        throw RequestParsingException(NOT_IMPLEMENTED);
                    state = WSV_URI_START;
                }
                break;

            case WSV_URI_START:
                if (*it != '/')
                    throw RequestParsingException(BAD_REQUEST);
                _uri_begin = it;
                state      = WSV_URI;
                break;

            case WSV_URI:
                if (*it == '?') {
                    _uri.assign(_uri_begin, it);
                    state = WSV_QUERY_STRING_START;
                    break;
                }
                if (*it == WHITESPACE) {
                    _uri.assign(_uri_begin, it);
                    state = WSV_HTTP_START;
                    break;
                }
                if (_isControlCharacter(*it))
                    throw RequestParsingException(BAD_REQUEST);
                break;

            case WSV_QUERY_STRING_START:
                _query_string_begin = it;
                state               = WSV_QUERY_STRING;
                break;

            case WSV_QUERY_STRING:
                if (*it == WHITESPACE) {
                    _query_string.assign(_query_string_begin, it);
                    state = WSV_HTTP_START;
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
                    _protocol.assign(_protocol_begin, it);
                    state = WSV_REQUEST_ALMOST_DONE;
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

void RequestTools::_parseHeaderLines(void)
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
                    _header_key.assign(_header_key_begin, it);
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
                        _header_value.assign(_header_value_begin, it);
                        if (ftstring::striequals(_header_key, "content-length")) {
                            if (!ftstring::is_positive_integer(_header_value))
                                throw RequestParsingException(BAD_REQUEST);
                            _has_body = true;
                        } else if (ftstring::striequals(_header_key, "transfer-encoding")) {
                            _has_body              = true;
                            _ignore_content_length = true;
                        }
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
                _headerFieldNormalizedInsert(_header_key, _header_value);
                state = WSV_HEADER_START;
                break;

            case WSV_HEADER_FINAL_ALMOST_DONE:
                if (*it != LF)
                    throw RequestParsingException(BAD_REQUEST);
                _position = it + 1;
                return;
        }
    }
    throw RequestParsingException(BAD_REQUEST);
}

void RequestTools::_parseRegularBody(void)
{
    if (!_ignore_content_length) {
        std::map<std::string, std::string>::const_iterator it;

        it = _headers.find("content-length");
        if (it != _headers.end()) {
            size_t content_length = ftstring::strtoi(it->second);
            if (content_length > _max_body_size)
                throw RequestParsingException(ENTITY_TOO_LARGE);
            if ((size_t)(_last - _position) > content_length)
                throw RequestParsingException(ENTITY_TOO_LARGE);
        }
    }
    if ((size_t)(_last - _position) > _max_body_size)
        throw RequestParsingException(ENTITY_TOO_LARGE);
    _body_data.assign(_position, _last);
}

void RequestTools::_parseChunkedBody(void)
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
                if (_chunk_size > _max_body_size)
                    throw RequestParsingException(ENTITY_TOO_LARGE);

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

                if ((_chunk_data.size() + _chunk_size) > _max_body_size)
                    throw RequestParsingException(ENTITY_TOO_LARGE);

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
                if (*it != LF)
                    throw RequestParsingException(BAD_REQUEST);
                state = WSV_CHUNK_TRAILER;
                break;

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
                if (*it != LF)
                    throw RequestParsingException(BAD_REQUEST);
                state = WSV_CHUNK_TRAILER;
                break;
        }
    }
    throw RequestParsingException(BAD_REQUEST);
}

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

void RequestTools::_headerFieldNormalizedInsert(std::string &key, std::string &value)
{
    std::map<std::string, std::string>::iterator it;

    // normalize key and value to lowercase
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if (key != "cookie")
        std::transform(value.begin(), value.end(), value.begin(), ::tolower);

    it = _headers.find(key);
    if (it != _headers.end())
        it->second += ", " + value;
    else
        _headers[key] = value;
}

/************************************ REQUEST PARSING EXCEPTION ***********************************/

RequestTools::RequestParsingException::RequestParsingException(char const *error) : _error(error) {}

char const *RequestTools::RequestParsingException::what() const throw()
{
    static std::map<std::string, std::string> codes;

    if (codes.empty()) {
        codes[BAD_REQUEST]                = "Bad Request";
        codes[ENTITY_TOO_LARGE]           = "Entity Too Large";
        codes[NOT_IMPLEMENTED]            = "Not Implemented";
        codes[HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
    }

    return (codes[_error].c_str());
}

char const *RequestTools::RequestParsingException::get_error() const { return (_error); }
