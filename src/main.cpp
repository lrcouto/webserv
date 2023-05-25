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
    std::cout << '\n';
    Logger::warning << "Received signal " << (sig + 128) << Logger::endl;
    webserver.stop();
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        if (argc < 2)
            std::cerr << "webserv: missing configuration file" << std::endl;
        if (argc > 2)
            std::cerr << "webserv: too many arguments" << std::endl;
        return (1);
    }

    struct sigaction interruptHandler;
    interruptHandler.sa_handler = interrupt;
    sigemptyset(&interruptHandler.sa_mask);
    interruptHandler.sa_flags = 0;
    sigaction(SIGINT, &interruptHandler, 0);

    webserver.run(argv[1]);

    return (0);
}
