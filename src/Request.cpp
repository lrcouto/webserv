/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:28 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/24 00:36:08 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

Request::Request(void)  : _raw("")
{
	return ;
}

Request::Request(std::string raw) : _raw(raw)
{
	return ;
}

Request::~Request(void) 
{
	return ;
}

Request::Request(Request const &other) 
{
	*this = other;
	return ;
}

Request &Request::operator=(Request const &other)
{
    if (this != &other) {
        this->_raw = other._raw;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &out, Request const &in) 
{
	(void)in;
	return (out);
}