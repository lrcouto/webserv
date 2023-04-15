/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 22:58:56 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/14 22:58:56 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "WebServer.hpp"

WebServer::WebServer(void) 
{
	std::cout << "Hello, this is your WebServer ready to go!\n";
	return ;
}

WebServer::~WebServer(void) 
{
	std::cout << "Bye bye!";
	return ;
}

WebServer::WebServer(WebServer const &other) 
{
	std::cout << "WebServer copy constructor called\n";
	*this = other;
	return ;
}

WebServer &WebServer::operator=(WebServer const &other)
{   	
    if (this != &other) {
		std::cout << "WebServer copy assignment operator called\n";
	}
	return (*this);
}

void WebServer::run(void)
{
    Socket serverSocket(3007);
    Socket clientSocket;
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    serverSocket.bind();
    serverSocket.listen(10);

    while(true)
    {
        std::cerr << "Waiting for connection..." << std::endl;
        clientSocket.accept(serverSocket.getFd());

        char buffer[30000] = {0};
        read(clientSocket.getFd(), buffer, 30000);
        std::cout << buffer << std::endl;
        write(clientSocket.getFd(), hello.c_str(), hello.length());
        std::cout << "Hello message sent" << std::endl;
        clientSocket.close();
    }
    serverSocket.close();
}

std::ostream &operator<<(std::ostream &out, WebServer const &in) 
{
	(void)in;
	return (out);
}