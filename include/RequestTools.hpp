/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestTools.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/01 01:31:07 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/04 18:13:10 by maolivei         ###   ########.fr       */
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
    WSV_REQUEST_CR,
    WSV_REQUEST_LF,
    WSV_REQUEST_DONE,
};

enum HeaderLineState {
    WSV_HEADER_START,
    WSV_HEADER_KEY,
    WSV_HEADER_SPACE,
    WSV_HEADER_VALUE,
    WSV_HEADER_CR,
    WSV_HEADER_LF,
    WSV_HEADER_DONE,
};

enum ParseExitCode { WSV_OK, WSV_ERROR };

class RequestTools {
    private:
        Request     _request;
        std::string _raw;

        std::string::iterator _method_begin;
        std::string::iterator _uri_begin;
        std::string::iterator _protocol_begin;

    public:
        RequestTools(void);
        RequestTools(std::string &raw);
        ~RequestTools(void);
        RequestTools(RequestTools const &src);
        RequestTools &operator=(RequestTools const &src);

        void parseHeaders(std::string &rawRequest);

    private:
        bool _isControlCharacter(int c);
        bool _isValidMethod(std::string &method);
        bool _isValidProtocol(std::string &protocol);
};

#endif /* REQUEST_TOOLS_HPP */
