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

std::map<std::string, ParseConfig::_parseConfigFn> ParseConfig::_parseConfigFns
    = ParseConfig::initConfigFns();

std::vector<std::string> ParseConfig::_directives = ParseConfig::initDirectiveVector();

ParseConfig::ParseConfig(void) {}

ParseConfig::~ParseConfig(void) {}

std::map<std::string, ParseConfig::_parseConfigFn> ParseConfig::initConfigFns(void)
{
    std::map<std::string, ParseConfig::_parseConfigFn> parseConfigFns;

    parseConfigFns["autoindex"]            = ParseDirectives::parseAutoindex;
    parseConfigFns["cgi"]                  = ParseDirectives::parseCgi;
    parseConfigFns["client_max_body_size"] = ParseDirectives::parseClientMaxBodySize;
    parseConfigFns["error_page"]           = ParseDirectives::parseErrorPage;
    parseConfigFns["index"]                = ParseDirectives::parseIndex;
    parseConfigFns["limit_except"]         = ParseDirectives::parseLimitExcept;
    parseConfigFns["listen"]               = ParseDirectives::parseListen;
    parseConfigFns["redirect"]             = ParseDirectives::parseRedirect;
    parseConfigFns["root"]                 = ParseDirectives::parseRoot;
    parseConfigFns["server_name"]          = ParseDirectives::parseServerName;

    return (parseConfigFns);
}

std::vector<std::string> ParseConfig::initDirectiveVector(void)
{
    std::vector<std::string> directives;

    directives.push_back("autoindex");
    directives.push_back("cgi");
    directives.push_back("client_max_body_size");
    directives.push_back("error_page");
    directives.push_back("index");
    directives.push_back("limit_except");
    directives.push_back("listen");
    directives.push_back("redirect");
    directives.push_back("root");
    directives.push_back("server_name");

    return (directives);
}

std::vector<Server> ParseConfig::execute(std::string inputFilePath)
{
    std::ifstream       inputFile(inputFilePath.c_str());
    std::stringstream   content;
    std::vector<Server> serverData;

    if (!inputFile.is_open()) {
        throw ParseConfigException("failed to open input file");
    }

    content << inputFile.rdbuf();
    this->_inputFile = content.str();
    inputFile.close();

    if (!checkCurlyBracesMatch() || !checkServerBlock())
        throw ParseConfigException("syntax error detected in input file");

    int    braces            = 0;
    size_t blockStart        = 0;
    bool   insideServerBlock = false;
    for (size_t i = 0; i < this->_inputFile.size(); i++) {
        char c = this->_inputFile[i];
        if (c == '{') {
            if (insideServerBlock == false) {
                insideServerBlock = true;
                blockStart        = i;
            }
            braces++;
        } else if (c == '}') {
            braces--;
            if (braces == 0) {
                processServer(this->_inputFile.substr(blockStart, i - blockStart + 1));
                insideServerBlock = false;
                blockStart        = i;
            }
        }
    }
    return (this->_serverData);
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

std::string ParseConfig::findDirective(std::string &line)
{
    std::vector<std::string>::const_iterator it;
    for (it = this->_directives.begin(); it != this->_directives.end(); ++it) {
        size_t pos = line.find(*it);

        if (line.find(*it) != std::string::npos && pos == 0) {
            std::istringstream iss(line);
            std::string        firstToken;

            std::getline(iss, firstToken, ' ');
            if (firstToken != *it)
                throw ParseConfigException("syntax error detected in input file");
            return (*it);
        }
    }
    return ("");
}

void ParseConfig::processServer(std::string serverBlock)
{
    Server server;
    splitOffLocationBlocks(serverBlock, server);

    serverBlock = ftstring::trim(serverBlock, " {}\n\t\v\f\r");
    std::istringstream iss(serverBlock);
    std::string        line, key;

    while (std::getline(iss, line)) {
        std::map<std::string, _parseConfigFn>::iterator it;
        line = ftstring::reduce(line, " \f\n\r\t\v");
        key  = findDirective(line);
        it   = this->_parseConfigFns.find(key);
        if (it != this->_parseConfigFns.end()) {
            server.insertServerData(it->second(line));
        }
    }
    this->_serverData.push_back(server);
}

void ParseConfig::splitOffLocationBlocks(std::string &serverBlock, Server &server)
{
    std::string delimiter = "location ";
    size_t      pos       = 0;

    while ((pos = serverBlock.find(delimiter, pos)) != std::string::npos) {
        size_t blockStart = pos;
        size_t blockEnd   = serverBlock.find_first_of('}', pos);

        Location location;
        processLocation(serverBlock.substr(blockStart, blockEnd - blockStart + 1), location);
        server.insertLocation(location);
        serverBlock.erase(blockStart, blockEnd - blockStart + 1);
        pos = blockStart;
    }
}

void ParseConfig::processLocation(std::string locationBlock, Location &location)
{
    std::istringstream iss(locationBlock);
    std::string        line, key, token;

    std::getline(iss, line);
    std::istringstream firstLine(line);

    std::getline(firstLine, token, ' ');
    std::getline(firstLine, token, ' ');
    location.setPath(token);

    while (std::getline(iss, line)) {
        line = ftstring::trim(line, " {}\n\t\v\f\r");
        std::map<std::string, _parseConfigFn>::iterator it;
        key = findDirective(line);
        it  = this->_parseConfigFns.find(key);
        if (it != this->_parseConfigFns.end()) {
            location.insertLocationData(it->second(line));
        }
    }
}

ParseConfig::ParseConfigException::ParseConfigException(std::string const &message)
{
    _message = "ParseConfig: " + message;
}

char const *ParseConfig::ParseConfigException::what(void) const throw()
{
    return (_message.c_str());
}
