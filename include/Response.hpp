/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/27 23:27:45 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/28 02:06:04 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef RESPONSE_HPP
# define RESPONSE_HPP

# include "libs.hpp"
# include "Request.hpp"

class Response {
    private:
        std::map<std::string, std::string>   _statusCodes;
        std::map<std::string, std::string>   _contentTypes;
        void                                 _initStatusCodes(void);
        void                                 _initContentTypes(void);

        Request                              _request;
        std::string                          _responseString;

        std::string                          _statusLine;
        std::map<std::string, std::string>   _headers;
        std::string                          _body;

	public:
		Response(void);
        Response(Request request);
        ~Response(void);
		Response(Response const &other);
		Response &operator=(Response const &other);

        std::string getResponseString(void);
        void        setRequest(Request request);

        void        assembleResponseString(void);

        void        clear(void);
};

#endif