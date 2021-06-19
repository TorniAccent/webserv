/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/10 20:29:55 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/10 20:29:56 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
//# include <iostream>

int main(int argc, char **argv)
{
	try {
		Server server(argv[1]);
		server.start();

	}
	catch (const char *e) {
		std::cout << "\e[0;31m[FAIL]\e[0m" << std::endl;
		std::cout << "\e[1;31mError: \e[0m" << e << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "lol" << std::endl;
	}
	return (0);
}
