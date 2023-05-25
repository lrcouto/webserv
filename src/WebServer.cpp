/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 22:58:56 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/14 22:58:56 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"

WebServer::WebServer(void) {}

WebServer::~WebServer(void) {}

WebServer::WebServer(WebServer const &other)
{
    std::cout << "WebServer copy constructor called" << std::endl;
    *this = other;
}

WebServer &WebServer::operator=(WebServer const &other)
{
    if (this != &other) {
        std::cout << "WebServer copy assignment operator called" << std::endl;
    }
    return (*this);
}

void WebServer::init(std::string const &inputFilePath)
{
    Socket *socket;

    this->_servers = this->_parseConfig.execute(inputFilePath);
    for (size_t i = 0; i < this->_servers.size(); ++i) {
        std::vector<std::string> sockinfo = this->_servers[i].getValue("listen");

        if (sockinfo.size() == 0) {
            sockinfo.push_back("127.0.0.1");
            sockinfo.push_back("8080");
        }
        socket = new Socket(sockinfo[1], sockinfo[0]);
        socket->connect(SOMAXCONN);
        this->_poll.insertSocket(socket);
        Logger::info << "Listening at " << sockinfo[0] << ':' << sockinfo[1] << Logger::endl;
    }
}

int WebServer::sockaccept(Socket *listener)
{
    Socket *client;

    Logger::info << "Accepting connection on socket file descriptor " << listener->getFd()
                 << Logger::endl;
    client = new Socket;
    client->accept(listener->getFd());
    if (client->getFd() < 0) {
        Logger::error << "Unable to accept connection on socket file descriptor "
                      << listener->getFd() << Logger::endl;
        return (-1);
    }
    this->_poll.insertSocket(client);
    return (0);
}

int WebServer::sockreceive(Socket *client)
{
    _rawRequest.clear();
    Logger::info << "Receiving request on client file descriptor " << client->getFd()
                 << Logger::endl;
    if (client->receive(_rawRequest) < 0) {
        Logger::error << "Unable to receive request on socket file descriptor " << client->getFd()
                      << Logger::endl;
        this->_poll.removeSocket(client);
        return (-1);
    }
    return (0);
}

int WebServer::socksend(Socket *client)
{
    RequestTools parser(_rawRequest, getCurrentServer(client->getServerFd()));
    Request      request;

    parser.parseRequest();
    request = parser.buildRequest();

#ifdef DEBUG
    Logger::debug << "Received request:\n" << request << Logger::endl;
#endif /* DEBUG */

    Response response(request);
    response.setServerData(getCurrentServer(client->getServerFd()));
    if (request.hasError())
        response.HTTPError(request.getErrorCode());
    response.assembleResponseString();

#ifdef DEBUG
    Logger::debug << "Assembled response:\n" << response << Logger::endl;
#endif /* DEBUG */

    Logger::info << "Sending response to client file descriptor " << client->getFd()
                 << Logger::endl;
    if (client->send(response.getResponseString()) <= 0) {
        Logger::info << "Unable to read request data on file descriptor " << client->getFd()
                     << Logger::endl;
        return (-1);
    }
    return (0);
}

Server *WebServer::getCurrentServer(int fd)
{
    Server *currentServer;

    for (size_t i = 0; i < this->_servers.size(); i++) {
        if (this->_servers[i].getFd() == fd)
            currentServer = &this->_servers[i];
    }
    return currentServer;
}

void WebServer::setServerSocketFds(void)
{
    for (size_t i = 0; i < this->_poll.getSize(); i++) {
        Socket *socket = this->_poll.getSocket(i);
        for (size_t i = 0; i < this->_servers.size(); i++) {
            if (socket->getPort() == this->_servers[i].getValue("listen")[1])
                this->_servers[i].setFd(socket->getFd());
        }
    }
}

void WebServer::run(std::string const &inputFilePath)
{
    init(inputFilePath);
    Logger::info << "Hello, this is your WebServer ready to go!" << Logger::endl;
    while (true) {
        this->_poll.execute();
        setServerSocketFds();
        for (size_t i = 0; i < this->_poll.getSize(); ++i) {
            if (this->_poll.verifyEventReturn(i)) {
                Socket *socket = this->_poll.getSocket(i);
                if (i < this->_servers.size()) {
                    if (sockaccept(socket) != 0)
                        continue;
                } else {
                    if (sockreceive(socket) != 0)
                        continue;
                    if (!_rawRequest.empty()) {
                        if (socksend(socket) != 0)
                            ; // Handle
                    }
                    this->_poll.removeSocket(socket);
                }
            }
        }
    }
}

void WebServer::stop(void)
{
    Logger::info << "Stopping Webserver" << Logger::endl;
    this->_poll.clear(); // in the future will also have to clear server data, request data, etc.
    Logger::info << "Bye bye!" << Logger::endl;
}

std::ostream &operator<<(std::ostream &out, WebServer const &in)
{
    (void)in;
    return (out);
}
