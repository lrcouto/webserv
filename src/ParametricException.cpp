/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParametricException.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 20:51:21 by maolivei          #+#    #+#             */
/*   Updated: 2023/04/27 20:52:08 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParametricException.hpp"

ParametricException::ParametricException(void) : _message("") {}

ParametricException::ParametricException(std::string const &message) : _message(message) {}

ParametricException::ParametricException(ParametricException const &src) { *this = src; }

ParametricException::~ParametricException(void) throw() {}

ParametricException &ParametricException::operator=(ParametricException const &src)
{
    if (this == &src)
        return (*this);
    _message = src._message;
    return (*this);
}

char const *ParametricException::what(void) const throw() { return (_message.c_str()); }
