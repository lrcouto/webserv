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
        if (!_isDirectiveOnMap(directive.first))
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

void Server::generateSessionId(void)
{
    std::time_t now = std::time(NULL);
    int randomNumber = std::rand();
    std::string serverAddress = getValue("listen")[0] + ":" + getValue("listen")[1];
    char sessionId[64];

    std::snprintf(sessionId, sizeof(sessionId), "%s-%ld-%d", serverAddress.c_str(), now, randomNumber);base64Encode(sessionId);
    this->_sessionId = base64Encode(std::string(sessionId));
}

std::string Server::getSessionId(void)
{
    return this->_sessionId;
}

std::string Server::base64Encode(const std::string &unencodedString)
{
    const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    std::string encodedString;
    int i = 0;
    int j = 0;
    unsigned char arrayOf3[3];
    unsigned char arrayOf4[4];
    const char* bytes_to_encode = unencodedString.c_str();
    int len = unencodedString.size();

    while (len--)
    {
        arrayOf3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
            arrayOf4[0] = (arrayOf3[0] & 0xfc) >> 2;
            arrayOf4[1] = ((arrayOf3[0] & 0x03) << 4) + ((arrayOf3[1] & 0xf0) >> 4);
            arrayOf4[2] = ((arrayOf3[1] & 0x0f) << 2) + ((arrayOf3[2] & 0xc0) >> 6);
            arrayOf4[3] = arrayOf3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                encodedString += base64_chars[arrayOf4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            arrayOf3[j] = '\0';

        arrayOf4[0] = (arrayOf3[0] & 0xfc) >> 2;
        arrayOf4[1] = ((arrayOf3[0] & 0x03) << 4) + ((arrayOf3[1] & 0xf0) >> 4);
        arrayOf4[2] = ((arrayOf3[1] & 0x0f) << 2) + ((arrayOf3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            encodedString += base64_chars[arrayOf4[j]];

        while ((i++ < 3))
            encodedString += '=';
    }

    return encodedString;
}
