# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: swquinc <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/06/17 20:37:17 by swquinc           #+#    #+#              #
#    Updated: 2021/06/17 20:37:19 by swquinc          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= server

PARSER		= Config.cpp ConfigParser.hpp Host.cpp Location.cpp utils.cpp RequestParser.cpp RequestParser.hpp #CParser.cpp CParser.hpp
CORE		= Server.cpp Server.hpp
RESPONSE	= Executor.cpp Executor.hpp
MAIN		= webserv.cpp

PARSERRDIR	= parser
COREDIR		= core
RESPONSEDIR	= response
OBJDIR		= objs
SRCSDIR		= srcs

FILES_TMP	= $(addprefix $(PARSERRDIR)/, $(PARSER)) \
			  $(addprefix $(COREDIR)/, $(CORE)) \
			  $(addprefix $(RESPONSEDIR)/, $(RESPONSE)) $(MAIN) \

FILES		= $(addprefix $(SRCSDIR)/, $(FILES_TMP))

CPP			= $(filter %.cpp,$(FILES))
HPP1		= $(filter %.hpp, $(FILES))
HPP			= $(addprefix -I ,$(dir $(HPP1)))


OBJS		= $(addprefix $(OBJDIR)/, $(patsubst %.cpp,%.o,$(filter %.cpp,$(FILES_TMP))))
OBJS_SUB	= $(addprefix $(OBJDIR)/, parser core response)

BUFFER_SIZE = -D BUFFER_SIZE=100
FLAGS = -Wno-unused-variable#-Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	clang++ $(FLAGS) $(BUFFER_SIZE) $(HPP) $(OBJS) -o $@

$(OBJDIR):
	mkdir -p $(OBJS_SUB)

$(OBJDIR)/%.o: $(SRCSDIR)/%.cpp
	clang++	$(FLAGS) $(BUFFER_SIZE) $(HPP) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
