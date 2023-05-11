/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/11 12:19:32 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_TOOLS_HPP
#define REQUEST_TOOLS_HPP

#include "Request.hpp"
#include "Server.hpp"
#include "libs.hpp"

enum RequestParsingErrorCode {
    BAD_REQUEST                = 400,
    ENTITY_TOO_LARGE           = 413,
    NOT_IMPLEMENTED            = 501,
    HTTP_VERSION_NOT_SUPPORTED = 505,
};

class RequestTools {
    private:
        Request     _request;
        Server     *_server;
        std::string _raw;

        std::string _header_key;
        std::string _header_value;

        size_t      _max_body_size;
        size_t      _chunk_size;
        std::string _chunk_data;
        std::string _body_data;

        size_t _http_major;
        size_t _http_minor;

        bool _has_body;
        bool _has_chunked_body;
        bool _ignore_content_length;

        std::string::const_iterator _position;
        std::string::const_iterator _last;

        std::string::const_iterator _method_begin;
        std::string::const_iterator _uri_begin;
        std::string::const_iterator _protocol_begin;
        std::string::const_iterator _header_key_begin;
        std::string::const_iterator _header_value_begin;

    public:
        RequestTools(void);
        RequestTools(std::string &raw, Server *server);
        ~RequestTools(void);
        RequestTools(RequestTools const &src);
        RequestTools &operator=(RequestTools const &src);

        void parseRequest(void);

        class RequestParsingException : public std::exception {
            public:
                RequestParsingException(RequestParsingErrorCode error);
                char const             *what() const throw();
                RequestParsingErrorCode get_error() const;

            private:
                RequestParsingErrorCode _error;
        };

    private:
        void _parseRequestLine(void);
        void _parseHeaderLines(void);
        void _parseChunkedBody(void);
        void _parseRegularBody(void);

        bool   _isControlCharacter(int c);
        bool   _isValidMethod(std::string &method);
        bool   _isValidProtocol(std::string &protocol);
        size_t _getHexValue(char c);
        void   _headerFieldNormalizedInsert(std::string &key, std::string &value);
};

#endif /* REQUEST_TOOLS_HPP */
