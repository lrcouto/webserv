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
        

	public:
		Socket(void);
        ~Socket(void);
		Socket(Socket const &other);
		Socket &operator=(Socket const &other);

		void	run(const int socket); // Temporary configuration because I don't have the parser yet.



};

std::ostream &operator<<(std::ostream &out, Socket const &in);


#endif