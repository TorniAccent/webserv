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

NAME = server

SRC = CParser.cpp webserv.cpp Server.cpp ResponseMaker.cpp

all:
	clang++ $(SRC) -o $(NAME)

clean:

fclean:

re:
