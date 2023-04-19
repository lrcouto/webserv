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

# include "libs.hpp"
# include "WebServer.hpp"

WebServer webserver;

void    interrupt(int sig)
{
    std::cout << std::endl << "Signal " << sig << " called." << std::endl;
    webserver.stop();
    exit(0);
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "\e[0;31mError: webserv requires a config file.\e[0m" << std::endl;
        return 1;
    }
    else if (argc == 2)
    {
        struct sigaction interruptHandler;
        interruptHandler.sa_handler = interrupt;
        sigemptyset(&interruptHandler.sa_mask);
        interruptHandler.sa_flags = 0;
        sigaction(SIGINT, &interruptHandler, 0);

        webserver.run(argv[1]);
    }
    else
    {
        std::cerr << "\e[0;31mError: too many arguments.\e[0m" << std::endl;
        return 1; 
    }
    return 0;
}