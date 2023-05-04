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
#define SOCKET_HPP

#include "libs.hpp"

class Socket {
    private:
        std::string _port;
        std::string _ip;
        int         _fd;
        int         _serverFd;

    public:
        Socket(std::string port = "8080", std::string ip = "127.0.0.1");
        ~Socket(void);
        Socket(Socket const &other);
        Socket &operator=(Socket const &other);

        void        socket(void);
        void        bind(int optval);
        void        listen(int backlog);
        void        connect(int backlog);
        void        accept(int serverFd);
        int         send(std::string const response);
        int         receive(std::string &request);
        void        close();

        int         getFd(void) const;
        void        setFd(int fd);
        std::string getPort(void) const;
        int         getServerFd(void) const;

        class CreateSocketError : public std::exception {
            public:
                virtual char const *what() const throw()
                {
                    return ("\e[0;31mError: unable to create socket.\e[0m");
                }
        };

        class BindSocketError : public std::exception {
            public:
                virtual char const *what() const throw()
                {
                    return ("\e[0;31mError: unable to bind socket.\e[0m");
                }
        };

        class ListenSocketError : public std::exception {
            public:
                virtual char const *what() const throw()
                {
                    return ("\e[0;31mError: unable to listen.\e[0m");
                }
        };

        class AcceptSocketError : public std::exception {
            public:
                virtual char const *what() const throw()
                {
                    return ("\e[0;31mError: unable to accept.\e[0m");
                }
        };
};

std::ostream &operator<<(std::ostream &out, Socket const &in);

#endif
