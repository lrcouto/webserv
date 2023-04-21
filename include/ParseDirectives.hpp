/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseDirectives.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 20:45:59 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/19 20:45:59 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSEDIRECTIVES_HPP
#define PARSEDIRECTIVES_HPP

#include "libs.hpp"

class ParseDirectives {
    public:
        typedef std::pair<std::string, std::vector<std::string> > DirectiveType;

        static DirectiveType parseAutoindex(std::string const &line);
        static DirectiveType parseCgi(std::string const &line);
        static DirectiveType parseClientMaxBodySize(std::string const &line);
        static DirectiveType parseErrorPage(std::string const &line);
        static DirectiveType parseIndex(std::string const &line);
        static DirectiveType parseLimitExcept(std::string const &line);
        static DirectiveType parseListen(std::string const &line);
        static DirectiveType parseRedirect(std::string const &line);
        static DirectiveType parseRoot(std::string const &line);
        static DirectiveType parseServerName(std::string const &line);
};

#endif
