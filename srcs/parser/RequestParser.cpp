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

RequestParser::RequestParser(ConfigParser &configParser): _config(configParser) {
}

RequestParser::~RequestParser() {

}

bool RequestParser::recieve(pollfd sock) {
	char		buffer[BUFFER_SIZE];
	int 		size;
	bool 		isSuccess;
	const char 	*data;
	const char 	*tmp;
	bool 		next_step;

	data = nullptr;
	next_step = false;
	isSuccess = false;
	while(true) {
		size = recv(sock.fd, &buffer, BUFFER_SIZE - 1, 0);
		if (size > 0) {
			buffer[size] = '\0';

			tmp = data;
			data = strjoin(data, buffer);
			delete tmp;
			if (next_step == false) {
				if (collectHeader(data) == true) {
					next_step = true;
				}
			}
			else if (next_step == true) {
				if (collectBody(data) == true) {
					isSuccess = true;
				}
			}
		}
		else if (size == -1 && isSuccess) // nice, data is good. We can send this to ResponseMaker!!!
			break;
		else if (size == 0){
			//client closed connection
			close(sock.fd);
			return (false);
		}
		else if (size == -1)
		{
			break;
			//error detected. Server should close connection
			// types of error. 1) Connection error 2) Wrong http-header 3) Wrong data-size 4) limit 5) ???
		}
	}
	this->_data = const_cast<char*>(data);
//	std::cout << _header_fields.front() << std::endl;
//	std::vector<std::string>::iterator it = _header_fields.begin();
//	while (it != _header_fields.end())
//	{
//		std::cout << *it << " rn" << std::endl;
//		it++;
//	}
//	it = _header_fields.begin();
//	while ()
	return (true);
}

bool RequestParser::collectHeader(const char *data) {
	std::string 		tmp(data);
	std::string 		field;
	static size_t 		last_pos;
	size_t 				pos;

	while(true) {
		pos = tmp.find("\r\n", last_pos);
		if (pos == std::string::npos) {
			std::cout << "!!!!";
			//pos = tmp.find("\r"); нужно обработать
			//pos = tmp.find("\n"); нужно обработать
			return (false);
		}
		else {
			field.append(tmp.substr(last_pos, pos + 1));
			_header_fields.push_back(field);
//			field.append("|");
			std::cout << field;
			std::cout << "[RN]" << '\n';
			field.clear();
			if (tmp[pos + 2] == '\r' && tmp[pos + 3] == '\n') {
				last_pos = 0;
				return (true);
			}
			last_pos = pos + 2;
		}
	}
}

bool RequestParser::collectBody(const char *data) {
	(void)data;
	return (false);
}

std::string RequestParser::getMethod() const {
	return ("GET");
}

std::string RequestParser::getData() const {
	return (_data);
}

const char* RequestParser::strjoin(const char *s1, const char *s2) const{
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

RequestParser &RequestParser::operator=(const RequestParser &r) {
	this->_data = r._data;
	return (*this);
}
