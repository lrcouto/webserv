/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:17:14 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/23 22:29:30 by maolivei         ###   ########.fr       */
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

    public:
        Logger(void);
        ~Logger(void);

        void debug(std::string const &message);
        void info(std::string const &message);
        void warning(std::string const &message);
        void error(std::string const &message);

    private:
        void        _print(std::string const &message, LogLevel level);
        std::string _timestamp(void);
};

#endif /* LOGGER_HPP */
