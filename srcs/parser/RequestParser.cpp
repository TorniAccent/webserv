/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 16:43:18 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/19 16:43:20 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser() {

}

RequestParser::RequestParser(Config &configParser): _config(configParser) {
}

RequestParser::~RequestParser() {

}

bool	RequestParser::parseHeader() {
	return (true);
}

std::string		RequestParser::getMethod() const {
	return ("GET");
}

RequestParser&	RequestParser::operator=(const RequestParser &r) {
	return (*this);
}
