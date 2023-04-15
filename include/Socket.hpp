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

# include "libs.hpp"

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

		int         getFd(void) const;
        void        setFd(int fd);

        void        bind(void);
        void        listen(int backlog);
        void        accept(int serverFd);
        int         send(const std::string response);
        int         receive(std::string &request);
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

        class ListenSocketError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31Error: unable to listen.\e[0m");
				}
		};

        class AcceptSocketError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31Error: unable to accept.\e[0m");
				}
		};
};

std::ostream &operator<<(std::ostream &out, Socket const &in);

#endif