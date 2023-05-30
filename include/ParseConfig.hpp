/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseConfig.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/19 00:26:54 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/19 00:26:54 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSECONFIG_HPP
#define PARSECONFIG_HPP
#define PARSECONFIG_HPP

#include "Location.hpp"
#include "ParametricException.hpp"
#include "ParseDirectives.hpp"
#include "Server.hpp"
#include "libs.hpp"

class ParseConfig {
    private:
        typedef ParseDirectives::DirectiveType (*_parseConfigFn)(std::string const &line);

        static std::vector<std::string>              _directives;
        static std::map<std::string, _parseConfigFn> _parseConfigFns;

        std::string         _inputFile;
        std::vector<Server> _serverData;

    public:
        ParseConfig(void);
        ~ParseConfig(void);

        static std::map<std::string, ParseConfig::_parseConfigFn> initConfigFns(void);
        static std::vector<std::string>                           initDirectiveVector(void);

        std::vector<Server> execute(std::string inputFilePath);
        bool                checkCurlyBracesMatch(void);
        bool                checkServerBlock(void);
        std::string         findDirective(std::string &line);
        void                processServer(std::string serverBlock);
        void                splitOffLocationBlocks(std::string &serverBlock, Server &server);
        void                processLocation(std::string locationBlock, Location &location);

        class ParseConfigException : public ParametricException {
            public:
                ParseConfigException(std::string const &message);
                virtual char const *what() const throw();
        };
};

#endif
