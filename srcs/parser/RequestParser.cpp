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

RequestParser::RequestParser(const char *request) {
	(void)request;

	this->method = "GET";
}

RequestParser::~RequestParser() {

}

std::string RequestParser::getMethod() const {
	return (this->method);
}
