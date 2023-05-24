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

Logger    log;
WebServer webserver;

void interrupt(int sig)
{
    log.warning() << "Received signal " << (sig + 128) << log.end();
    webserver.stop();
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        log.error() << "WebServ requires a configuration file" << log.end();
        return (1);
    } else if (argc == 2) {
        struct sigaction interruptHandler;
        interruptHandler.sa_handler = interrupt;
        sigemptyset(&interruptHandler.sa_mask);
        interruptHandler.sa_flags = 0;
        sigaction(SIGINT, &interruptHandler, 0);

        webserver.run(argv[1]);
    } else {
        log.error() << "WebServ received too many arguments" << log.end();
        return (1);
    }
    return (0);
}
