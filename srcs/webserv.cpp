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

#include "core/Server.hpp"

int main(int argc, char **argv)
{

//	std::cout << t.substr(0, 2) << std::endl;
	if (argc > 2)
	{
		std::cout << "too many arguments6 max is 1" << std::endl;
		return (1);
	}
	try {
		Server server(argv[1]);
		server.start();

	}
	catch (const char *e) {
		std::cout << paintString("[FAIL]", BOLD, RED, 0);
//		std::cout << "\e[0;31m[FAIL]\e[0m" << std::endl;
		std::cout << "\e[1;31mError: \e[0m" << e << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "lol" << std::endl;
	}
	return (0);
}
