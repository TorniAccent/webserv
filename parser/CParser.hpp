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
/*
 *  config - это путь к конфиг-файлу, относительно нашей программы-сервера. Открываешь, считываешь и парсишь.
 */
struct Address { int port; std::string ip; }; // сюда парсишь port и ip.
struct Location{std::string methods; };
struct Server {
		std::vector<Address> getAddress() const;
		std::map<std::string, Location> getLocation() const;
	private:
		std::vector<Address> address;
		std::map<std::string, Location> location;
};


class Parser {
	private:
		Parser();
		std::vector<Address> listen; // уникальные   ip:host
		std::vector<Server> server;
		 
	public:
		Parser(char *config); // если config == NULL, нужно расспарсить config file по-умолчанию.
		~Parser(); // динамическую память нужно почистить, если она была выделена.
		std::vector<Address> getListen(void) const;
		std::vector<Server> getServer() const;
};

#endif

// listen 0.0.0.0:80
// listen 0.0.0.0:80
