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
    Socket *listener;
    Socket *client;
    int ports[3] = {3007, 3008, 3009};
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world! "; // hardcoded test message.
    std::string request;

    for(int i = 0; i < 3; i++) // hardcoded number of sockets because I have no input file yet.
    {
        listener = new Socket(ports[i]); // sockets allocated with new will have to be explicitly deleted.
        listener->bind();
        listener->listen(SOMAXCONN); // add some sort of error handling to this, do not add socket to poll() if binding/listening fails.
        this->_poll.insertSocket(listener);
    }

    while(true)
    {
        this->_poll.execute();
        for(size_t i = 0; i < this->_poll.getSize(); i++)
        {
            if (this->_poll.verifyEvenReturn(this->_poll.getEventReturn(i)))
            {
                if (i < 3)
                {
                    listener = this->_poll.getSocket(i);
                    std::cout << "Accepting connection to client on fd " << listener->getFd() << std::endl;
                    client = new Socket; // sockets allocated with new will have to be explicitly deleted.
                    client->accept(listener->getFd());
                    if (client->getFd() == -1)
                    {
                        std::cerr << "\e[0;31mError: unable to accept connection.\e[0m" << std::endl;
                        continue;
                    }
                    this->_poll.insertSocket(client);
                }
                else
                {
                    client = this->_poll.getSocket(i);
                    request.clear();
                    std::cout << "Receiving request through client fd " << client->getFd() << std::endl;
                    if (client->receive(request) < 0)
                    {
                        std::cerr << "\e[0;31mError: unable to receive request data on fd" << client->getFd() << "\e[0m" << std::endl;
                        this->_poll.removeSocket(client);
                        client->close();
                        continue;
                    }
                    if (!request.empty())
                    {
                        if (client->send(hello) <= 0) // will have to implement parsing the request and building the appropriate response.
                            std::cerr << "\e[0;31mError: unable to receive request data on fd" << client->getFd() << "\e[0m" << std::endl;
                    }
                    this->_poll.removeSocket(client);
                    client->close(); // listeners sockets will need to be explicitly closed on server shutdown.
                }
            }
        }
    }
}

std::ostream &operator<<(std::ostream &out, WebServer const &in) 
{
	(void)in;
	return (out);
}