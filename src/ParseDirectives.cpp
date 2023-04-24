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

# include "ParseDirectives.hpp"

ParseDirectives::DirectiveType ParseDirectives::parseAutoindex(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "autoindex";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseCgi(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "cgi";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseClientMaxBodySize(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "client_max_body_size";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseErrorPage(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "error_page";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseIndex(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "index";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseLimitExcept(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "limit_except";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseListen(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "listen";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseRedirect(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "redirect";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseRoot(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "root";
    data.second.push_back(line);
    return data;
}

ParseDirectives::DirectiveType ParseDirectives::parseServerName(const std::string &line)
{
    ParseDirectives::DirectiveType data;
    data.first = "server_name";
    data.second.push_back(line);
    return data;
}
