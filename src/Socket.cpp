/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 00:12:36 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/15 00:12:36 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(std::string port, std::string ip) : _port(port), _ip(ip), _fd(-1), _serverFd(-1) {}

Socket::Socket(Socket const &other) { *this = other; }

Socket::~Socket(void) { this->close(); }

Socket &Socket::operator=(Socket const &other)
{
    if (this != &other)
        _fd = other._fd;
    return *this;
}

void Socket::socket(void)
{
    if ((_fd = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw CreateSocketError();
}

void Socket::bind(int optval)
{
    struct addrinfo  info = {};
    struct addrinfo *response;

    info.ai_family   = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(_ip.c_str(), _port.c_str(), &info, &response) != 0)
        throw std::exception(); // TODO: Create custom exception
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0)
        throw std::exception(); // TODO: Create custom exception
    if (::bind(_fd, response->ai_addr, response->ai_addrlen) < 0)
        throw BindSocketError();
    freeaddrinfo(response);
}

void Socket::listen(int backlog)
{
    if (::listen(_fd, backlog) < 0)
        throw BindSocketError();
}

void Socket::connect(int backlog)
{
    socket();
    bind(1);
    listen(backlog);
}

void Socket::accept(int serverFd)
{
    int                newFd;
    struct sockaddr_in clientAddr    = {};
    socklen_t          clientAddrLen = sizeof(clientAddr);

    newFd = ::accept(serverFd, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (newFd < 0)
        throw AcceptSocketError();
    _serverFd = serverFd;
    _fd = newFd;
}

int Socket::send(std::string const response)
{
    int bytesReturned, bytesTotal = 0;

    while ((size_t)bytesTotal < response.size()) {
        bytesReturned = ::send(_fd, response.c_str(), response.size(), 0);
        if (bytesReturned <= 0)
            break;
        bytesTotal += bytesReturned;
    }
    return bytesReturned;
}

int Socket::receive(std::string &request)
{
    int const BUFFER_SIZE = 30000;
    char      buffer[BUFFER_SIZE];
    int       bytesRead, totalBytes = 0;

    while ((bytesRead = recv(_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        request.append(buffer, bytesRead);
        if (request.find("Expect: 100-continue") != std::string::npos) {
            sleep(2);
            continue;
        }
        totalBytes += bytesRead;
        if (request.find("\r\n\r\n") != std::string::npos)
            break;
    }
    return (bytesRead == -1) ? -1 : totalBytes;
}

void Socket::close(void)
{
    if (_fd != -1) {
        ::close(_fd);
        _fd = -1;
    }
}

int Socket::getFd(void) const { return this->_fd; }

void Socket::setFd(int fd) { this->_fd = fd; }

std::string Socket::getPort(void) const { return this->_port; }

int Socket::getServerFd(void) const { return this->_serverFd; }
