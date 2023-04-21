/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseDirectives.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 21:25:01 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/19 21:25:01 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseDirectives.hpp"

ParseDirectives::DirectiveType ParseDirectives::parseAutoindex(std::string const &line)
{
    std::vector<std::string> tokens, directiveValues;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() != 2)
        throw std::exception(); // TODO: Create custom exception
    if (tokens[1] != "on" && tokens[1] != "off")
        throw std::exception(); // TODO: Create custom exception
    directiveValues.push_back(tokens[1]);
    return (std::make_pair(tokens[0], directiveValues));
}

ParseDirectives::DirectiveType ParseDirectives::parseCgi(std::string const &line)
{
    return (std::make_pair(line, std::vector<std::string>()));
}

ParseDirectives::DirectiveType ParseDirectives::parseClientMaxBodySize(std::string const &line)
{
    std::string const        size_type_charset = "MmGg";
    std::vector<std::string> tokens, directiveValues;
    bool                     has_size_type;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() != 2)
        throw std::exception(); // TODO: Create custom exception
    has_size_type = false;
    for (std::string::iterator it = tokens[1].begin(); it != tokens[1].end(); ++it) {
        if (has_size_type && it != tokens[1].end())
            throw std::exception(); // TODO: Create custom exception
        if (!std::isdigit(*it)) {
            if (size_type_charset.find(*it) != std::string::npos) {
                if (!has_size_type) {
                    has_size_type = true;
                    continue;
                }
            } else {
                throw std::exception(); // TODO: Create custom exception
            }
        }
    }
    directiveValues.push_back(tokens[1]);
    return (std::make_pair(tokens[0], directiveValues));
}

ParseDirectives::DirectiveType ParseDirectives::parseErrorPage(std::string const &line)
{
    char const *http_codes[] = {
        "300", "301", "302", "303", "304", "305", "306", "307", "308", "400", "401", "402", "403",
        "404", "405", "406", "407", "408", "409", "410", "411", "412", "413", "414", "415", "416",
        "417", "418", "421", "422", "423", "424", "425", "426", "428", "429", "431", "451", "500",
        "501", "502", "503", "504", "505", "506", "507", "508", "510", "511", NULL};
    size_t const http_code_array_size = sizeof(http_codes) / sizeof(http_codes[0]);

    std::vector<std::string> tokens, directiveValues;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() < 3)
        throw std::exception(); // TODO: Create custom exception
    for (std::vector<std::string>::iterator it = tokens.begin() + 1; it != tokens.end() - 1; ++it) {
        for (size_t index = 0; index < http_code_array_size; ++index) {
            if (index == (http_code_array_size - 1))
                throw std::exception(); // TODO: Create custom exception
            if (*it == http_codes[index]) {
                directiveValues.push_back(*it);
                break;
            }
        }
    }
    return (std::make_pair(tokens[0], directiveValues));
}

ParseDirectives::DirectiveType ParseDirectives::parseIndex(std::string const &line)
{
    return (std::make_pair(line, std::vector<std::string>()));
}

ParseDirectives::DirectiveType ParseDirectives::parseLimitExcept(std::string const &line)
{
    return (std::make_pair(line, std::vector<std::string>()));
}

ParseDirectives::DirectiveType ParseDirectives::parseListen(std::string const &line)
{
    return (std::make_pair(line, std::vector<std::string>()));
}

ParseDirectives::DirectiveType ParseDirectives::parseLocation(std::string const &line)
{
    return (std::make_pair(line, std::vector<std::string>()));
}

ParseDirectives::DirectiveType ParseDirectives::parseRedirect(std::string const &line)
{
    return (std::make_pair(line, std::vector<std::string>()));
}

ParseDirectives::DirectiveType ParseDirectives::parseRoot(std::string const &line)
{
    return (std::make_pair(line, std::vector<std::string>()));
}

ParseDirectives::DirectiveType ParseDirectives::parseServerName(std::string const &line)
{
    return (std::make_pair(line, std::vector<std::string>()));
}
