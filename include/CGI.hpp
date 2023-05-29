/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/17 16:57:16 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/29 20:35:25 by maolivei         ###   ########.fr       */
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
        std::string _root;
        int         _pipedes[2];
        pid_t       _pid;

    public:
        CGI(void);
        CGI(Request &request, std::string &binary, std::string &resource, std::string &root);
        CGI(CGI const &src);
        ~CGI(void);
        CGI &operator=(CGI const &rhs);

        int execute(void);

        std::string const &getOutput(void) const;
        std::string        str(void) const;

    private:
        void                     _childRoutine(void);
        int                      _parentRoutine(void);
        std::string              _resolveBinaryPath(void);
        std::vector<std::string> _formatEnvironment(void);
};

std::ostream &operator<<(std::ostream &out, CGI const &in);
std::ostream &operator<<(std::stringstream &ss, CGI const &in);

#endif /* CGI_HPP */
