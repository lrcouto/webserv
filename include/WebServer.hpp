/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:45:35 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/13 17:45:35 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Location.hpp"
#include "ParseConfig.hpp"
#include "Poll.hpp"
#include "Request.hpp"
#include "RequestTools.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "libs.hpp"

class WebServer {
    private:
        Poll                _poll;
        ParseConfig         _parseConfig;
        std::vector<Server> _servers;

        std::string _rawRequest;

    public:
        WebServer(void);
        ~WebServer(void);

        int  init(std::string const &inputFilePath);
        int  run(char const *inputFilePath);
        void stop(void);
        void setServerSocketFds(void);

        Server *getCurrentServer(int fd);

        int sockaccept(Socket *listener);
        int sockreceive(Socket *client);
        int socksend(Socket *client);
};

#endif
