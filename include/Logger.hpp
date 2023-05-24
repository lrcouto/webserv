/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:17:14 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/24 00:38:08 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>

#define MAX_LEVEL     4
#define LEVEL_DEBUG   "DEBUG"
#define LEVEL_INFO    "INFO"
#define LEVEL_WARNING "WARNING"
#define LEVEL_ERROR   "ERROR"

#define RED    "\033[0;31m"
#define YELLOW "\033[0;33m"
#define BLUE   "\033[0;34m"
#define PURPLE "\033[0;35m"
#define RESET  "\033[0m"

#define COLOR_DEBUG   BLUE
#define COLOR_INFO    PURPLE
#define COLOR_WARNING YELLOW
#define COLOR_ERROR   RED

enum LogLevel { LOG_DEBUG, LOG_INFO, LOG_WARNING, LOG_ERROR };

class Logger {
    private:
        static std::string _colors[MAX_LEVEL];
        static std::string _levels[MAX_LEVEL];
        std::ostream      &_stdout;

    public:
        Logger(void);
        ~Logger(void);

        std::ostream &debug(void);
        std::ostream &info(void);
        std::ostream &warning(void);
        std::ostream &error(void);
        std::string   end(void);

    private:
        std::ostream &_print(LogLevel level);
        std::string   _timestamp(void);
};

#endif /* LOGGER_HPP */
