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

//	_sock = &sock;
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
////	std::cout << _requestParser.getBoundary() << "bbb" << std::endl;
//	std::cout << "---------------" <<std::endl;
	return (true);
}

bool Executor::executeMethod() {
	bool 		res;
	std::string method;
	int			check;
	int 		objType;

	res = false;
	method = _requestParser.getMethod();
	check = selectLocation(_requestParser.getURI());
	if (!check)
		objType = getResourceType("config");
	else
		return (false);

	if (!objType)
		std::cout << "Magic object" << std::endl;
	else if (objType == EXECUTABLE_FILE)
		executeCGI(method);
	else if (objType == FILE)
		std::cout << "FILE" << std::endl;
	else
		std::cout << "DIRECTORY" << std::endl;

	if (method == "GET")
		res = methodGet(objType);
	else if (method == "POST")
		res = methodPost(objType);
	else if (method == "DELETE")
		res = methodDelete(objType);
	return (res);
}

bool Executor::methodDelete(int obj) {
	return (false);
}

bool Executor::methodPost(int obj) {
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

bool Executor::methodGet(int obj) { // формировать body
	if (_requestParser.getError() != 0) //FIXME отравлять ошибку клиенту
		return (false);
//	_configParser.get
	return (true);
}

bool Executor::executeCGI(std::string method) {
	pid_t		pid;
	int			status;
	int 		exit_status;
	char 		**object_to_execute;
	char 		**env;
	std::string	interpreter;

	// 1) нужно определить расширение cgi. От этого зависит первый аргумент execve. DONE
	// 2) во второй аргумент execve нужно положить путь к запускаемому объекту
	// 3) нужно собрать переменные окружение в двумерный массив по типу ключ-значение
	// 4) если post, то нужно передать данные cgi программе через stdin

	pid = fork();
	if (pid == -1)
		return (false);
	else if (pid == 0) // if fork return 0, it's mean that it's child process
	{
		object_to_execute = new char* [2];
		object_to_execute[1] = NULL;
		interpreter = getInterpreter(_requestParser.getURI());
		if (interpreter.empty()) {
			object_to_execute[0] = NULL;
			if (_error == 510) {
				delete object_to_execute;
				exit(510);
			}
		}
		else
			object_to_execute[0] = const_cast<char*>(interpreter.c_str());
		env = assembleEnv();
		if (execve(interpreter.c_str(), object_to_execute, NULL) == -1)
		{
			if (errno == ENOENT)
				exit(501);
		}
	}
	else
	{
		waitpid(pid, &status, WUNTRACED);
		if (WIFEXITED(status)) // если вернет 0, то значит, что  cgi-программа завершилась аварийно.
			exit_status = WEXITSTATUS(status);
		else
			exit_status = -999;
		if (exit_status == 510 || exit_status == 501) {
			_error = exit_status;
			return (false);
		}
		else if (exit_status == -999){
			_error = 500;
			return (false);
		}
	}
	return (true);
}

char	**Executor::assembleEnv() {
	char 						**res;
	std::vector<std::string>	tmp(23, "");
	std::string					uri;
	size_t						i;
	struct sockaddr_in			addr = {};
	std::string 				client_ip;
	std::string 				host_ip;
	std::string 				client_port;
	std::string 				host_port;
	socklen_t 					size;

	res = new char* [23];
	res[22] = NULL;

	size = sizeof(addr);
	uri = _requestParser.getURI();
	i = uri.find_last_of('/') + 1;

	getpeername(_sock->fd, (struct sockaddr*)&addr, &size);
	client_ip.append(inet_ntoa(addr.sin_addr));
	client_port.append(std::to_string(addr.sin_port));

	getsockname(_sock->fd, (struct sockaddr*)&addr, &size);
	host_ip.append(inet_ntoa(addr.sin_addr));
	host_port.append(std::to_string(addr.sin_port));

	tmp[0]	= "AUTH_TYPE=Basic";
	tmp[1]	= "CONTENT_LENGTH=" + std::to_string(_requestParser.getContentLength());
	tmp[2]	= "CONTENT_TYPE=" + _requestParser.getContentType();
	tmp[3]	= "GATEWAY_INTERFACE=CGI/1.1";
	tmp[4]	= "HTTP_ACCEPT="; //_requestParser.getAccept();
	tmp[5]	= "HTTP_ACCEPT_CHARSET="; //_requestParser.getAcceptCharset();
	tmp[6]	= "HTTP_ACCEPT_ENCODING="; //_requestParser.getAcceptEncoding();
	tmp[7]	= "HTTP_ACCEPT_LANGUAGE="; //_requestParser.getAcceptLanguage();
	tmp[8]	= "HTTP_HOST=" + _requestParser.getHost().first; // ?
	tmp[9] = "HTTP_USER_AGENT=";// _requestParser.getUserAgent();
	tmp[10] = "PATH_INFO=" + _requestParser.getURI();
	tmp[11] = "PATH_TRANSLATED="; //_requestParser.getPath();
	tmp[12] = "QUERY_STRING="; // _requestParser.getQueryString();
	tmp[13] = "REMOTE_ADDR=" + client_ip;
	tmp[14] = "REMOTE_PORT=" + client_port;
	tmp[15] = "REQUEST_METHOD=" + _requestParser.getMethod();
	tmp[16] = "SCRIPT_NAME=" + uri.substr(i, uri.length() - i);
	tmp[17] = "SERVER_NAME=" + host_ip;
	tmp[18] = "SERVER_PORT=" + host_port;
	tmp[19] = "SERVER_PROTOCOL=HTTP/1.1";
	tmp[20] = "SERVER_SOFTWARE=Webserv/1";
	tmp[21] = "HTTP_COOKIE=";

	for (int i = 0; res[i] != NULL; i++)
		res[i] = const_cast<char*>(tmp[i].c_str());
	return (res);
}

std::string Executor::getInterpreter(std::string uri) {
	size_t		check;
	std::string	extension;
	std::string res;

	extension = "";
	res = "";
	check = uri.find_last_of('.');
	if (check != npos)
		extension = uri.substr(check, uri.length() - check);
	else
		return (res);
	if (extension == ".py")
		res = "python";
	else if (extension == ".sh")
		res = "sh";
	else if (extension == ".pl")
		res = "perl";
	else if (extension == ".rb")
		res = "ruby";
	else
		_error = 510;
	return (res);
}

int	Executor::readChunkedBody() {
	char		buffer[BUFFER_SIZE];
	size_t 		size;

	while (1) {
		size = recv(_sock->fd, &buffer, BUFFER_SIZE, 0);
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
		size = recv(_sock->fd, &buffer, BUFFER_SIZE, 0);
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
//		tmp = it->getServerNames();
		it2 = tmp.begin();
		while (it2 != tmp.end())		//перебираем server_names серверов из конфига
		{
			if (requestedHost.first == *it2){
				std::cerr << "CATCH U!" << std::endl;	//перебираем locations найденного сервера
				tmp2 = it->getLocations();
				it3 = tmp2.begin();
				while (it3 != tmp2.end())
				{
//					if ()
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

int	Executor::getResourceType(std::string uri) {
	struct stat buf;
	int			type;
	int 		check;
	int 		mode;
	std::string chmod;

	type = 0;
	check = stat(uri.c_str(), &buf);
	if (check == -1)
		return (0);

	mode = buf.st_mode;
	chmod = std::to_string(toOctal(mode));
	if (S_ISREG(mode))
	{
		type = FILE;
		for (int i = 3; i != 6; i++)
			if (((chmod[i] - 48) % 2) != 0)
				type = EXECUTABLE_FILE;
	}
	else if (S_ISDIR(mode))
		type = DIRECTORY;
	return (type);
}

int	Executor::toOctal(int decimal) {
	int octal;
	int i;
	int tmp;

	octal = 0;
	i = 1;
	while (decimal != 0)
	{
		tmp = decimal % 8;
		decimal = decimal / 8;
		octal = octal + (tmp * i);
		i = i * 10;
	}
	return (octal);
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
	close(fd);
	if (send(sock.fd, response2.c_str(), response2.length(), 0) < 0)
		throw "over";
	return (true);
}

