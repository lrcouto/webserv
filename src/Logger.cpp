/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:17:14 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/24 00:38:50 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

std::string   Logger::_colors[MAX_LEVEL] = {COLOR_DEBUG, COLOR_INFO, COLOR_WARNING, COLOR_ERROR};
std::string   Logger::_levels[MAX_LEVEL] = {LEVEL_DEBUG, LEVEL_INFO, LEVEL_WARNING, LEVEL_ERROR};

Logger::Logger(void) : _stdout(std::cout) {}

Logger::~Logger(void) {}

std::ostream &Logger::debug(void) { return (_print(LOG_DEBUG)); }

std::ostream &Logger::info(void) { return (_print(LOG_INFO)); }

std::ostream &Logger::warning(void) { return (_print(LOG_WARNING)); }

std::ostream &Logger::error(void) { return (_print(LOG_ERROR)); }

std::string Logger::end(void)
{
    _stdout << RESET << std::endl;
    return ("");
}

std::ostream &Logger::_print(LogLevel level)
{
    _stdout << _colors[level] << _timestamp() << std::setfill(' ') << std::setw(8) << _levels[level]
            << " - ";
    return (_stdout);
}

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
