/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 21:53:47 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/21 21:53:47 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(void) {}

Location::~Location(void) {}

void Location::insertLocationData(std::pair<std::string, std::vector<std::string> > directive)
{
    if (_isUniqueDirective(directive.first)) {
        if (_isDirectiveOnMap(directive.first))
            throw DuplicateDirectiveError(directive.first);
        else
            this->_locationData.insert(directive);
    } else if (_isDirectiveNotAllowed(directive.first)) {
        throw DirectiveNotAllowedError(directive.first);
    } else {
        if (!_isDirectiveOnMap(directive.first))
            this->_locationData.insert(directive);
        else
            this->_locationData[directive.first].insert(
                this->_locationData[directive.first].begin(),
                directive.second.begin(),
                directive.second.end());
    }
}

std::vector<std::string> Location::getValue(std::string key)
{
    std::vector<std::string> value;

    std::map<std::string, std::vector<std::string> >::iterator it;
    it = this->_locationData.find(key);
    if (it != _locationData.end())
        value = it->second;
    return (value);
}

void Location::setPath(std::string path) { this->_path = path; }

std::string Location::getPath(void) { return (this->_path); }

bool Location::_isUniqueDirective(std::string const &directive) const
{
    char const  *uniqueDirectives[] = {"root", "autoindex"};
    size_t const arrayLength        = sizeof(uniqueDirectives) / sizeof(uniqueDirectives[0]);

    for (size_t i = 0; i < arrayLength; ++i)
        if (directive == uniqueDirectives[i])
            return (true);
    return (false);
}

bool Location::_isDirectiveNotAllowed(std::string const &directive) const
{
    char const  *notAllowedDirectives[] = {"listen", "server_name", "cgi", "redirect"};
    size_t const arrayLength = sizeof(notAllowedDirectives) / sizeof(notAllowedDirectives[0]);

    for (size_t i = 0; i < arrayLength; ++i)
        if (directive == notAllowedDirectives[i])
            return (true);
    return (false);
}

bool Location::_isDirectiveOnMap(std::string const &directive) const
{
    return (this->_locationData.find(directive) != this->_locationData.end());
}

Location::DuplicateDirectiveError::DuplicateDirectiveError(std::string const &directive)
{
    _message = ERR_PARSE "directive \"" + directive + "\" must be unique";
}

char const *Location::DuplicateDirectiveError::what(void) const throw()
{
    return (_message.c_str());
}

Location::DirectiveNotAllowedError::DirectiveNotAllowedError(std::string const &directive)
{
    _message = ERR_PARSE "directive \"" + directive + "\" not allowed inside location block";
}

char const *Location::DirectiveNotAllowedError::what(void) const throw()
{
    return (_message.c_str());
}
