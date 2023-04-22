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

#include "ParseConfig.hpp"

ParseConfig::ParseConfig(void)
{
    initConfigFns();
    initDirectiveVector();
}

ParseConfig::~ParseConfig(void) {}

ParseConfig &ParseConfig::operator=(ParseConfig const &other)
{
    if (this != &other)
        std::cout << "ParseConfig copy assignment operator called" << std::endl;
    return (*this);
}

void ParseConfig::initConfigFns(void)
{
    this->_parseConfigFns["autoindex"]            = ParseDirectives::parseAutoindex;
    this->_parseConfigFns["cgi"]                  = ParseDirectives::parseCgi;
    this->_parseConfigFns["client_max_body_size"] = ParseDirectives::parseClientMaxBodySize;
    this->_parseConfigFns["error_page"]           = ParseDirectives::parseErrorPage;
    this->_parseConfigFns["index"]                = ParseDirectives::parseIndex;
    this->_parseConfigFns["limit_except"]         = ParseDirectives::parseLimitExcept;
    this->_parseConfigFns["listen"]               = ParseDirectives::parseListen;
    this->_parseConfigFns["location"]             = ParseDirectives::parseLocation;
    this->_parseConfigFns["redirect"]             = ParseDirectives::parseRedirect;
    this->_parseConfigFns["root"]                 = ParseDirectives::parseRoot;
    this->_parseConfigFns["server_name"]          = ParseDirectives::parseServerName;
}

void ParseConfig::initDirectiveVector(void)
{
    for (std::map<std::string, _parseConfigFn>::iterator it = this->_parseConfigFns.begin();
         it != this->_parseConfigFns.end();
         ++it)
        this->_directives.push_back(it->first);
}

std::vector<Server> ParseConfig::execute(std::string inputFilePath)
{
    std::ifstream       inputFile(inputFilePath.c_str());
    std::stringstream   content;
    std::vector<Server> serverData;

    content << inputFile.rdbuf();
    this->_inputFile = content.str();
    inputFile.close();

    if (!checkCurlyBracesMatch() || !checkServerBlock())
        throw ParseSyntaxError();

    int     braces = 0;
    size_t  blockStart = 0;
    bool    insideServerBlock = false;
    for (size_t i = 0; i < this->_inputFile.size(); i++) {
        char c = this->_inputFile[i];
        if (c == '{') {
            if (insideServerBlock == false) {
                insideServerBlock = true;
                blockStart = i;                
            }
            braces++;
        }
        else if (c == '}') {
            braces--;
            if (braces == 0) {
                processServer(this->_inputFile.substr(blockStart, i - blockStart + 1));
                insideServerBlock = false;
                blockStart = i;
            }
        }
    }
    return this->_serverData;
}

bool ParseConfig::checkServerBlock(void)
{
    size_t const firstBracket = this->_inputFile.find_first_of('{');
    std::string  firstBlock;

    if (firstBracket == std::string::npos)
        return (false);
    firstBlock = this->_inputFile.substr(0, firstBracket);

    firstBlock = ftstring::reduce(firstBlock, " \f\n\r\t\v");
    if (firstBlock != "server")
        return (false);
    return (true);
}

bool ParseConfig::checkCurlyBracesMatch(void)
{
    size_t openBraces  = 0;
    size_t closeBraces = 0;

    for (size_t i = 0; i < this->_inputFile.length(); ++i) {
        if (this->_inputFile[i] == '{')
            ++openBraces;
        else if (this->_inputFile[i] == '}')
            ++closeBraces;
    }
    if (openBraces != closeBraces)
        return (false);
    return (true);
}

std::string ParseConfig::findDirective(std::string line)
{
    line = ftstring::reduce(line, " \f\n\r\t\v", " ");
    for (std::vector<std::string>::const_iterator it = this->_directives.begin();
         it != this->_directives.end();
         ++it) {
        if (line.find(*it) != std::string::npos) {
            std::istringstream iss(line);
            std::string firstToken;
            std::getline(iss, firstToken, ' ');
            if (firstToken != *it)
                throw (ParseSyntaxError());
            return *it;
        }
    }
    return "";
}

void ParseConfig::processServer(std::string serverBlock)
{
    serverBlock = ftstring::trim(serverBlock, " {}\t\v\f\r");
    Server server;
    splitOffLocationBlocks(serverBlock, server);
    std::istringstream iss(serverBlock);
    std::string line, key;

    while (std::getline(iss, line)) {
        std::map<std::string, _parseConfigFn>::iterator it;
        key = findDirective(line);
        it  = this->_parseConfigFns.find(key);
        if (it != this->_parseConfigFns.end()) {
            server.insertServerData(it->second(line));
        }
    }

    this->_serverData.push_back(server);
}

void ParseConfig::splitOffLocationBlocks(std::string &serverBlock, Server &server)
{
    Location location;
    std::string delimiter = "location ";
    size_t pos = 0;

    while ((pos = serverBlock.find(delimiter, pos)) != std::string::npos) {
        size_t blockStart = pos;
        size_t blockEnd = serverBlock.find_first_of('}', pos);

        processLocation(serverBlock.substr(blockStart, blockEnd - blockStart + 1)); //pass location reference
        server.insertLocation(location);
        serverBlock.erase(blockStart, blockEnd - blockStart + 1);
        pos = blockStart;
    }
}

void ParseConfig::processLocation(std::string locationBlock) //pass location reference
{
    std::cout << "\nBLOCK START\n" << locationBlock << "\nBLOCK END\n" <<  std::endl;
}


