/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 16:28:00 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/26 16:28:01 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Executor.hpp"

/*
 * 1) Получение данных
 * 2) Вычленение и парсинг первых двух полей хедера (method-uri, host:port)
 * 3) Считывание всего хедера и его парсинг
 * 4) Ищем нужный Location
 * 5) Выполняем метод
 * 6) Формируем ответ
 * 7) Отправляем ответ
 */

Executor::Executor(Config &configParser, RequestParser &requestParser):
_configParser(configParser),  _requestParser(requestParser) {
	_header_size = 0;
	_error = 0;
//	_max_body_size = _configParser
}

/*
 * first_step - is reading of first two header fields
 * second_step - is reading of all header, this is last step.
 */

bool Executor::receiveRequest(pollfd &sock) {
	char						buffer[BUFFER_SIZE];
	int 						size;
	size_t 						res;
	std::vector<std::string>	splitted_header;
	std::string 				tmp;
	std::string 				header;

	_sock = sock;
	while(true) {
		size = recv(sock.fd, &buffer, BUFFER_SIZE - 1, 0);
		if (size > 0) {
			buffer[size] = '\0';
			tmp.append(buffer);
			res = tmp.find("\r\n\r\n");

			if (res == npos) {
				_header_size = _header_size + size;
				if (_header_size >= MAX_HEADER_SIZE)
					_error = 413;
			}
			else
			{
				if (res >= MAX_HEADER_SIZE)
					_error = 413;
				else {
					header.append(tmp.substr(0, res));
					splitHeader(splitted_header, header);
					_error = _requestParser.parseHeader(splitted_header);
					tmp.erase(0, res + 4);
					_body.write(tmp.c_str(), tmp.length());
					_max_body_size = tmp.length();
					break ;
				}
			}
		}
		else if (size == -1) // nice, data is good. We can send this to ResponseMaker!!!
			break;
		else if (size == 0){
			_error = 499;
		}
//		else if (size == -1)
//		{
//			if (_error == 0)
//				_error = 500;
//			return (false);
//			//error detected. Server should close connection
//			// types of error. 1) Connection error 2) Wrong http-header 3) Wrong data-size 4) limit 5) ???
//		}
	}

//	std::vector<std::string>::iterator it = splitted_header.begin();
//	while (it != splitted_header.end())
//	{
//		std::cout << *it << "|" << std::endl;
//		it++;
//	}
//	std::cout << _requestParser.getBoundary() << "bbb" << std::endl;
//	std::cout << "---------------" <<std::endl;
	return (true);
}

bool Executor::executeMethod() {
	bool res;
	std::string method;
	method = _requestParser.getMethod();
	selectLocation(_requestParser.getURI());
	res = false;
	if (method == "GET")
		res = methodGet();
	else if (method == "POST")
		res = methodPost();
	else if (method == "DELETE")
		res = methodDelete();
	return (res);
}

bool Executor::methodDelete() {
	return (false);
}

bool Executor::methodPost() {
	size_t		content_length;
	std::string	content_type;

	content_length = _requestParser.getContentLength();
	content_type = _requestParser.getContentType();
	//	if (content_length > selectLocation(_requestParser.getURI()).getBodySize() -> 413  error
	if (!content_length)
	{
			_error = readChunkedBody();
	}
	else
	{
			_error = readFixBody(content_length);
	}
	selectFunction(content_type);
	if (_error) {
		return (false);
	}
	return (true);
}

bool Executor::methodGet() {
//	getLocation
//	_configParser.

	return (false);
}

int	Executor::readChunkedBody() {
	char		buffer[BUFFER_SIZE];
	size_t 		size;

	while (1) {
		size = recv(_sock.fd, &buffer, BUFFER_SIZE, 0);
		if (size > 0)
		{
		}
		else if (!size)
			return (499);
		else
		{
			break ;
//			return (400);
		}
	}
	return (0);
}

int Executor::readFixBody(size_t content_length) {
	char				buffer[BUFFER_SIZE];
	ssize_t				size;
//	std::stringstream	body;

	while (true) {
		size = recv(_sock.fd, &buffer, BUFFER_SIZE, 0);
		std::cout << size << std::endl;
		if (size < 0)
			std::cerr << std::strerror(errno) << "zzzz" << std::endl;
		if (size > 0)
		{
//			buffer[size] = '\0';
			_body.write(buffer, size);
			_max_body_size += size;
			if (_max_body_size > content_length)
				return (413);
			else if (_max_body_size == content_length)
				return (1);
		}
		else if (!size)
			return (499);
		else
		{
//			break ;
//			return (400);
		}
	}
//	(this->*postType)(buffer, size);
	return (0);
}


int Executor::selectFunction(std::string content_type) {
	int res;

	res = 0;
	if (content_type == "application/x-www-form-urlencoded")
		res = postApplicationXWFU();
	else if (content_type == "multipart/form-data")
		res = postMultiPartFD();
	return (res);
}


int Executor::postMultiPartFD() {
	return (0);
}

int Executor::postApplicationXWFU() {
	return(0);
}

int Executor::writeToFile(std::string filename, char *data, size_t size) {
	std::ofstream file;

	file.open(filename.c_str(),std::fstream::out | std::fstream::app | std::fstream::binary);
	file.write(data, size);
	return (0);
}

int Executor::selectLocation(std::string uri)
{
	Location *location;
	std::vector<Config::Host> host = _configParser.getHosts();
	//Location location(_configParser.getHosts().front().getLocations().front());
	std::pair<std::string, int> requestedHost = _requestParser.getHost();
	std::vector<Config::Host>::iterator it;
	std::vector<std::string>::iterator it2;
	std::vector<Location>::iterator it3;
	std::vector<Location> tmp2;
	std::vector<std::string> tmp;

	it = host.begin();
	while (it != host.end())			//перебираем сервера из конфига
	{
		tmp = it->getServerNames();
		it2 = tmp.begin();
		while (it2 != tmp.end())		//перебираем server_names серверов из конфига
		{
			if (requestedHost.first == *it2){
				std::cerr << "CATCH U!" << std::endl;	//перебираем locations найденного сервера
				tmp2 = it->getLocations();
				it3 = tmp2.begin();
				while (it3 != tmp2.end())
				{
					++it3;
				}
			}
			std::cout << *it2 << std::endl;
			++it2;
		}
		++it;
	}
	_location = location;
	return (0);
}

int Executor::getError() {
	return (_error);
}

bool Executor::splitHeader(std::vector<std::string> &main_strings, std::string &tmp) {
	size_t 		res;
	static int 	strings_count;
	bool 		next_step;

	while (res != npos) {
		next_step = false;
		//// ищем позицию CRLF || CR || LF
		res = tmp.find("\r\n");
		if (res == npos)
			res = tmp.find('\r');
		if (res == npos)
			res = tmp.find('\n');
		if (res != npos)
			next_step = true;

		//// Игнорируем пустые строки до запроса
		if (next_step && res == 0) {
			if (tmp[0] == '\r' && tmp[1] == '\n')
				tmp.erase(0, 2);
			else
				tmp.erase(0, 1);
			next_step = false;
		}
		//// нашли CRLF, добавляем в вектор "main_strings" и удаляем из строки "tmp"
		if (next_step) {
			main_strings.push_back(tmp.substr(0, res));
			if (tmp[res] == '\r' && tmp[res + 1] == '\n')
				tmp.erase(0, res + 2);
			else
				tmp.erase(0, res + 1);
		}
	}

	return (true);
}


bool Executor::sendResponse(pollfd &sock) {
	std::string 		response2;
	std::string 		method;
	std::ifstream		fin;
	std::stringstream	content;
	std::stringstream	content2;
	std::stringstream	response;

	char c;
	int fd;
	int s;

	_data = "HELLO";
	fd = open("root/index.html", O_RDONLY);
//	fcntl(fd, F_SETFL, O_NONBLOCK);
//	if (!fin.is_open())
//		throw "file";
	while ((s = read(fd, &c, 1)) > 0) {
		content2.write(&c, 1);
	}
//	content << "<title>Test C++ HTTP Server</title>\n"
//			<< "<h1>Test Server!</h1>\n"
//			<< "<p>This is body of the test page...</p>\n"
//			<< "<h2>Request headers</h2>\n"
//			<< "<pre>" << _data << "</pre>\n"
//			<< "<em><small>Test C++ Http Server</small></em>\n";
//			<< content2 << "\n";
//	content << content2;

//	std::cout << "Hello " << content2.str().length() << std::endl;
	// Формируем весь ответ вместе с заголовками
	response << "HTTP/1.1 200 OK\r\n"
			 << "Version: HTTP/1.1\r\n"
			 << "Content-Type: text/html\r\n"
			 << "Connection: keep-alive\r\n"
			 << "Content-Length: " << content2.str().length()
			 //			 << "Transfer-Encoding: chunked"
			 << "\r\n\r\n"
			 //			 << "5059\r\n"
			 << content2.str();
//			 << "0" << "\r\n\r\n";
	response2 = response.str();
	if (send(sock.fd, response2.c_str(), response2.length(), 0) < 0)
		throw "over";
	return (true);
}

