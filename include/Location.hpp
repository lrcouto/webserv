/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/21 21:53:40 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/21 21:53:40 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef  LOCATION_HPP
# define LOCATION_HPP

# include "libs.hpp"

class Location {
    private:
        std::map<std::string, std::vector<std::string> > _locationData;
    
    public:
    	Location(void);
        ~Location(void);
		// Location(Location const &other);
		// Location &operator=(Location const &other);

        void                        insertLocationData(std::pair<std::string, std::vector<std::string> > directive);
        std::vector<std::string>    getValue(std::string key);    

        class DuplicateDirectiveError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31mError: directives 'autoindex' and 'root' must be unique.\e[0m");
				}
		};

        class DirectiveNotAllowedError : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("\e[0;31mError: directives not allowed inside location block'.\e[0m");
				}
		};

};

#endif