/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseDirectives.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 21:25:01 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/19 21:25:01 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ParseDirectives.hpp"

ParseDirectives::DirectiveType ParseDirectives::parseAutoindex(std::string const &line)
{
    std::vector<std::string> tokens, arguments;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() != 2)
        throw InvalidNumberArguments(tokens[0]);
    if (tokens[1] != "on" && tokens[1] != "off")
        throw InvalidArgument(tokens[0], tokens[1]);
    arguments.push_back(tokens[1]);
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseCgi(std::string const &line)
{
    std::vector<std::string> tokens, arguments;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() != 3)
        throw InvalidNumberArguments(tokens[0]);
    if (!_isValidCgiExtension(tokens[1]))
        throw InvalidArgument(tokens[0], tokens[1]);
    if (!_isValidCgiExecutable(tokens[2]))
        throw SystemError(tokens[0], tokens[2]);
    arguments.insert(arguments.begin(), (tokens.begin() + 1), tokens.end());
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseClientMaxBodySize(std::string const &line)
{
    std::string const        size_type_charset = "MmGg";
    std::vector<std::string> tokens, arguments;
    bool                     hasSizeType;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() != 2)
        throw InvalidNumberArguments(tokens[0]);
    hasSizeType = false;
    std::string::const_iterator it;
    for (it = tokens[1].begin(); it != tokens[1].end(); ++it) {
        if (hasSizeType && it != tokens[1].end())
            throw InvalidArgument(tokens[0], tokens[1]);
        if (!std::isdigit(*it)) {
            if (size_type_charset.find(*it) != std::string::npos) {
                if (!hasSizeType) {
                    hasSizeType = true;
                    continue;
                }
            } else {
                throw InvalidArgument(tokens[0], tokens[1]);
            }
        }
    }
    arguments.push_back(tokens[1]);
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseErrorPage(std::string const &line)
{
    char const *arr[] = {"300", "301", "302", "303", "304", "305", "306", "307", "308", "400",
                         "401", "402", "403", "404", "405", "406", "407", "408", "409", "410",
                         "411", "412", "413", "414", "415", "416", "417", "418", "421", "422",
                         "423", "424", "425", "426", "428", "429", "431", "451", "500", "501",
                         "502", "503", "504", "505", "506", "507", "508", "510", "511"};
    std::vector<std::string> const http_codes(arr, (arr + sizeof(arr) / sizeof(arr[0])));

    std::vector<std::string> tokens, arguments;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() < 3)
        throw InvalidNumberArguments(tokens[0]);
    std::vector<std::string>::const_iterator it;
    for (it = (tokens.begin() + 1); it != (tokens.end() - 1); ++it) {
        if (std::find(http_codes.begin(), http_codes.end(), *it) != http_codes.end()) {
            arguments.push_back(*it);
        } else {
            throw InvalidArgument(tokens[0], *it);
        }
    }
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseIndex(std::string const &line)
{
    std::vector<std::string> tokens, arguments;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() < 2)
        throw InvalidNumberArguments(tokens[0]);
    arguments.insert(arguments.begin(), (tokens.begin() + 1), tokens.end());
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseLimitExcept(std::string const &line)
{
    char const                    *arr[] = {"GET", "POST", "DELETE"};
    std::vector<std::string> const http_methods(arr, (arr + sizeof(arr) / sizeof(arr[0])));

    std::vector<std::string> tokens, arguments;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() < 2)
        throw InvalidNumberArguments(tokens[0]);
    std::vector<std::string>::const_iterator it;
    for (it = tokens.begin() + 1; it != tokens.end(); ++it) {
        if (std::find(http_methods.begin(), http_methods.end(), *it) != http_methods.end())
            arguments.push_back(*it);
        else
            throw InvalidArgument(tokens[0], *it);
    }
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseListen(std::string const &line)
{
    std::vector<std::string> tokens, arguments, splitIpPort;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() != 2)
        throw InvalidNumberArguments(tokens[0]);
    splitIpPort = ftstring::split(tokens[1], ':');
    if (splitIpPort.size() > 2)
        throw InvalidArgument(tokens[0], tokens[1]);
    if (splitIpPort.size() == 1) {
        if (_isValidIp(splitIpPort[0])) {
            if (splitIpPort[0] == "localhost")
                arguments.push_back("127.0.0.1");
            else
                arguments.push_back(splitIpPort[0]);
            arguments.push_back("8080");
        } else if (_isValidPort(splitIpPort[0])) {
            arguments.push_back("127.0.0.1");
            arguments.push_back(splitIpPort[0]);
        } else {
            throw InvalidArgument(tokens[0], tokens[1]);
        }
    } else {
        if (!_isValidIp(splitIpPort[0]) || !_isValidPort(splitIpPort[1]))
            throw InvalidArgument(tokens[0], tokens[1]);
        if (splitIpPort[0] == "localhost")
            arguments.push_back("127.0.0.1");
        else
            arguments.push_back(splitIpPort[0]);
        arguments.push_back(splitIpPort[1]);
    }
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseRedirect(std::string const &line)
{
    std::vector<std::string> tokens, arguments;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() != 2)
        throw InvalidNumberArguments(tokens[0]);
    arguments.push_back(tokens[1]);
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseRoot(std::string const &line)
{
    std::vector<std::string> tokens, arguments;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() != 2)
        throw InvalidNumberArguments(tokens[0]);
    // TODO: Maybe check wether the given directory exists
    arguments.push_back(tokens[1]);
    return (std::make_pair(tokens[0], arguments));
}

ParseDirectives::DirectiveType ParseDirectives::parseServerName(std::string const &line)
{
    std::vector<std::string> tokens, arguments;

    tokens = ftstring::split(line, ' ');
    if (tokens.size() < 2)
        throw InvalidNumberArguments(tokens[0]);
    std::vector<std::string>::const_iterator it;
    for (it = (tokens.begin() + 1); it != tokens.end(); ++it) {
        if (*((*it).begin()) == '.' || *((*it).end() - 1) == '.')
            throw InvalidArgument(tokens[0], *it);
        if ((*it).find("..") != std::string::npos)
            throw InvalidArgument(tokens[0], *it);
        std::string::const_iterator strIt;
        for (strIt = (*it).begin(); strIt != (*it).end(); ++strIt)
            if (!std::isalnum(*strIt) && *strIt != '.')
                throw InvalidArgument(tokens[0], *it);
    }
    arguments.insert(arguments.begin(), (tokens.begin() + 1), tokens.end());
    return (std::make_pair(tokens[0], arguments));
}

bool ParseDirectives::_isValidIp(std::string const &ip)
{
    std::vector<std::string> splitIp;

    if (ip == "localhost")
        return (true);
    splitIp = ftstring::split(ip, '.');
    if (splitIp.size() != 4)
        return (false);
    std::vector<std::string>::const_iterator it;
    for (it = splitIp.begin(); it != splitIp.end(); ++it) {
        int octet = ftstring::strtoi(*it);
        if (!ftstring::is_positive_integer(*it) || octet < 0 || octet > 255)
            return (false);
    }
    return (true);
}

bool ParseDirectives::_isValidPort(std::string const &ip)
{
    int port = ftstring::strtoi(ip);
    if (!ftstring::is_positive_integer(ip) || port < 1 || port > 65535)
        return (false);
    return (true);
}

bool ParseDirectives::_isValidCgiExtension(std::string const &extension)
{
    if (extension[0] != '.' || !extension[1])
        return (false);
    std::string::const_iterator it;
    for (it = extension.begin() + 1; it != extension.end(); ++it)
        if (!::isalnum(*it))
            return (false);
    return (true);
}

bool ParseDirectives::_isValidCgiExecutable(std::string const &executable)
{
    std::vector<std::string> splitPath;

    if (executable.find_first_of('/') != std::string::npos && access(executable.c_str(), X_OK) != 0)
        return (false);
    splitPath = ftstring::split(std::getenv("PATH"), ':');
    std::vector<std::string>::const_iterator it;
    for (it = splitPath.begin(); it != splitPath.end(); ++it) {
        std::string binary((*it) + "/" + executable);
        if (access(binary.c_str(), X_OK) == 0)
            return (true);
    }
    return (false);
}

ParseDirectives::InvalidNumberArguments::InvalidNumberArguments(std::string const &str)
{
    this->_message = ERR_PARSE "invalid number of arguments in \"" + str + "\" directive";
}

char const *ParseDirectives::InvalidNumberArguments::what(void) const throw()
{
    return (this->_message.c_str());
}

ParseDirectives::InvalidArgument::InvalidArgument(std::string const &field,
                                                  std::string const &value)
{
    this->_message = ERR_PARSE "invalid argument \"" + value + "\" in \"" + field + "\" directive";
}

char const *ParseDirectives::InvalidArgument::what(void) const throw()
{
    return (this->_message.c_str());
}

ParseDirectives::SystemError::SystemError(std::string const &field, std::string const &value)
{
    this->_message = ERR_PARSE "invalid \"" + value + "\" in \"" + field + "\": " + strerror(errno);
}

char const *ParseDirectives::SystemError::what(void) const throw()
{
    return (this->_message.c_str());
}
