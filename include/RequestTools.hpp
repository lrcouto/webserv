/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/15 00:39:36 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_TOOLS_HPP
#define REQUEST_TOOLS_HPP

#include "Request.hpp"
#include "Server.hpp"
#include "libs.hpp"

#define BAD_REQUEST                "400"
#define ENTITY_TOO_LARGE           "413"
#define NOT_IMPLEMENTED            "501"
#define HTTP_VERSION_NOT_SUPPORTED "505"

class RequestTools {
    private:
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
        std::string::const_iterator _query_string_begin;
        std::string::const_iterator _protocol_begin;
        std::string::const_iterator _header_key_begin;
        std::string::const_iterator _header_value_begin;

        std::map<std::string, std::string> _headers;
        std::string                        _method;
        std::string                        _uri;
        std::string                        _query_string;
        std::string                        _protocol;
        std::string                        _body;

        bool        _hasError;
        std::string _errorCode;

    public:
        RequestTools(std::string &raw, Server *server);
        ~RequestTools(void);
        RequestTools(RequestTools const &src);
        RequestTools &operator=(RequestTools const &src);

        void parseRequest(void);

        Request buildRequest(void) const;

        class RequestParsingException : public std::exception {
            public:
                RequestParsingException(char const *error);
                char const *what() const throw();
                char const *get_error(void) const;

            private:
                char const *_error;
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
