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

class WebServer {
    private:


	public:
		WebServer(void);
        ~WebServer(void);
		WebServer(WebServer const &other);
		WebServer &operator=(WebServer const &other);

		void	run(void); // Temporary configuration because I don't have the parser yet.
};

std::ostream &operator<<(std::ostream &out, WebServer const &in);


#endif