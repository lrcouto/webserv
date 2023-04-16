/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 20:51:23 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/15 20:51:23 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Poll.hpp"

Poll::Poll(void) 
{
	return ;
}

Poll::~Poll(void) 
{
	return ;
}

void   Poll::insertSocket(Socket *socket)
{
    pollfd pollFdToInsert;

    this->_sockets.push_back(socket);
    pollFdToInsert.fd = socket.getFd();
    pollFdToInsert.events = POLLIN | POLLPRI | POLLOUT | POLLWRBAND;
    pollFdToInsert.revents = 0;
    this->_pollfds.push_back(pollFdToInsert);

    return;
}

void   Poll::removeSocket(Socket *socket)
{
    for (std::vector<pollfd>::reverse_iterator it = _poolfds.rbegin(); it != _poolfds.rend(); ++it) 
    {
        if (it->fd == socket->getFd()) 
        {
            _poolfds.erase(std::vector<pollfd>::iterator(&(*it)));
            break;
        }
    }

    std::vector<Socket*>::iterator socketIt = std::find(_sockets.begin(), _sockets.end(), socket);
    if (socketIt != _sockets.end()) 
    {
        delete *socketIt;
        _sockets.erase(socketIt);
    }
}

void   Poll::execute(void)
{
    int return = poll(this->_pollfds.data(), this->getSize(), 0);
    if (return == -1)
        throw(PollError());
}

size_t Poll::getSize(void) const 
{
	return (this->_poolfds.size());
}

Socket Poll::getSocket(size_t index)
{
    return (this->_sockets[index]);
}

short  Poll::getEventReturn(size_t index)
{
    return (this->_poolfds[index].revents);
}