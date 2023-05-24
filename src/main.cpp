/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 23:32:25 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/11 23:32:25 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServer.hpp"
#include "libs.hpp"

WebServer webserver;

void interrupt(int sig)
{
    std::cout << std::endl << "Signal " << sig << " called." << std::endl;
    webserver.stop();
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        log.error("webserv requires a configuration file");
        return (1);
    } else if (argc == 2) {
        struct sigaction interruptHandler;
        interruptHandler.sa_handler = interrupt;
        sigemptyset(&interruptHandler.sa_mask);
        interruptHandler.sa_flags = 0;
        sigaction(SIGINT, &interruptHandler, 0);

        webserver.run(argv[1]);
    } else {
        log.error("too many arguments");
        return (1);
    }
    return (0);
}
