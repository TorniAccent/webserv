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

NAME		=	server

PARSER		= CParser.cpp RequestParser.cpp CParser.hpp RequestParser.hpp
CORE		= Server.cpp Server.hpp
RESPONSE	= ResponseMaker.cpp ResponseMaker.hpp
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
HPP			= $(addprefix -I , $(filter %.hpp, $(FILES)))

OBJS		= $(addprefix $(OBJDIR)/, $(patsubst %.cpp,%.o,$(filter %.cpp,$(FILES_TMP))))
OBJS_SUB	= $(addprefix $(OBJDIR)/, parser core response)

all: $(NAME)

$(NAME): $(OBJDIR) $(OBJS)
	clang++ -Wall -Wextra -Werror $(HPP) $(OBJS) -o $@

$(OBJDIR):
	mkdir -p $(OBJS_SUB)

$(OBJDIR)/%.o: $(SRCSDIR)/%.cpp
	clang++	-Wall -Wextra -Werror $(HPP) -c $< -o $@

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
