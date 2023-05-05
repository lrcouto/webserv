/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:33:50 by lcouto            #+#    #+#             */
/*   Updated: 2023/04/13 17:33:50 by lcouto           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_HPP
#define LIBS_HPP

// C LIBRARIES

#include <arpa/inet.h>  // Functions to convert IP addresses.
#include <cctype>       // Standard library implementation of C's ctype.
#include <cstdlib>      // Standard library implementation of C's stdlib.
#include <dirent.h>     // Directory handling functions.
#include <errno.h>      // Error codes to indicate system call errors.
#include <fcntl.h>      // File control operations like fcntl.
#include <netdb.h>      // Functions to perform DNS lookups and IP address conversions.
#include <netinet/in.h> // Internet address-related data types and macros.
#include <poll.h>       // The poll system call for I/O multiplexing.
#include <stdio.h>      // Error functions (perror, strerror).
#include <stdlib.h>     // Standard C library.
#include <string.h>     // C-style string functions like strlen and strcpy.
#include <sys/socket.h> // Socket-related system calls.
#include <sys/stat.h>   // Information about files and directories.
#include <sys/types.h>  // System types like pid_t and size_t.
#include <unistd.h>     // System calls like read, write, and close.

// C++ LIBRARIES

#include <algorithm> // STL algorithms.
#include <csignal>   // Signal handling.
#include <ctime>     // Time and date functions.
#include <fstream>   // File stream.
#include <iostream>  // Standard input/output operations.
#include <map>       // Map container class.
#include <sstream>   // Parsing strings.
#include <string>    // String data type and its operations.
#include <utility>   // Pair class.
#include <vector>    // Vector container class.

// LOCAL LIBRARIES

#include "ftstring.hpp" // String manipulation functions.

// HTTP DEFINES

#define SP          " "
#define CRLF        "\r\n"
#define DOUBLE_CRLF "\r\n\r\n"

#define CR          '\r'
#define LF          '\n'
#define WHITESPACE  ' '

#endif
