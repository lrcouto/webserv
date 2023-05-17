/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 16:57:16 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/17 19:38:55 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGI_HPP
#define CGI_HPP

#include "Request.hpp"
#include "libs.hpp"

class CGI {
    private:
        Request     _request;
        std::string _binary;
        std::string _resource;
        std::string _output;
        int         _pipedes[2];
        pid_t       _pid;

    public:
        CGI(void);
        CGI(Request &request, std::string &binary, std::string &resource);
        CGI(CGI const &src);
        ~CGI(void);
        CGI &operator=(CGI const &rhs);

        void execute(void);

        std::string const &getOutput(void) const;

    private:
        void        _childRoutine(void);
        void        _parentRoutine(void);
        std::string _resolveBinaryPath(void);
};

#endif /* CGI_H */
