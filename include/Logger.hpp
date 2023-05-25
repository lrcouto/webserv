/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:17:14 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/24 21:23:58 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "libs.hpp"

#define MAX_LEVEL     4
#define LEVEL_DEBUG   "DEBUG"
#define LEVEL_INFO    "INFO"
#define LEVEL_WARNING "WARNING"
#define LEVEL_ERROR   "ERROR"

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

        static std::ostream &endl(std::ostream &os);

        class LogStream {
            public:
                LogStream(LogLevel level);
                ~LogStream(void);

                template <typename T>
                LogStream &operator<<(T const &value)
                {
                    _stream << value;
                    return (*this);
                };

                LogStream &operator<<(std::ostream &(*manipulator)(std::ostream &));

            private:
                LogLevel          _level;
                std::stringstream _stream;
        };

        static LogStream debug;
        static LogStream info;
        static LogStream warning;
        static LogStream error;

    private:
        static std::string _timestamp(void);
};

#endif /* LOGGER_HPP */
