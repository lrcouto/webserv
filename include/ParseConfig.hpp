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
# define PARSECONFIG_HPP

# include "libs.hpp"
# include "ParseDirectives.hpp"
# include "Server.hpp"
# include "Location.hpp"

class ParseConfig {
    private:
		typedef std::pair<std::string, std::vector<std::string> > (*_parseConfigFn)(const std::string &line);

        std::string 							_inputFile;
		std::vector<std::string>				_directives;
		std::map<std::string, _parseConfigFn>	_parseConfigFns;
		std::vector<Server>						_serverData;

	public:
        ParseConfig(void);
        ~ParseConfig(void);
		ParseConfig &operator=(ParseConfig const &other);

		void					initConfigFns(void);
		void					initDirectiveVector(void);
        std::vector<Server>		execute(std::string inputFilePath);
        void    				normalizeWhitespaces(void);
        bool    				checkCurlyBracesMatch(void);
        bool    				checkServerBlock(void);
		std::string				findDirective(std::string line);
		void    				processServer(std::string serverBlock);
		void 					splitOffLocationBlocks(std::string &serverBlock, Server &server);
		void 					processLocation(std::string locationBlock); //pass location reference

	    class ParseSyntaxError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31mError: Invalid syntax on config file.\e[0m");
				}
		};
};

#endif