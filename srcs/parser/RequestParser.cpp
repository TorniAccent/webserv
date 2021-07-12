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

bool	RequestParser::parseHeader(std::vector<std::string> &header) {
	int a;

//	std::cout << header.front() << std::endl;
	if (header.front().find("GET") != npos)
		_method = "GET";
	else if (header.front().find("POST") != npos)
		_method = "POST";
	for(int i = 0; header[i] != header.back(); i++)
	{
		a = header[i].find("boundary");
		if (a != npos)
		{
			_boundary = header[i].substr(a + 9, header[i].length() - (a + 9));
		}
	}
	for(int i = 0; header[i] != header.back(); i++)
	{
		a = header[i].find("Content-Length:");
		if (a != npos)
		{
			_content_length = std::stoi(header[i].substr(15, header[i].length()));
		}
	}
	return (true);
}

std::string		RequestParser::getBoundary() const {
	return (_boundary);
}

std::string		RequestParser::getMethod() const {
	return (_method);
}

std::string 	RequestParser::getURI() const {
	return ("/images/favicon.ico");
}

std::string 	RequestParser::getContentType() const {
	return ("multipart/form-data");
}

int 			RequestParser::getError() const {
	return (400);
}

size_t			RequestParser::getContentLength() const {
	return (_content_length);
}

RequestParser&	RequestParser::operator=(const RequestParser &r) {
	return (*this);
}

std::pair<std::string, int> RequestParser::getHost() const {
	return (std::make_pair("MailRu", 2021));
}
