# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lcouto <lcouto@student.42sp.org.br>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/11 23:31:18 by lcouto            #+#    #+#              #
#    Updated: 2023/04/11 23:31:18 by lcouto           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

INCLUDES = -I include

CC	= clang++

RM	= rm -rf

CFLAGS	= -Wall -Wextra -Werror -std=c++98


DIR_SRCS	=	src
DIR_BUILD	=	build

SRCS		=	$(DIR_SRCS)/main.cpp \
				$(DIR_SRCS)/WebServer.cpp \
				$(DIR_SRCS)/Socket.cpp \
				$(DIR_SRCS)/Poll.cpp \
				$(DIR_SRCS)/ParseConfig.cpp \
				$(DIR_SRCS)/ParseDirectives.cpp \
				$(DIR_SRCS)/ftstring.cpp \
				$(DIR_SRCS)/Server.cpp \
				$(DIR_SRCS)/Location.cpp \
				$(DIR_SRCS)/Request.cpp \
				$(DIR_SRCS)/Response.cpp \
				$(DIR_SRCS)/ResponseTools.cpp \
				$(DIR_SRCS)/ParametricException.cpp \
				$(DIR_SRCS)/RequestTools.cpp \
				$(DIR_SRCS)/CGI.cpp \
				$(DIR_SRCS)/Logger.cpp \

BUILD		=	$(subst $(DIR_SRCS), $(DIR_BUILD), $(SRCS:.cpp=.o))

ifeq ($(SANITIZE_A),true)
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer
endif

ifeq ($(SANITIZE_L),true)
	CFLAGS += -fsanitize=leak -fno-omit-frame-pointer
endif

ifeq ($(DEBUG),true)
	CFLAGS += -g -fno-limit-debug-info -DDEBUG
endif

$(NAME): $(BUILD)
		@-$(CC) $(CFLAGS) $(BUILD) -o $(NAME)
		@echo "created $(NAME) executable file successfully!"

$(DIR_BUILD)/%.o	:	$(DIR_SRCS)/%.cpp
			@mkdir -p $(DIR_BUILD)
			@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
			@echo "Compiled "$<" successfully!"

all: $(NAME)

clean:
		@$(RM) $(BUILD)
		@$(RM) -r $(DIR_BUILD)
		@echo "Cleanup completed!"

fclean:	clean
		@$(RM) $(NAME)
		@$(RM) $(BUILD)
		@$(RM) -r $(DIR_BUILD)

re:		fclean all

PHONY:	all clean fclean re
