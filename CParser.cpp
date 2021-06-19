/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CParser.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/06 14:48:03 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/06 14:48:06 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CParser.hpp"

Parser::Parser(char *config) {
	Address address;
	Address address2;
	Address address3;

	address.ip = "0.0.0.0";
	address2.ip = "0.0.0.0";
	address3.ip = "0.0.0.0";
	address.port = 2021;
	address2.port = 1500;
	address3.port = 1700;
	this->listen.push_back(address);
	this->listen.push_back(address2);
	this->listen.push_back(address3);
}

Parser::~Parser() {

}

std::vector<Address> Parser::getListen(void) const {
 	return (this->listen);
 }

 std::vector<ServerConf> Parser::getServer() const {
	return (this->server);
}