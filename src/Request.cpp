/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:28 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/23 22:04:12 by lcouto           ###   ########.fr       */
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
		std::cout << "Request copy assignment operator called" << std::endl;
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &out, Request const &in) 
{
	(void)in;
	return (out);
}