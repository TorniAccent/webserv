/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseMaker.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 17:12:46 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/19 17:28:04 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ResponseMaker.hpp"

ResponseMaker::ResponseMaker() {

}

ResponseMaker::~ResponseMaker() {

}

std::string ResponseMaker::makeResponse(const char *request) {

	RequestParser		parser(request);
	std::string 		method;
	std::string 		response;

	method = parser.getMethod();
	if (method == "GET")
		response = get(parser);
	else if (method == "POST")
		response = post(parser);
//	std::cout << request << std::endl;
	return (response);
}

std::string ResponseMaker::get(RequestParser &parser) {
	std::stringstream	content;
	std::stringstream	response;

	(void)parser;
	content << "<title>Test C++ HTTP Server</title>\n"
			<< "<h1>Test page</h1>\n"
			<< "<p>This is body of the test page...</p>\n"
			<< "<h2>Request headers</h2>\n"
			<< "<pre>" << "GET" << "</pre>\n"
			<< "<em><small>Test C++ Http Server</small></em>\n";

	// Формируем весь ответ вместе с заголовками
	response << "HTTP/1.1 200 OK\r\n"
			 << "Version: HTTP/1.1\r\n"
			 << "Content-Type: text/html; charset=utf-8\r\n"
			 << "Connection: keep-alive\r\n"
			 << "Content-Length: " << content.str().length()
			 << "\r\n\r\n"
			 << content.str().c_str();
	return (response.str());
}

std::string ResponseMaker::post(RequestParser &parser) {
	std::string response;
	(void)parser;
	return (response);
}