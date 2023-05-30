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
#define POLL_HPP

#include "ParametricException.hpp"
#include "Socket.hpp"
#include "libs.hpp"


class Poll {
    private:
        std::vector<Socket *> _sockets;
        std::vector<pollfd>   _pollfds;

    public:
        Poll(void);
        ~Poll(void);

        void    insertSocket(Socket *socket);
        void    removeSocket(Socket *socket);
        void    execute(void);
        bool    verifyEventReturn(size_t index);
        size_t  getSize(void) const;
        Socket *getSocket(size_t index);
        short   getEventReturn(size_t index);
        void    clear(void);

        class PollException : public ParametricException {
            public:
                PollException(std::string const &err);
                char const *what() const throw();
        };
};

#endif
