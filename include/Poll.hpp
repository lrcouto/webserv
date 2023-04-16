/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Poll.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 21:27:33 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/14 21:27:33 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef POLL_HPP
# define POLL_HPP

# include "libs.hpp"
# include "Socket.hpp"

class Poll {
    private:
        std::vector<Socket *>	_sockets;
		std::vector<pollfd>		_pollfds;

	public:
		Poll(void);
        ~Poll(void);

        void   insertSocket(Socket *socket);
        void   removeSocket(Socket *socket);
        void   execute(void);
        size_t getSize(void) const;
        Socket getSocket(size_t index);
        short  getEventReturn(size_t index);

	    class PollError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31Error: unable to poll\e[0m");
				}
		};
};


#endif