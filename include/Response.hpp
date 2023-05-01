/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:45 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/01 17:58:17 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Location.hpp"
#include "Request.hpp"
#include "ResponseTools.hpp"
#include "Server.hpp"
#include "libs.hpp"

class Response {
    private:
        static std::map<std::string, std::string> _statusCodes;
        static std::map<std::string, std::string> _contentTypes;
        static std::map<std::string, std::string> _initStatusCodes(void);
        static std::map<std::string, std::string> _initContentTypes(void);

        Request     _request;
        Server     *_serverData;
        std::string _status;
        std::string _type;
        std::string _responseString;

        std::string                        _statusLine;
        std::map<std::string, std::string> _headers;
        std::string                        _body;

    public:
        Response(void);
        Response(Request request);
        ~Response(void);
        Response(Response const &other);
        Response &operator=(Response const &other);

        std::string getResponseString(void);
        void        setServerData(Server *serverData);
        void        setRequest(Request request);

        void assembleResponseString(void);
        void assembleStatusLine(void);
        void assembleHeaders(void);
        void assembleBody(void);

        std::string assemblePath(std::string root, std::string requestURI);

        void clear(void);
};

#endif
