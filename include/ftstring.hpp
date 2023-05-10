/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftstring.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 18:36:10 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/10 16:42:25 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FTSTRING_HPP
#define FTSTRING_HPP

#include <sstream>
#include <string>
#include <vector>

namespace ftstring {

std::string trim(std::string const &str, std::string const &set);
std::string reduce(std::string const &str, std::string const &set, std::string const &fill = " ");
std::vector<std::string> split(std::string const &str, char delimiter);
bool                     is_positive_integer(std::string const &str);
int                      strtoi(std::string const &str);
bool                     striequals(std::string const &s1, std::string const &s2);

} /* ftstring */

#endif /* FTSTRING_HPP */
