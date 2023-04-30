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
	std::cout << "Hello, this is your WebServer ready to go!" << std::endl;
	return ;
}

WebServer::~WebServer(void) 
{
	std::cout << "Bye bye!" << std::endl;
	return ;
}

WebServer::WebServer(WebServer const &other)
{
	std::cout << "WebServer copy constructor called" << std::endl;
	*this = other;
	return ;
}

WebServer &WebServer::operator=(WebServer const &other)
{   	
    if (this != &other) {
		std::cout << "WebServer copy assignment operator called" << std::endl;
	}
	return (*this);
}

void WebServer::run(const std::string &inputFilePath)
{
    Socket *listener;
    Socket *client;
    int ports[3] = {3007, 3008, 3009};
    std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world! "; // hardcoded test message.
    std::string rawRequest;

    this->_serverData = this->_parseConfig.execute(inputFilePath);

    for(int i = 0; i < 3; i++) // hardcoded number of sockets because I have no input file yet.
    {
        listener = new Socket(ports[i]);
        listener->bind(1); // 1 for forcefully releasing port, 0 for not doing that.
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
                    rawRequest.clear();
                    std::cout << "Receiving request through client fd " << client->getFd() << std::endl;
                    if (client->receive(rawRequest) < 0)
                    {
                        std::cerr << "\e[0;31mError: unable to receive request data on fd" << client->getFd() << "\e[0m" << std::endl;
                        this->_poll.removeSocket(client);
                        continue;
                    }
                    if (!rawRequest.empty())
                    {
                        Request request(rawRequest);
                        request.parse();
                        std::cout << request << std::endl;
                        Response response(request);
                        response.setServerData(&this->_serverData[0]); // hardcoded to always get the first server data structure for testing purposes
                        response.assembleResponseString();
                        if (client->send(response.getResponseString()) <= 0) // will have to implement parsing the request and building the appropriate response.
                            std::cerr << "\e[0;31mError: unable to receive request data on fd" << client->getFd() << "\e[0m" << std::endl;
                        request.clear();
                        response.clear();
                    }
                    this->_poll.removeSocket(client);
                }
            }
        }
    }
}

void WebServer::stop(void)
{
    std::cout << "Stopping Webserver..." << std::endl;
    this->_poll.clear(); // in the future will also have to clear server data, request data, etc.
}

std::ostream &operator<<(std::ostream &out, WebServer const &in) 
{
	(void)in;
	return (out);
}