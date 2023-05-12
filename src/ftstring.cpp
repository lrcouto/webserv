/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ftstring.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 18:37:02 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/10 16:42:13 by maolivei         ###   ########.fr       */
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

std::vector<std::string> split(std::string const &str, char delimiter)
{
    std::string              tmp;
    std::stringstream        ss(str);
    std::vector<std::string> substrings;

    while (std::getline(ss, tmp, delimiter))
        substrings.push_back(tmp);
    return (substrings);
}

bool is_positive_integer(std::string const &str)
{
    return (!str.empty() && str.find_first_not_of("0123456789") == std::string::npos);
}

int strtoi(std::string const &str)
{
    size_t n;
    std::istringstream(str) >> n;
    return (n);
}

bool striequals(std::string const &s1, std::string const &s2)
{
    size_t const s1_size = s1.size();

    if (s2.size() != s1_size)
        return (false);
    for (size_t i = 0; i < s1_size; ++i)
        if (std::toupper(s1[i]) != std::toupper(s2[i]))
            return (false);
    return (true);
}

} /* ftstring */
