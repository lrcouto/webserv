/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 19:36:45 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/20 19:36:45 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include "Socket.hpp"
#include "libs.hpp"

class Server {
    private:
        std::map<std::string, std::vector<std::string> > _serverData;
        std::vector<Location>                            _locations;

    public:
        Server(void);
        ~Server(void);

        void insertServerData(std::pair<std::string, std::vector<std::string> > directive);
        void insertLocation(Location location);

        std::vector<std::string> getValue(std::string key);
        std::vector<Location>    getLocations(void);

        class DuplicateDirectiveError : public std::exception {
            public:
                virtual char const *what() const throw()
                {
                    return ("\e[0;31mError: directives 'listen', 'autoindex', 'root' and "
                            "'client_max_body_size' must be unique.\e[0m");
                }
        };

        class DirectiveNotAllowedError : public std::exception {
            public:
                virtual char const *what() const throw()
                {
                    return ("\e[0;31mError: directive not allowed inside server block'.\e[0m");
                }
        };

    private:
        bool _isUniqueDirective(std::string const &directive) const;
        bool _isDirectiveNotAllowed(std::string const &directive) const;
        bool _isDirectiveOnMap(std::string const &directive) const;
};

#endif
