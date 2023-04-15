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

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Say GO!" << std::endl; //TODO change this crap to something that makes sense.
        return 1;
    }
    std::string input = argv[1];
    if (input.compare("GO!") == 0)
    {
        WebServer webserver;

        webserver.run();
    }
    return 0;
}