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

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "ParametricException.hpp"
#include "libs.hpp"

class Location {
    private:
        std::string                                      _path;
        std::map<std::string, std::vector<std::string> > _locationData;

    public:
        Location(void);
        ~Location(void);

        void insertLocationData(std::pair<std::string, std::vector<std::string> > directive);

        std::vector<std::string> getValue(std::string key);
        std::string              getPath(void);

        void setPath(std::string path);

        class DuplicateDirectiveError : public ParametricException {
            public:
                DuplicateDirectiveError(std::string const &directive);
                virtual char const *what() const throw();
        };

        class DirectiveNotAllowedError : public ParametricException {
            public:
                DirectiveNotAllowedError(std::string const &directive);
                virtual char const *what() const throw();
        };

    private:
        bool _isUniqueDirective(std::string const &directive) const;
        bool _isDirectiveNotAllowed(std::string const &directive) const;
        bool _isDirectiveOnMap(std::string const &directive) const;
};

#endif
