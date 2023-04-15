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
        int serverFd = socket(AF_INET, SOCK_STREAM, 0);
        std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

        if (serverFd < 0) 
        {
            std::cerr << "Error: unable to open socket." << std::endl;
            exit(1);
        }

        struct sockaddr_in serverAddress;

        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(3007);

        memset(serverAddress.sin_zero, '\0', sizeof(serverAddress.sin_zero));

        if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
        {
            std::cerr << "Error: Unable to bind socket to server address" << std::endl;
            exit(1);
        }

        if (listen(serverFd, 10) < 0)
        {
            std::cerr << "Error: Unable to listen" << std::endl;
            exit(1);
        }
        
        int new_socket;
        int addrLen = sizeof(serverAddress);

        while(1)
        {
            std::cerr << "Waiting for connection..." << std::endl;
            if ((new_socket = accept(serverFd, (struct sockaddr *)&serverAddress, (socklen_t*)&addrLen)) < 0)
            {
                std::cerr << "Error: Unable to accept" << " " << errno << " " << strerror(errno) << std::endl;
                exit(1);
            }

            char buffer[30000] = {0};
            read(new_socket, buffer, 30000);
            std::cout << buffer << std::endl;
            write(new_socket, hello.c_str(), hello.length());
            std::cout << "Hello message sent" << std::endl;
            close(new_socket);
        }
    }
    return 0;
}