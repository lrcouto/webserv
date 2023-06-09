/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:45 by lcouto            #+#    #+#             */
/*   Updated: 2023/05/30 16:34:17 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "CGI.hpp"
#include "Location.hpp"
#include "Request.hpp"
#include "ResponseTools.hpp"
#include "Server.hpp"
#include "libs.hpp"

class Response {
    private:
        static std::map<std::string, std::string> _statusCodes;
        static std::map<std::string, std::string> _contentTypes;

        Request     _request;
        Server     *_serverData;
        std::string _status;
        std::string _type;
        std::string _responseString;
        std::string _root;
        bool        _redirected;

        std::string                             _statusLine;
        std::multimap<std::string, std::string> _headers;
        std::string                             _body;

    public:
        Response(void);
        Response(Request request);
        ~Response(void);
        Response(Response const &other);
        Response &operator=(Response const &other);

        std::string getResponseString(void);
        std::string getRoot(void);
        void        setServerData(Server *serverData);
        void        setRequest(Request request);

        void assembleResponseString(void);
        void assembleStatusLine(void);
        void assembleHeaders(void);
        void assembleBody(void);

        void getResource(std::string requestURI);
        void postResource(std::string requestURI);
        void deleteResource(std::string requestURI);
        void HTTPError(std::string status);
        void setErrorPage(std::string status, std::string path);
        void validateServerName(void);
        bool sessionHandler(std::string resource);
        void handleCGI(std::string &binaryPath, std::string &resource);

        std::vector<std::string> verifyLocationAutoindexOverride(std::string resourcePath);

        std::string assemblePath(std::string root, std::string requestURI);
        std::string findResourceByIndex(std::vector<std::string> indexes, std::string resourcePath);
        std::string assembleCookie(std::pair<std::string, std::string> dataEntry);

        void        clear(void);
        std::string str(void) const;
};

std::ostream &operator<<(std::ostream &out, Response const &in);
std::ostream &operator<<(std::stringstream &ss, Response const &in);

#endif
