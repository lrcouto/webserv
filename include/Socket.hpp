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

#include "ParametricException.hpp"
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

        void socket(void);
        void bind(int optval);
        void listen(int backlog);
        void connect(int backlog);
        void accept(int serverFd);
        int  send(std::string const response);
        int  receive(std::string &request);
        void close(void);

        int         getFd(void) const;
        std::string getPort(void) const;
        int         getServerFd(void) const;

        void setFd(int fd);

        class SocketException : public ParametricException {
            public:
                SocketException(std::string const &err);
                char const *what(void) const throw();
        };
};

#endif
