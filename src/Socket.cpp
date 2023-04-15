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
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw (CreateSocketError());
	return ;
}

Socket::Socket(int port) : _port(port) 
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd < 0)
        throw (CreateSocketError());
	return ;
}

Socket::~Socket(void) 
{
	close(_fd);
	return ;
}

Socket::Socket(Socket const &other) 
{
	*this = other;
	return ;
}

void    Socket::bind()
{
    struct addrinfo info, *response;

    memset(&info, 0, sizeof(info));
    info.ai_family = AF_UNSPEC;
    info.ai_socktype = SOCK_STREAM;
    info.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, _port, &info, &response)
    
    if (bind(_fd, response->ai_family, response->ai_addrlen) < 0)
        throw (BindSocketError());
    
    return;
}

void    Socket::listen();

void    Socket::accept();

void    Socket::send();

void    Socket::receive();

void    Socket::close();

Socket &Socket::operator=(Socket const &other)
{
	if (this != &other) 
		_fd = other._fd;
	return (*this);
}