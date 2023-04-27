/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:23 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/27 01:41:29 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP

# include "libs.hpp"

class Request {
    private:
    std::string                         _raw;
    std::string                         _method;
    std::string                         _requestURI;
    std::string                         _protocol;
    std::map<std::string, std::string>  _headers;
    std::string                         _body;

	public:
		Request(void);
        Request(std::string raw);
        ~Request(void);
		Request(Request const &other);
		Request &operator=(Request const &other);

        void                                setRawRequest(std::string raw);
        std::string                         getRawRequest(void);
        std::string                         getMethod(void);
        std::string                         getRequestURI(void);
        std::string                         getProtocol(void);
        std::map<std::string, std::string>  getHeaders(void);
        std::string                         getBody(void);

        void	parse(void);
        void	parseRequestLine(std::stringstream &rawStream);
        void	parseHeaders(std::stringstream &rawStream);
        void	parseBody(std::stringstream &rawStream);

        void    clear(void);

        class EmptyRequestError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31mError: cannot parse empty request\e[0m");
				}
		};
};

std::ostream &operator<<(std::ostream &out, Request &in);


#endif