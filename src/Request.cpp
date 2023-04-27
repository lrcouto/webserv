/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/23 21:44:28 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/27 01:30:19 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Request.hpp"

#define SP				" "
#define CRLF			"\r\n"
#define DOUBLE_CRLF		"\r\n\r\n"

Request::Request(void)  : _raw("")
{
	return ;
}

Request::Request(std::string raw) : _raw(raw)
{
	return ;
}

Request::~Request(void) 
{
	clear();
	return ;
}

Request::Request(Request const &other) 
{
	*this = other;
	return ;
}

Request &Request::operator=(Request const &other)
{
    if (this != &other) {
        this->_raw = other._raw;
		this->_method = other._method;
		this->_requestURI = other._requestURI;
		this->_protocol = other._protocol;
		this->_headers = other._headers;
		this->_body = other._body;
    }
    return *this;
}

void    Request::setRawRequest(std::string raw)
{
	this->_raw = raw;
}

std::string	Request::getRawRequest(void)
{
	return this->_raw;
}

std::string	Request::getMethod(void)
{
	return this->_method;
}

std::string	Request::getRequestURI(void)
{
	return this->_requestURI;
}

std::string	Request::getProtocol(void)
{
	return this->_protocol;
}

std::map<std::string, std::string>	Request::getHeaders(void)
{
	return this->_headers;
}

std::string	Request::getBody(void)
{
	return this->_body;
}

void	Request::parse(void)
{
	if (this->_raw.empty())
		throw (EmptyRequestError());
	
	std::stringstream rawStream;
	rawStream << this->_raw;

	parseRequestLine(rawStream);
	parseHeaders(rawStream);
	parseBody(rawStream);
}

void	Request::parseRequestLine(std::stringstream &rawStream)
{
	std::string	requestLine;
	std::getline(rawStream, requestLine, '\r');

	std::stringstream requestStream(requestLine);

	requestStream >> this->_method;
	requestStream >> this->_requestURI;
	requestStream >> this->_protocol;
}

void	Request::parseHeaders(std::stringstream &rawStream)
{
	size_t headerEndPosition = rawStream.str().find(DOUBLE_CRLF);
	std::string headerChunk = rawStream.str().substr(0, headerEndPosition);

	std::stringstream headers(headerChunk);
	std::string line;

	while (std::getline(headers, line, '\r')) {
		std::string::size_type pos = line.find(':');
        if (pos != std::string::npos) {
            std::string key = ftstring::trim(line.substr(0, pos), " \f\n\r\t\v");
            std::string value = ftstring::trim(line.substr(pos + 1), " \f\n\r\t\v");
            this->_headers[key] = value;
		}
	}
}

void	Request::parseBody(std::stringstream &rawStream)
{
	size_t headerEndPosition = rawStream.str().find(DOUBLE_CRLF);
	std::string bodyChunk = ftstring::trim(rawStream.str().substr(headerEndPosition + 4), " \f\n\r\t\v");

	this->_body = bodyChunk;
}

void    Request::clear(void)
{
	this->_raw.clear();
	this->_method.clear();
	this->_requestURI.clear();
	this->_protocol.clear();
	this->_headers.clear();
	this->_body.clear();
}

std::ostream &operator<<(std::ostream &out, Request &in) 
{
    out << "\n\e[1;32mRaw Request:\e[0m" << std::endl << in.getRawRequest() << std::endl;
    out << "\e[1;32mMethod: \e[0m" << in.getMethod() << std::endl;
    out << "\e[1;32mRequest URI\e[0m: " << in.getRequestURI() << std::endl;
    out << "\e[1;32mProtocol\e[0m: " << in.getProtocol() << std::endl;
    out << "\e[1;32mHeaders:\e[0m" << std::endl;
    std::map<std::string, std::string> headers = in.getHeaders();
    std::map<std::string, std::string>::const_iterator it;
    for (it = headers.begin(); it != headers.end(); ++it) {
        out << it->first << ": " << it->second << std::endl;
    }
    out << "\e[1;32mBody:\e[0m" << std::endl << in.getBody() << std::endl;
    return out;
}