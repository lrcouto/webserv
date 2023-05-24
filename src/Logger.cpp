/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Logger.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maolivei <maolivei@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 21:17:14 by maolivei          #+#    #+#             */
/*   Updated: 2023/05/23 22:31:45 by maolivei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Logger.hpp"

std::string Logger::_colors[] = {COLOR_DEBUG, COLOR_INFO, COLOR_WARNING, COLOR_ERROR};
std::string Logger::_levels[] = {LEVEL_DEBUG, LEVEL_INFO, LEVEL_WARNING, LEVEL_ERROR};

Logger::Logger(void) {}

Logger::~Logger(void) {}

void Logger::debug(std::string const &message) { _print(message, LOG_DEBUG); }

void Logger::info(std::string const &message) { _print(message, LOG_INFO); }

void Logger::warning(std::string const &message) { _print(message, LOG_WARNING); }

void Logger::error(std::string const &message) { _print(message, LOG_ERROR); }

void Logger::_print(std::string const &message, LogLevel level)
{
    std::cout << _colors[level] << _timestamp() << std::setfill(' ') << std::setw(8)
              << _levels[level] << " - " << message << RESET << std::endl;
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
