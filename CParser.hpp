/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CParser.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/06 14:48:12 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/06 14:48:14 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CPARSER_HPP
# define CPARSER_HPP
# include <iostream>
# include <vector>
# include <string>
# include <map>
//# include "webserv.hpp"
/*
 *  config - это путь к конфиг-файлу, относительно нашей программы-сервера. Открываешь, считываешь и парсишь.
 */

struct Address {
	int port;
	std::string ip;
};

struct Location {
	std::string methods;
};

struct ServerConf {
	std::vector<Address> getAddress() const;
	std::map<std::string, Location> getLocation() const;
private:
	std::vector<Address> address;
};

class Parser {
	private:
		Parser();
		std::vector<Address> listen; // уникальные   ip:host
		std::vector<ServerConf> server;
		 
	public:
		Parser(char *config); // если config == NULL, нужно расспарсить config file по-умолчанию.
		~Parser(); // динамическую память нужно почистить, если она была выделена.
		std::vector<Address> getListen(void) const;
		std::vector<ServerConf> getServer() const;
};

#endif

