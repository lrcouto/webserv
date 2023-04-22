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

# include "Server.hpp"

Server::Server(void)
{
	return ;
}

Server::~Server(void) 
{
	return ;
}

void    Server::insertServerData(std::pair<std::string, std::vector<std::string> > directive)
{
    bool    isUniqueDirective = (directive.first == "root" || directive.first == "client_max_body_size" || directive.first == "autoindex" || directive.first == "listen");
    bool    directiveNotOnMap = (this->_serverData.find(directive.first) != this->_serverData.end());

    if (isUniqueDirective)
        if (directiveNotOnMap)
            throw (DuplicateDirectiveError());
        else
            this->_serverData.insert(directive);
    else
        if (directiveNotOnMap)
            this->_serverData.insert(directive);
        else
            this->_serverData[directive.first].insert(this->_serverData[directive.first].begin(), directive.second.begin(), directive.second.end());
}

std::vector<std::string>    Server::getValue(std::string key)
{
    std::vector<std::string> value;

    std::map<std::string, std::vector<std::string> >::iterator it = this->_serverData.find(key);
    if (it != _serverData.end())
        value = it->second;
    
    return value;
}

void    Server::insertLocation(Location location)
{
    this->_locations.push_back(location);
}

std::vector<Location>    Server::getLocations(void)
{
    return this->_locations;
}