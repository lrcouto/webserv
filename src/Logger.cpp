/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:17:14 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/24 02:37:07 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

std::string Logger::_colors[MAX_LEVEL] = {COLOR_DEBUG, COLOR_INFO, COLOR_WARNING, COLOR_ERROR};
std::string Logger::_levels[MAX_LEVEL] = {LEVEL_DEBUG, LEVEL_INFO, LEVEL_WARNING, LEVEL_ERROR};

Logger::LogStream Logger::debug(LOG_DEBUG);
Logger::LogStream Logger::info(LOG_INFO);
Logger::LogStream Logger::warning(LOG_WARNING);
Logger::LogStream Logger::error(LOG_ERROR);

Logger::Logger(void) {}

Logger::~Logger(void) {}

std::ostream &Logger::endl(std::ostream &os) { return (os << RESET << std::endl); }

std::string Logger::_timestamp(void)
{
    struct tm *time_struct;
    time_t     current_time;
    char       buffer[32];

    current_time = std::time(0);
    time_struct  = localtime(&current_time);
    std::strftime(buffer, sizeof(buffer), "[%d/%m/%Y - %H:%M:%S]", time_struct);
    return (std::string(buffer));
}

Logger::LogStream::LogStream(LogLevel level) : _level(level) {}

Logger::LogStream::~LogStream(void) {}

Logger::LogStream &Logger::LogStream::operator<<(std::ostream &(*manipulator)(std::ostream &))
{
    std::cout << _colors[_level] << _timestamp() << std::setfill(' ') << std::setw(8)
              << _levels[_level] << " - " << _stream.str() << manipulator;
    _stream.str("");
    _stream.clear();
    return (*this);
}
