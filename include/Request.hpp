/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:23 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/23 21:57:40 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "libs.hpp"

class Request {
    private:
    std::string     _raw;

	public:
		Request(void);
        Request(std::string raw);
        ~Request(void);
		Request(Request const &other);
		Request &operator=(Request const &other);
};

std::ostream &operator<<(std::ostream &out, Request const &in);


#endif