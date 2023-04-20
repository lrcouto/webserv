/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 00:26:49 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/19 00:26:49 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ParseConfig.hpp"

ParseConfig::ParseConfig(void) 
{
    initConfigFns();
    initDirectiveVector();
	return ;
}

ParseConfig::~ParseConfig(void) 
{
	return ;
}

ParseConfig &ParseConfig::operator=(ParseConfig const &other) 
{
	if (this != &other) {
		std::cout << "ParseConfig copy assignment operator called" << std::endl;
	}
	return (*this);
}

void	ParseConfig::initConfigFns(void)
{
    this->_parseConfigFns["autoindex"] = ParseDirectives::parseAutoindex;
    this->_parseConfigFns["cgi"] = ParseDirectives::parseCgi;
    this->_parseConfigFns["client_max_body_size"] = ParseDirectives::parseClientMaxBodySize;
    this->_parseConfigFns["error_page"] = ParseDirectives::parseErrorPage;
    this->_parseConfigFns["index"] = ParseDirectives::parseIndex;
    this->_parseConfigFns["limit_except"] = ParseDirectives::parseLimitExcept;
    this->_parseConfigFns["listen"] = ParseDirectives::parseListen;
    this->_parseConfigFns["location"] = ParseDirectives::parseLocation;
    this->_parseConfigFns["redirect"] = ParseDirectives::parseRedirect;
    this->_parseConfigFns["root"] = ParseDirectives::parseRoot;
    this->_parseConfigFns["server_name"] = ParseDirectives::parseServerName;
}

void	ParseConfig::initDirectiveVector(void)
{
    for (std::map<std::string, _parseConfigFn>::iterator it = this->_parseConfigFns.begin(); it != this->_parseConfigFns.end(); ++it)
        this->_directives.push_back(it->first);
}

void    ParseConfig::execute(std::string inputFilePath)
{
    std::ifstream       inputFile(inputFilePath.c_str());
    std::stringstream   content;

    content << inputFile.rdbuf();
    this->_inputFile = content.str();
    inputFile.close();
    normalizeWhitespaces();

    if (checkCurlyBracesMatch() && checkServerBlock())
    {
        std::istringstream iss(this->_inputFile);
        std::string line;
        std::string key;
        while (std::getline(iss, line))
        {
            key = findDirective(line);
            std::map<std::string, _parseConfigFn>::iterator it = this->_parseConfigFns.find(key);
                if (it != this->_parseConfigFns.end())
                    it->second(line);
        }
    }
    else
        throw(ParseSyntaxError());
    return ;
}

void    ParseConfig::normalizeWhitespaces(void)
{
    size_t startPos = this->_inputFile.find_first_not_of(" \t\r\n");
    if (startPos != std::string::npos)
        this->_inputFile.erase(0, startPos);

    size_t endPos = this->_inputFile.find_last_not_of(" \t\r\n");
    if (endPos != std::string::npos)
        this->_inputFile.erase(endPos + 1);

    for (size_t i = 0; i < this->_inputFile.size(); ++i) 
        if (this->_inputFile[i] == ' ' || this->_inputFile[i] == '\t' || this->_inputFile[i] == '\r') 
            this->_inputFile[i] = ' ';
}

bool    ParseConfig::checkCurlyBracesMatch(void)
{
    int openBraces = 0;
    int closeBraces = 0;

    for(size_t i = 0; i < this->_inputFile.length(); i++)
    {
        if (this->_inputFile[i] == '{')
            openBraces++;
        else if (this->_inputFile[i] == '}')
            closeBraces++;
    }

    if (openBraces == closeBraces)
        return true;
    return false;
}

bool    ParseConfig::checkServerBlock(void)
{
    size_t pos = this->_inputFile.find("{");
    if ((this->_inputFile.substr(0, pos) == "server ") || (this->_inputFile.substr(0, pos + 1) == "server{")) //does not cover most bad syntax cases.
        return true;
    return false;
}

std::string ParseConfig::findDirective(std::string line)
{
    for (std::vector<std::string>::const_iterator it = this->_directives.begin(); it != this->_directives.end(); ++it) 
    {
        if (line.find(*it) != std::string::npos)
            return *it;
    }
    return "";
}
