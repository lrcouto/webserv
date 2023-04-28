/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParametricException.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 20:43:36 by maolivei          #+#    #+#             */
/*   Updated: 2023/04/27 20:56:07 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARAMETRIC_EXCEPTION_HPP
#define PARAMETRIC_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class ParametricException : public std::exception {
    public:
        ParametricException(void);
        ParametricException(ParametricException const &src);
        ParametricException(std::string const &message);
        virtual ~ParametricException(void) throw();
        ParametricException &operator=(ParametricException const &src);

        char const *what(void) const throw();

    protected:
        std::string _message;
};

#endif /* PARAMETRIC_EXCEPTION_HPP */
