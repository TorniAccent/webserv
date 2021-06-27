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
	_body_size = 0;
//	_max_body_size = _configParser
}

bool Executor::receiveRequest(pollfd &sock) {
	char				buffer[BUFFER_SIZE];
	int 				size;
	bool 				isSuccess;
	const char 			*data;
	const char 			*tmp;
	bool 				next_step;
	std::stringstream	header;
	std::string			first_field;
	size_t 				res;

	data = nullptr;
	next_step = false;
	isSuccess = false;
	while(true) {
		size = recv(sock.fd, &buffer, BUFFER_SIZE - 1, 0);
//		if (_body_size >)
		if (size > 0) {
			buffer[size] = '\0';
			first_field.append(buffer);
			////first_two_Lines
			if (!next_step) {
				res = first_field.find("\r\n");

				if (res <= 0)
					res = first_field.find('\r');
				else
					next_step = true;

				if (res <= 0)
					res = first_field.find('\n');
				else
					next_step = true;

				if (res > 0)
					next_step = true;

//				if (next_step)

			}
			////full_header
			res = first_field.find("\r\n\r\n");
			_body_size += size;
			if (res == npos)
				continue ;
			else
				std::cout << first_field << std::endl;
//			for (int i = 0; buffer[i] != '\0'; i++)
//			{
//				if (buffer[i] == '\r' || '\n')
//				{
//					if (buffer[i] == '\r' && buffer[i + 1] == '\n')
//					{
//
//					}
//
//				}
//			}
//			buffer[size] = '\0';
//			tmp = data;
//			data = strjoin(data, buffer);
//			delete tmp;
//			isSuccess = collectRequest(data, next_step);
		}
		else if (size == -1) // nice, data is good. We can send this to ResponseMaker!!!
			break;
		else if (size == 0){
			_error = 499;
//			return (false);
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
	std::cout << header.str() << std::endl;

//	this->_data = const_cast<char*>(data);

//////	 this code for check header correctness
//	std::list<std::string>::iterator it = _header_fields.begin();
//	while (it != _header_fields.end())
//	{
//		std::cout << *it << "[RN]" << std::endl;
//		it++;
//	}
//	std::cout << "===================" << std::endl;

	return (true);
}

bool Executor::executeMethod() {
	bool res;


	res = false;
	if (_requestParser.getMethod() == "GET")
		res = methodGet();
	else if (_requestParser.getMethod() == "POST")
		res = methodPost();
	else if (_requestParser.getMethod() == "DELETE")
		res = methodDelete();
	return (res);
}

bool Executor::methodDelete() {
	return (false);
}

bool Executor::methodPost() {
	return (false);
}

bool Executor::methodGet() {
	return (false);
}

Config::Host::Location &selectLocation(std::string uri)
{

}


bool Executor::sendResponse(pollfd &sock) {
	std::string 		response2;
	std::string 		method;
	std::stringstream	content;
	std::stringstream	content2;
	std::stringstream	response;
	std::ifstream		fin;

	char c;
	int fd;
	int s;

	fd = open("root/images/test.jpeg", O_RDONLY);
//	fcntl(fd, F_SETFL, O_NONBLOCK);
//	if (!fin.is_open())
//		throw "file";
	while ((s = read(fd, &c, 1)) > 0) {
		content2.write(&c, 1);
	}
	content << "<title>Test C++ HTTP Server</title>\n"
			<< "<h1>Test Server!</h1>\n"
			<< "<p>This is body of the test page...</p>\n"
			<< "<h2>Request headers</h2>\n"
			<< "<pre>" << _data << "</pre>\n"
			<< "<em><small>Test C++ Http Server</small></em>\n";
//			<< content2 << "\n";
//	content << content2;

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
	response2 = response.str();
	if (send(sock.fd, response2.c_str(), response2.length(), 0) < 0)
		throw "over";
	return (true);
}

//// will remove
const char*		Executor::strjoin(const char *s1, const char *s2) const{
	size_t 	i;
	size_t 	i2;
	char 	*res;

	if (!s1) {
		return(strdup(s2));
	}
	i = strlen(s1);
	i2 = strlen(s2);
	res = new char[i + i2 + 1];

	strcpy(res, s1);
	strcat(res, s2);
	res[i + i2] = '\0';
	return (res);
}

bool 			Executor::collectRequest(const char *data, bool &next_step) {

	if (next_step == false) {
		if (collectHeader(data) == true) {
			next_step = true;
//			parseHeader();
			if (!_content_length && !_is_chunked)
				return (true);
		}
	}

	else if (next_step == true) {
		if (collectBody(data) == true) {
			return (true);
		}
	}
	return (next_step);
}

bool 			Executor::collectHeader(const char *data) {
	std::string 		tmp(data);
	std::string 		field;
	static size_t 		last_pos;
	size_t 				pos;

//	while(true) {
//		pos = tmp.find("\r\n", last_pos);
////		if (pos == std::string::npos || pos < 0) {
////			pos = tmp.find("\r"); нужно обработать
////			 pos = tmp.find("\n"); нужно обработать
////			return (false);
//		}
//		else {
//			field.append(tmp.substr(last_pos, pos - last_pos));
//			_header_fields.push_back(field);
//			field.clear();
//			if (tmp[pos + 2] == '\r' && tmp[pos + 3] == '\n') {
//				last_pos = 0;
//				_header_size = pos + 4;
//				return (true);
//			}
//			last_pos = pos + 2;
//		}
//	}
	return (false);
}

bool			Executor::collectBody(const char *data) {
	std::string tmp(data);
	size_t 		pos;
	size_t 		length;

	length = tmp.length() - _header_size;
	if (_is_chunked)
	{
		pos = tmp.find("\r\n", _header_size);
	}
	else if (_content_length)
	{
		if (length == _content_length)
			return (true);
		else if (length > _content_length) {
			_body.write(tmp.substr(_header_size, _content_length).c_str(), _content_length);
			return (false);
		}
		else
			return (false);

	}
	return (true);
}
