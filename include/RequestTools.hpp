/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/04 21:36:57 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_TOOLS_HPP
#define REQUEST_TOOLS_HPP

#include "Request.hpp"
#include "libs.hpp"

#define CR         '\r'
#define LF         '\n'
#define WHITESPACE ' '

enum RequestLineState {
    WSV_REQUEST_START,
    WSV_METHOD,
    WSV_URI_START,
    WSV_URI,
    WSV_HTTP_START,
    WSV_HTTP,
    WSV_REQUEST_LF,
    WSV_REQUEST_DONE,
};

enum HeaderLineState {
    WSV_HEADER_START,
    WSV_HEADER_KEY,
    WSV_HEADER_SPACE_BEFORE_VALUE,
    WSV_HEADER_VALUE,
    WSV_HEADER_LF,
    WSV_HEADER_FINAL_LF,
    WSV_HEADER_DONE,
};

enum ParseExitCode { WSV_OK, WSV_ERROR };

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

        bool _done_parsing;

    public:
        RequestTools(void);
        RequestTools(std::string &raw);
        ~RequestTools(void);
        RequestTools(RequestTools const &src);
        RequestTools &operator=(RequestTools const &src);

        void parseRequestLine(std::string &rawRequest);
        void parseHeaderLine(std::string &rawRequest);

    private:
        bool _isControlCharacter(int c);
        bool _isValidMethod(std::string &method);
        bool _isValidProtocol(std::string &protocol);
};

#endif /* REQUEST_TOOLS_HPP */
