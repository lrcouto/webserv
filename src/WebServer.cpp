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

WebServer::WebServer(void)
{
    std::cout << "Hello, this is your WebServer ready to go!" << std::endl;
}

WebServer::~WebServer(void) { std::cout << "Bye bye!" << std::endl; }

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
    }
}

int WebServer::sockaccept(Socket *listener)
{
    Socket *client;

    std::cout << "Accepting connection on fd " << listener->getFd() << std::endl;
    client = new Socket;
    client->accept(listener->getFd());
    if (client->getFd() < 0) {
        std::cerr << "\e[0;31mError: unable to accept connection\e[0m" << std::endl;
        return (-1);
    }
    this->_poll.insertSocket(client);
    return (0);
}

int WebServer::sockreceive(Socket *client)
{
    _rawRequest.clear();
    std::cout << "Receiving request on client fd " << client->getFd() << std::endl;
    if (client->receive(_rawRequest) < 0) {
        std::cerr << "\e[0;31mError: unable to receive request on fd " << client->getFd() << "\e[0m"
                  << std::endl;
        this->_poll.removeSocket(client);
        return (-1);
    }
    return (0);
}

int WebServer::socksend(Socket *client)
{
    Request request(_rawRequest);
    request.parse();
    std::cout << request << std::endl; // Debugging purposes

    Response response(request);
    response.setServerData(getCurrentServer(client->getServerFd()));
    response.assembleResponseString();
    if (client->send(response.getResponseString()) <= 0) {
        // will have to implement parsing the request and building the appropriate response.
        std::cerr << "\e[0;31mError: unable to receive request data on fd" << client->getFd()
                  << "\e[0m" << std::endl;
        return (-1);
    }
    return (0);
}

Server  *WebServer::getCurrentServer(int fd)
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
    std::cout << "Stopping Webserver..." << std::endl;
    this->_poll.clear(); // in the future will also have to clear server data, request data, etc.
}

std::ostream &operator<<(std::ostream &out, WebServer const &in)
{
    (void)in;
    return (out);
}
