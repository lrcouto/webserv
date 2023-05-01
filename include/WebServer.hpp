/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:45:35 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/13 17:45:35 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "libs.hpp"
# include "Socket.hpp"
# include "Poll.hpp"
# include "ParseConfig.hpp"
# include "Server.hpp"
# include "Location.hpp"
# include "Request.hpp"
# include "Response.hpp"

class WebServer {
    private:
        Poll                    _poll;
        ParseConfig             _parseConfig;
        std::vector<Server>     _serverData;

	public:
		WebServer(void);
        ~WebServer(void);
		WebServer(WebServer const &other);
		WebServer &operator=(WebServer const &other);

		void	run(const std::string &inputFilePath);
        void    stop(void);

};

std::ostream &operator<<(std::ostream &out, WebServer const &in);


#endif