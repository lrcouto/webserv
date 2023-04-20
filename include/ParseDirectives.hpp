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

#ifndef  PARSEDIRECTIVES_HPP
# define PARSEDIRECTIVES_HPP

# include "libs.hpp"

class ParseDirectives {
    public:
        static void parseAutoindex(const std::string &line);
        static void parseCgi(const std::string &line);
        static void parseClientMaxBodySize(const std::string &line);
        static void parseErrorPage(const std::string &line);
        static void parseIndex(const std::string &line);
        static void parseLimitExcept(const std::string &line);
        static void parseListen(const std::string &line);
        static void parseLocation(const std::string &line);
        static void parseRedirect(const std::string &line);
        static void parseRoot(const std::string &line);
        static void parseServerName(const std::string &line);
};

#endif
