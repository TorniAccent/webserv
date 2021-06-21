/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/06 14:41:58 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/06 14:41:59 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *config): _configParser(config) {
	_fds = new pollfd[1];
	_fds[0].fd = -2;
	_fds_size = 0;
	_nfds = 0;
}


Server::~Server() {
	delete []_fds;
}

void Server::start() {
	int	events;

	std::cout << paintString("\n[Running server]", BOLD, BWHITE, 0) << std::endl;
	std::cout << paintString(" Parsing   -> [OK]", BOLD, GREEN, 0) << std::endl;

	startListen(_configParser.getListen());

	std::cout << paintString("[Success]", BOLD, BWHITE, 0) << '\n' <<std::endl;
	std::cout << paintString("[Waiting to events]", BOLD, BWHITE, 0) << std::endl;

	while(true) {
		events = poll(_fds, _nfds + 1, 0);
		if (events == -1)
			throw std::strerror(errno);
		else if (events == 0)
			continue ;
		findEvent(events);
	}
}

void Server::findEvent(int events) {
	std::string		response;
	const char		*request;
	ResponseMaker	responseMaker;

	(void)events;
	for (int i = 0; (_fds[i].fd != -2); i++) {
		if (_fds[i].revents == NEW_CONNECTION)
			acceptConnection(_fds[i], i);
		else if (_fds[i].revents == REQUEST_RESPONSE) {
			request = getRequest(_fds[i].fd);
			response = responseMaker.makeResponse(request);
			delete request;
			if (send(_fds[i].fd, response.c_str(), response.length(), 0) < 0)
				throw "over";
		}
		else if (_fds[i].revents >= POLLHUP && _fds[i].revents <= 17)
			deleteSocket(_fds[i]);
		else if (_fds[i].revents == POLLERR)
			throw "POLLERR detected!";
		else if (_fds[i].revents == POLLNVAL)
			throw "Файловый дескриптор не открыт";
		else {
			if (_fds[i].revents != 0 && _fds[i].revents != 4)
				std::cout << _fds[i].revents << std::endl;
		}
	}
}

void Server::startListen(Listen listen) {
	int					opt;
	int					sock;
	struct sockaddr		*base;
	struct sockaddr_in	inet = {};

	opt = 1;
	std::cout << paintString(" Listening -> ", BOLD, GREEN, 0);
	for (Listen::iterator it = listen.begin(); it != listen.end(); it++) {
		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // Семейств сокета, тип потокового взаимодействия(тип передачи данных), протокол.
		if (sock == -1)
			throw std::strerror(errno);
		fcntl(sock, F_SETFL, O_NONBLOCK); // от блокирования сокета.
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // от залипания tcp-порта.

		inet.sin_family = PF_INET; // Семейство сокета.
		inet.sin_port = htons(it->port); // переводим значение порта в сетевой порядок следования байт
		inet.sin_addr.s_addr = inet_addr(it->ip.c_str()); // переводит char* в какое-то число, необходимо для bind();

		base = reinterpret_cast<struct sockaddr *>(&inet);
		if (bind(sock, base, sizeof(inet)) == -1)
			throw std::strerror(errno);
		if (::listen(sock, 5) == -1)
			throw std::strerror(errno);
		addSocket(sock, NEW_CONNECTION);
	}
	std::cout << "\033[1;32m[OK]\033[0m" << std::endl;
}

const char 	*Server::getRequest(int fd) {
	char		buffer[BUFFER_SIZE];
	int 		size;
	const char 	*data;
	const char 	*tmp;

	data = nullptr;
	while(true) {
		size = recv(fd, &buffer, BUFFER_SIZE, 0);
		if (size > 0) {
			buffer[size] = '\0';
			tmp = data;
			data = strjoin(data, buffer);
			delete tmp;
		}
		else
			break;
	}
	return (data);
}

void Server::acceptConnection(pollfd lsocket, int i) {
	int					new_client;
	socklen_t			len;
	struct sockaddr		*base;
	struct sockaddr_in	inet = {};

	lsocket.revents = 0;
	base = reinterpret_cast<struct sockaddr *>(&inet);
	len = sizeof(inet);

	new_client = accept(lsocket.fd, base, &len);
	if (new_client == -1)
		throw std::strerror(errno);
	if (fcntl(new_client, F_SETFL, O_NONBLOCK) < 0)
		throw std::strerror(errno);

	std::cout   << paintString("================================", 0, BWHITE, 0) << std::endl;
	std::cout	<< paintString("[NEW_CLIENT]", 1, GREEN, 0)
				<< "\nConnected to " << lsocket.fd << " listening socket"
				<< "\nfd		 = " << new_client << "(" << i << ")"
				<< "\nport		 = " << inet.sin_port
				<< "\nip		 = " << inet.sin_addr.s_addr
				<< "\nIPv4		 = " << inet_ntoa(inet.sin_addr)
				<< std::endl;
	std::cout   << paintString("================================", 0, BWHITE, 0) << std::endl;
	addSocket(new_client, REQUEST_RESPONSE);
}

void Server::addSocket(int sock, short event) {
	int i;

	_nfds = sock;
	for(i = 0; _fds[i].fd != -2; i++)
	{
		if (_fds[i].fd == -1)
		{
			_fds[i].fd = sock;
			_fds[i].events = event;
			_fds[i].revents = 0;
			return ;
		}
	}
	expandPoll();
	addSocket(sock, event);
}

void Server::deleteSocket(pollfd &socket) {
	std::cout << "\033[0;93mКлиент " <<  socket.fd  << " разорвал соединение!\033[0m" << std::endl;
	close(socket.fd);
	socket.fd = -1;
	socket.events = 0;
	socket.revents = 0;
}

void Server::expandPoll() {
	pollfd	*tmp;
	pollfd	*tmp2;
	int 	i;

	_fds_size += 100;
	tmp = new pollfd[_fds_size];
	tmp[_fds_size - 1].fd = -2;
	for(i = 0; _fds[i].fd != -2; i++)
	{
		tmp[i].fd = _fds[i].fd;
		tmp[i].events = _fds[i].events;
	}
	for(int a = i; tmp[a].fd != -2; a++)
	{
		tmp[a].fd = -1;
	}
	tmp2 = _fds;
	_fds = tmp;
	delete []tmp2;
}

const char* Server::strjoin(const char *s1, const char *s2) const{
	size_t 	i;
	size_t 	i2;
	char 	*res;

	if (!s1) {
		return(strdup(s2));
	}
	i = strlen(s1);
	std::cout << "flag" << std::endl;
	i2 = strlen(s2);
	res = new char[i + i2 + 1];

	strcpy(res, s1);
	strcat(res, s2);
	res[i + i2] = '\0';
	return (res);
}

std::string paintString(const std::string &string, int format, int color, int background)
{
	std::stringstream res;

	if (format <= 0)
		format = 10;
	if (color <= 0)
		color = 39;
	background = background + 10;
	if (background <= 10)
		background = 49;
	res << "\033["
		<< std::to_string(format)	  << ';'
		<< std::to_string(color) 	  << ';'
		<< std::to_string(background) << 'm'
		<< string << "\033[0m";
	return (res.str());
}