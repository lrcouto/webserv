/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/10 12:48:47 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_TOOLS_HPP
#define REQUEST_TOOLS_HPP

#include "Request.hpp"
#include "libs.hpp"

enum RequestLineState {
    WSV_REQUEST_START,
    WSV_METHOD,
    WSV_URI_START,
    WSV_URI,
    WSV_HTTP_START,
    WSV_HTTP,
    WSV_REQUEST_ALMOST_DONE,
};

enum HeaderLineState {
    WSV_HEADER_START,
    WSV_HEADER_KEY,
    WSV_HEADER_SPACE_BEFORE_VALUE,
    WSV_HEADER_VALUE,
    WSV_HEADER_ALMOST_DONE,
    WSV_HEADER_FINAL_ALMOST_DONE,
};

enum ChunkedState {
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
};

// enum ParseExitCode { WSV_OK, WSV_ERROR };

enum RequestParsingErrorCode {
    BAD_REQUEST     = 400,
    NOT_IMPLEMENTED = 501,
};

class RequestTools {
    private:
        Request     _request;
        std::string _raw;

        std::string::const_iterator _method_begin;
        std::string::const_iterator _uri_begin;
        std::string::const_iterator _protocol_begin;
        std::string::const_iterator _header_key_begin;
        std::string::const_iterator _header_value_begin;

        std::string _header_key;
        std::string _header_value;

        bool        _done_parsing;
        size_t      _chunk_size;
        std::string _chunk_data;

    public:
        RequestTools(void);
        RequestTools(std::string &raw);
        ~RequestTools(void);
        RequestTools(RequestTools const &src);
        RequestTools &operator=(RequestTools const &src);

        void parseRequestLine(std::string &rawRequest);
        void parseHeaderLine(std::string &rawRequest);
        void parseChunkedBody(std::string &rawRequest);

        class RequestParsingException : public std::exception {
            public:
                RequestParsingException(RequestParsingErrorCode error);
                char const             *what() const throw();
                RequestParsingErrorCode get_error() const;

            private:
                RequestParsingErrorCode _error;
        };

    private:
        bool   _isControlCharacter(int c);
        bool   _isValidMethod(std::string &method);
        bool   _isValidProtocol(std::string &protocol);
        size_t _getHexValue(char c);
};

#endif /* REQUEST_TOOLS_HPP */
