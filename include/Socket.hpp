/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 22:16:50 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/14 22:16:50 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP

class Socket {
    private:
        int     _fd;
        int     _port;
        
	public:
		Socket(void);
        Socket(int port);
        ~Socket(void);
		Socket(Socket const &other);
		Socket &operator=(Socket const &other);

		const int   getFd(void) const;

        void        bind();
        void        listen();
        void        accept();
        void        send();
        void        receive();
        void        close();

        class CreateSocketError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31mError: unable to create socket.\e[0m");
				}
		};

		class BindSocketError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31Error: unable to bind socket.\e[0m");
				}
		};
};

std::ostream &operator<<(std::ostream &out, Socket const &in);

#endif