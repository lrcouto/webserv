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

class ParseConfig {
    private:
        std::string _inputFile;

	public:
        ParseConfig(void);
        ~ParseConfig(void);
		ParseConfig &operator=(ParseConfig const &other);

        void    execute(std::string inputFilePath);
        void    normalizeWhitespaces(void);
        bool    checkCurlyBracesMatch(void);
        bool    checkServerBlock(void);

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