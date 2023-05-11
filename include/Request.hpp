/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:23 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/11 19:12:40 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "libs.hpp"

class Request {
    private:

    public:
        std::map<std::string, std::string> _headers;
        std::string                        _method;
        std::string                        _requestURI;
        std::string                        _protocol;
        std::string                        _body;
        std::string                        _raw;

        Request(void);
        Request(std::string raw);
        ~Request(void);
        Request(Request const &other);
        Request &operator=(Request const &other);

        std::map<std::string, std::string> getHeaders(void);
        std::string                        getMethod(void);
        std::string                        getRequestURI(void);
        std::string                        getProtocol(void);
        std::string                        getBody(void);
        std::string                        getRawRequest(void);

        void setRawRequest(std::string raw);

        void parse(void);
        void parseRequestLine(std::stringstream &rawStream);
        void parseHeaders(std::stringstream &rawStream);
        void parseBody(std::stringstream &rawStream);

        void clear(void);

        class EmptyRequestError : public std::exception {
            public:
                virtual char const *what() const throw()
                {
                    return ("\e[0;31mError: cannot parse empty request\e[0m");
                }
        };
};

std::ostream &operator<<(std::ostream &out, Request &in);

#endif
