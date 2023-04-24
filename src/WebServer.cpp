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
    std::string request;

    this->_serverData = this->_parseConfig.execute(inputFilePath);

    // vvvvv PRINTING FOR TESTING - REMOVE THIS BLOCK vvvvv

    for (size_t i = 0; i < this->_serverData.size(); i++)
    {
        std::vector<std::string> value = this->_serverData[i].getValue("listen");
        std::cout << "\e[1;32m" << value[0] << "\e[0m" << std::endl;
        std::vector<Location> locations = this->_serverData[i].getLocations();
        for (size_t i = 0; i < locations.size(); i++)
            std::cout << "\e[0;32m" << locations[i].getPath() << "\e[0m" << std::endl;
    }

    // ^^^^^ PRINTING FOR TESTING - REMOVE THIS BLOCK ^^^^^

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
                    request.clear();
                    std::cout << "Receiving request through client fd " << client->getFd() << std::endl;
                    if (client->receive(request) < 0)
                    {
                        std::cerr << "\e[0;31mError: unable to receive request data on fd" << client->getFd() << "\e[0m" << std::endl;
                        this->_poll.removeSocket(client);
                        continue;
                    }
                    if (!request.empty())
                    {
                        std::cout << "\e[0;32m" << request << "\e[0m" << std::endl;
                        if (client->send(hello) <= 0) // will have to implement parsing the request and building the appropriate response.
                            std::cerr << "\e[0;31mError: unable to receive request data on fd" << client->getFd() << "\e[0m" << std::endl;
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