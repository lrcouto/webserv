/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/10 13:35:03 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_TOOLS_HPP
#define REQUEST_TOOLS_HPP

#include "Request.hpp"
#include "libs.hpp"

enum RequestParsingErrorCode {
    BAD_REQUEST     = 400,
    NOT_IMPLEMENTED = 501,
};

class RequestTools {
    private:
        Request     _request;
        std::string _raw;

        std::string _header_key;
        std::string _header_value;

        bool        _done_parsing;
        size_t      _chunk_size;
        std::string _chunk_data;

        std::string::const_iterator _position;
        std::string::const_iterator _last;

        std::string::const_iterator _method_begin;
        std::string::const_iterator _uri_begin;
        std::string::const_iterator _protocol_begin;
        std::string::const_iterator _header_key_begin;
        std::string::const_iterator _header_value_begin;

    public:
        RequestTools(void);
        RequestTools(std::string &raw);
        ~RequestTools(void);
        RequestTools(RequestTools const &src);
        RequestTools &operator=(RequestTools const &src);

        void parseRequestLine(void);
        void parseHeaderLines(void);
        void parseChunkedBody(void);
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
        bool   _isControlCharacter(int c);
        bool   _isValidMethod(std::string &method);
        bool   _isValidProtocol(std::string &protocol);
        size_t _getHexValue(char c);
};

#endif /* REQUEST_TOOLS_HPP */
