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

ResponseMaker::ResponseMaker(ConfigParser &configParser): CParser(configParser){
}

ResponseMaker::~ResponseMaker() {

}

std::string ResponseMaker::makeResponse(RequestParser &requestParser) {

	std::string 		method;
	std::string 		response;

	// server_name тут будет обрабатываться.
	method = requestParser.getMethod(); // ∗ define a list of accepted HTTP Methods for the route
	if (method == "GET")
		response = get(requestParser);
	else if (method == "POST")
		response = post(requestParser);
//	std::cout << request << std::endl;
	return (response);
}

std::string ResponseMaker::get(RequestParser &parser) {
	std::stringstream	content;
	std::stringstream	content2;
	std::stringstream	response;
//	std::streambuf 		cont;
	std::ifstream fin;
	char c;
	int fd;
	int s;
//	int a = 0;

	// default file to answer if the request is a directory
	(void)parser;
	fd = open("root/images/test.jpeg", O_RDONLY);
//	fcntl(fd, F_SETFL, O_NONBLOCK);
//	if (!fin.is_open())
//		throw "file";
	while ((s = read(fd, &c, 1)) > 0) {
		content2.write(&c, 1);
	}
//	50597
	content << "<title>Test C++ HTTP Server</title>\n"
			<< "<h1>Test Server!</h1>\n"
			<< "<p>This is body of the test page...</p>\n"
			<< "<h2>Request headers</h2>\n"
			<< "<pre>" << parser.getData() << "</pre>\n"
			<< "<em><small>Test C++ Http Server</small></em>\n";
//			<< content2 << "\n";

//	std::cout << "Hello " << content2.str().length() << std::endl;
	// Формируем весь ответ вместе с заголовками
	response << "HTTP/1.1 200 OK\r\n"
			 << "Version: HTTP/1.1\r\n"
			 << "Content-Type: text/html\r\n"
			 << "Connection: keep-alive\r\n"
			 << "Content-Length: " << content.str().length()
//			 << "Transfer-Encoding: chunked"
			 << "\r\n\r\n"
//			 << "5059\r\n"
			 << content.str();
//			 << "0" << "\r\n\r\n";
	return (response.str());
}

std::string ResponseMaker::post(RequestParser &parser) {
	std::string response;
	(void)parser;
	return (response);
}