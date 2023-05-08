/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:45 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/06 17:29:29 by lcouto           ###   ########.fr       */
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
        std::map<std::string, std::string>   _statusCodes;
        std::map<std::string, std::string>   _contentTypes;

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

        void getResource(std::string requestURI);
        void postResource(std::string requestURI);
        void deleteResource(std::string requestURI);

        std::string assemblePath(std::string root, std::string requestURI);
        std::string findResourceByIndex(std::vector<std::string> indexes, std::string resourcePath);

        void clear(void);
};

#endif
