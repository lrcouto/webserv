/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftstring.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 18:37:02 by maolivei          #+#    #+#             */
/*   Updated: 2023/04/20 18:44:43 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftstring.hpp"

namespace ftstring {

std::string trim(std::string const &str, std::string const &set)
{
    size_t strBegin, strEnd, strRange;

    strBegin = str.find_first_not_of(set);
    if (strBegin == std::string::npos)
        return ("");
    strEnd   = str.find_last_not_of(set);
    strRange = strEnd - strBegin + 1;
    return (str.substr(strBegin, strRange));
}

std::string reduce(std::string const &str, std::string const &set, std::string const &fill)
{
    size_t      beginSet, endSpace, range, newStart;
    std::string result;

    result   = trim(str, set);
    beginSet = result.find_first_of(set);
    while (beginSet != std::string::npos) {
        endSpace = result.find_first_not_of(set, beginSet);
        range    = endSpace - beginSet;
        result.replace(beginSet, range, fill);
        newStart = beginSet + fill.length();
        beginSet = result.find_first_of(set, newStart);
    }
    return (result);
}

} /* ftstring */
