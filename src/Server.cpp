/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 19:36:40 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/20 19:36:40 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(void) {}

Server::~Server(void) {}

void Server::insertServerData(std::pair<std::string, std::vector<std::string> > directive)
{
    if (_isUniqueDirective(directive.first)) {
        if (_isDirectiveOnMap(directive.first))
            throw(DuplicateDirectiveError());
        else
            this->_serverData.insert(directive);
    } else if (_isDirectiveNotAllowed(directive.first)) {
        throw(DirectiveNotAllowedError());
    } else {
        if (_isDirectiveOnMap(directive.first))
            this->_serverData.insert(directive);
        else
            this->_serverData[directive.first].insert(this->_serverData[directive.first].begin(),
                                                      directive.second.begin(),
                                                      directive.second.end());
    }
}

std::vector<std::string> Server::getValue(std::string key)
{
    std::vector<std::string>                                   value;
    std::map<std::string, std::vector<std::string> >::iterator it;

    it = this->_serverData.find(key);
    if (it != _serverData.end())
        value = it->second;
    return value;
}

void Server::insertLocation(Location location) { this->_locations.push_back(location); }

std::vector<Location> Server::getLocations(void) { return this->_locations; }

int     Server::getFd(void) { return this->_fd; }

void    Server::setFd(int fd) { this->_fd = fd; }

bool Server::_isUniqueDirective(std::string const &directive) const
{
    char const  *uniqueDirectives[] = {"root", "client_max_body_size", "autoindex", "listen"};
    size_t const arrayLength        = sizeof(uniqueDirectives) / sizeof(uniqueDirectives[0]);

    for (size_t i = 0; i < arrayLength; ++i)
        if (directive == uniqueDirectives[i])
            return (true);
    return (false);
}

bool Server::_isDirectiveNotAllowed(std::string const &directive) const
{
    return (directive == "limit_except");
}

bool Server::_isDirectiveOnMap(std::string const &directive) const
{
    return (this->_serverData.find(directive) != this->_serverData.end());
}
