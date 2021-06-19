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

Server::Server(char *config): configParser(config) {
	fds = new pollfd[1];
	fds[0].fd = -2;
	fds_size = 0;
}

Server::~Server() {
	delete []fds;
}

void Server::start() {
	int					event;
	std::string			response;
	const char		 	*request;
	ResponseMaker		responseMaker;

	std::cout << "\e[1;97mRunning server...\e[0m" << std::endl;
	startListen(configParser.getListen());

	std::cout << "\e[1;32m Polling   -> \e[0m" << std::endl;
	while(1) {
		event = poll(fds, nfds + 1, -1); // поменять 3 аргумент на 0, для неблокируемого опроса
		if (event == -1)
			throw std::strerror(errno);
		else if (event == 0)
			throw "poll timeout"; // поменять на continue для неблокируемого опроса.
		for(int i = 0; (fds[i].fd != -2); i++)
		{
			if (fds[i].revents == POLLRDNORM)
				acceptConnection(fds[i], i);
			else if (fds[i].revents == (POLLIN | POLLOUT))
			{
				request = getRequest(fds[i].fd);
				response = responseMaker.makeResponse(request);
				delete request;
				if (send(fds[i].fd, response.c_str(), response.length(), 0) < 0)
					throw "over";
			}
			else if (fds[i].revents >= POLLHUP && fds[i].revents <= 17)
				deleteSocket(fds[i]);
			else if (fds[i].revents == POLLERR)
				throw "POLLERR detected!";
			else if (fds[i].revents == POLLNVAL)
				throw "Файловый дескриптор не открыт";
			else
			{
				if (fds[i].revents != 0 && fds[i].revents != 4)
					std::cout << fds[i].revents << std::endl;
			}
		}
	}
}

void Server::startListen(Listen listen) {
	int					opt;
	int					sock;
	struct sockaddr		*base;
	struct sockaddr_in	inet;

	opt = 1;
	std::cout << "\e[1;32m Listening -> \e[0m";
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
		addSocket(sock, POLLRDNORM);
	}
	std::cout << "\e[1;32m[OK]\e[0m" << std::endl;
}

const char 	*Server::getRequest(int fd) {
	char				buffer[BUFFER_SIZE];
	int 				size;
	char 				*data;
	char 				*tmp;

	data = NULL;
	while(1) {
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
	struct sockaddr_in	inet;
//	std::pair<std::map<int, in_addr_t>::iterator, bool> ret;

	lsocket.revents = 0;
	base = reinterpret_cast<struct sockaddr *>(&inet);
	len = sizeof(inet);

	new_client = accept(lsocket.fd, base, &len);
	if (new_client == -1)
		throw std::strerror(errno);
	if (fcntl(new_client, F_SETFL, O_NONBLOCK) < 0)
		throw std::strerror(errno);

	std::cout << "==================" << std::endl;
	std::cout	<< "\e[1;32m[NEW_CLIENT]\e[0m"
				<< "\nConnected to " << lsocket.fd << " listening socket"
				<< "\nfd		 = " << new_client << "(" << i << ")"
				<< "\nport		 = " << inet.sin_port
				<< "\nip		 = " << inet.sin_addr.s_addr
				<< "\nIPv4		 = " << inet_ntoa(inet.sin_addr)
				<< std::endl;
	addSocket(new_client, (POLLIN | POLLOUT));
}

void Server::addSocket(int sock, short event) {
	int i;

	nfds = sock;
	for(i = 0; fds[i].fd != -2; i++)
	{
		if (fds[i].fd == -1)
		{
			fds[i].fd = sock;
			fds[i].events = event;
			fds[i].revents = 0;
			return ;
		}
	}
	expandPoll();
	addSocket(sock, event);
}

void Server::deleteSocket(pollfd &socket) {
	std::cout << "\e[0;93mКлиент " <<  socket.fd  << " разорвал соединение!\e[0m" << std::endl;
	close(socket.fd);
	socket.fd = -1;
	socket.events = 0;
	socket.revents = 0;
}

void Server::expandPoll() {
	pollfd	*tmp;
	pollfd	*tmp2;
	int 	i;

	fds_size += 100;
	tmp = new pollfd[fds_size];
	tmp[fds_size - 1].fd = -2;
	for(i = 0; fds[i].fd != -2; i++)
	{
		tmp[i].fd = fds[i].fd;
		tmp[i].events = fds[i].events;
	}
	for(int a = i; tmp[a].fd != -2; a++)
	{
		tmp[a].fd = -1;
	}
	tmp2 = fds;
	fds = tmp;
	delete []tmp2;
}

char* Server::strjoin(const char *s1, const char *s2) {
	int 	i;
	int 	i2;
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