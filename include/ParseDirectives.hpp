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

#include "ParametricException.hpp"
#include "libs.hpp"

#define ERR_PARSE "webserv: error in config file: "

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

        class InvalidNumberArguments : public ParametricException {
            public:
                InvalidNumberArguments(std::string const &field);
                char const *what(void) const throw();
        };

        class InvalidArgument : public ParametricException {
            public:
                InvalidArgument(std::string const &field, std::string const &value);
                char const *what(void) const throw();
        };

        class SystemError : public ParametricException {
            public:
                SystemError(std::string const &field, std::string const &value);
                char const *what(void) const throw();
        };

    private:
        static bool _isValidIp(std::string const &ip);
        static bool _isValidPort(std::string const &ip);
        static bool _isValidCgiExtension(std::string const &extension);
        static bool _isValidCgiExecutable(std::string const &executable);
};

#endif
