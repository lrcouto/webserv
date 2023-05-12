/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:23 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/11 21:55:08 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "libs.hpp"

class Request {
    public:
        Request(void);
        Request(std::map<std::string, std::string> headers,
                std::string                        method,
                std::string                        requestURI,
                std::string                        protocol,
                std::string                        body,
                std::string                        raw,
                std::string                        errorCode = "",
                bool                               hasError  = false);
        ~Request(void);
        Request(Request const &other);
        Request &operator=(Request const &other);

        std::map<std::string, std::string> const &getHeaders(void) const;

        std::string const &getMethod(void) const;
        std::string const &getRequestURI(void) const;
        std::string const &getProtocol(void) const;
        std::string const &getBody(void) const;
        std::string const &getRawRequest(void) const;
        std::string const &getErrorCode(void) const;

        void setError(std::string const &errorCode);

        bool hasError(void) const;

    private:
        std::map<std::string, std::string> _headers;
        std::string                        _method;
        std::string                        _requestURI;
        std::string                        _protocol;
        std::string                        _body;
        std::string                        _raw;

        bool        _hasError;
        std::string _errorCode;
};

std::ostream &operator<<(std::ostream &out, Request &in);

#endif
