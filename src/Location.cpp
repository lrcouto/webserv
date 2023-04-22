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

# include "Location.hpp"

Location::Location(void)
{
	return ;
}

Location::~Location(void) 
{
	return ;
}

void    Location::insertLocationData(std::pair<std::string, std::vector<std::string> > directive)
{
    bool    isUniqueDirective = (directive.first == "root" || directive.first == "autoindex");
    bool    directiveNotOnMap = (this->_locationData.find(directive.first) != this->_locationData.end());

    if (isUniqueDirective)
        if (directiveNotOnMap)
            throw (DuplicateDirectiveError());
        else
            this->_locationData.insert(directive);
    else
        if (directiveNotOnMap)
            this->_locationData.insert(directive);
        else
            this->_locationData[directive.first].insert(this->_locationData[directive.first].begin(), directive.second.begin(), directive.second.end());
}

std::vector<std::string>    Location::getValue(std::string key)
{
    std::vector<std::string> value;

    std::map<std::string, std::vector<std::string> >::iterator it = this->_locationData.find(key);
    if (it != _locationData.end())
        value = it->second;
    
    return value;
}