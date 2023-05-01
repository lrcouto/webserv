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

#include "Poll.hpp"

Poll::Poll(void) { return; }

Poll::~Poll(void) { return; }

void Poll::insertSocket(Socket *socket)
{
    pollfd pollFdToInsert;

    this->_sockets.push_back(socket);
    pollFdToInsert.fd      = socket->getFd();
    pollFdToInsert.events  = POLLIN | POLLPRI | POLLOUT | POLLWRBAND;
    pollFdToInsert.revents = 0;
    this->_pollfds.push_back(pollFdToInsert);

    return;
}

void Poll::removeSocket(Socket *socket)
{
    std::vector<pollfd>::reverse_iterator itpoll;
    for (itpoll = _pollfds.rbegin(); itpoll != _pollfds.rend(); ++itpoll) {
        if (itpoll->fd == socket->getFd()) {
            ::close(itpoll->fd);
            _pollfds.erase(std::vector<pollfd>::iterator(&(*itpoll)));
            break;
        }
    }

    std::vector<Socket *>::iterator itsock = std::find(_sockets.begin(), _sockets.end(), socket);
    if (itsock != _sockets.end()) {
        delete *itsock;
        _sockets.erase(itsock);
    }
}

void Poll::execute(void)
{
    int retValue = poll(this->_pollfds.data(), this->getSize(), 0);
    if (retValue == -1)
        throw(PollError());
    return;
}

bool Poll::verifyEventReturn(size_t index)
{
    short revents = this->_pollfds[index].revents;

    if ((revents & POLLIN) == POLLIN)
        return (true);
    if ((revents & POLLPRI) == POLLPRI)
        return (true);
    if ((revents & POLLOUT) == POLLOUT)
        return (true);
    if ((revents & POLLWRBAND) == POLLWRBAND)
        return (true);
    return (false);
}

size_t Poll::getSize(void) const { return (this->_pollfds.size()); }

Socket *Poll::getSocket(size_t index) { return (this->_sockets[index]); }

short Poll::getEventReturn(size_t index) { return (this->_pollfds[index].revents); }

void Poll::clear(void)
{
    std::vector<pollfd>::reverse_iterator itpoll;
    for (itpoll = _pollfds.rbegin(); itpoll != _pollfds.rend(); ++itpoll) {
        std::cout << "Closing fd " << itpoll->fd << std::endl;
        ::close(itpoll->fd);
    }
    _pollfds.clear();

    std::vector<Socket *>::reverse_iterator itsock;
    for (itsock = _sockets.rbegin(); itsock != _sockets.rend(); ++itsock)
        delete *itsock;
    _sockets.clear();
}
