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

#ifndef  SERVER_HPP
# define SERVER_HPP

# include "libs.hpp"

class Server {
    private:
        std::map<std::string, std::vector<std::string> > _serverData;
        // std::vector<Location>                         _locations;
    
    public:
    	Server(void);
        ~Server(void);
		// Server(Server const &other);
		// Server &operator=(Server const &other);

        void                        insertServerData(std::pair<std::string, std::vector<std::string> > directive);
        std::vector<std::string>    getValue(std::string key);    

        class DuplicateDirectiveError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31mError: directives 'autoindex', 'root' and 'client_max_body_size' must be unique.\e[0m");
				}
		};

};

#endif
