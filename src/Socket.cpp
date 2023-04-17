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

# include "Socket.hpp"

Socket::Socket(void) : _port(0) 
{
    _fd = -1;
	return ;
}

Socket::Socket(int port) : _port(port) 
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);

    if (_fd < 0)
    {
        throw (CreateSocketError());
    }
	return;
}

Socket::~Socket(void) 
{
	this->close();
	return ;
}

Socket::Socket(Socket const &other) 
{
	*this = other;
	return ;
}

int Socket::getFd(void) const
{
    return this->_fd;
}

void Socket::setFd(int fd)
{
    this->_fd = fd;
    return;
}

void    Socket::bind(int optval)
{
    struct addrinfo info, *response;
    std::stringstream ss;
    ss << _port;
    std::string port = ss.str();

    memset(&info, 0, sizeof(info));
    info.ai_family = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;
    info.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, port.c_str(), &info, &response);

    setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    
    if (::bind(_fd, response->ai_addr, response->ai_addrlen) < 0)
        throw (BindSocketError());
    
    freeaddrinfo(response);
    return;
}

void    Socket::listen(int backlog)
{
    if (::listen(_fd, backlog) < 0)
        throw (BindSocketError());
    
    return;
}

void    Socket::accept(int serverFd)
{
    int newFd;
    struct sockaddr_in clientAddr = {};
	socklen_t clientAddrLen = sizeof(clientAddr);

    if ((newFd = ::accept(serverFd, (struct sockaddr*)&clientAddr, &clientAddrLen)) < 0)
        throw (AcceptSocketError());

    this->setFd(newFd);

	return;
}

int    Socket::send(const std::string response)
{
    int bytesReturned, bytesTotal = 0;

    while((unsigned long int)bytesTotal < response.size())
    {
        bytesReturned = ::send(this->getFd(), response.c_str(), response.size(), 0);
        
        if (bytesReturned <= 0)
            break;
        bytesTotal += bytesReturned;
    }    
    return bytesReturned;
}

int    Socket::receive(std::string &request)
{
    const int BUFFER_SIZE = 30000;
    char buffer[BUFFER_SIZE];
    int  bytesRead, totalBytes = 0;

    while ((bytesRead = recv(this->getFd(), buffer, BUFFER_SIZE, 0)) > 0)
    {
        request.append(buffer, bytesRead);
        if (request.find("Expect: 100-continue") != std::string::npos)
        {
            sleep(2);
            continue;
        }
        totalBytes += bytesRead;
        if (request.find("\r\n\r\n") != std::string::npos)
            break;
    }
    return (bytesRead == -1) ? -1 : totalBytes;
}

void    Socket::close()
{
    if (_fd != -1) 
    {
		::close(_fd);
	    _fd = -1;
	}
    return;
}

Socket &Socket::operator=(Socket const &other)
{
	if (this != &other) 
		_fd = other._fd;
	return (*this);
}