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

Server::Server(char *config) {
	struct sockaddr *base;
	int opt;
	int sock;
	struct sockaddr_in	inet;
	fds = new pollfd[1];
	fds[0].fd = -2;
	fds_size = 0;

	std::cout << "\e[1;97mRunning server...\e[0m" << std::endl;
	Parser parser(config);
	Listen listen = parser.getListen();

	std::cout << "\e[1;32m Listening -> \e[0m";
	for (Listen::iterator it = listen.begin(); it != listen.end(); it++) {
		sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP); // Семейств сокета, тип потокового взаимодействия(тип передачи данных), протокол.
		if (sock == -1) {
			throw std::strerror(errno);
		}
		fcntl(sock, F_SETFL, O_NONBLOCK); // от блокирования сокета.
		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // от залипания tcp-порта.

		inet.sin_family = PF_INET; // Семейство сокета.
		inet.sin_port = htons(it->port); // переводим значение порта в сетевой порядок следования байт
		inet.sin_addr.s_addr = inet_addr(it->ip.c_str()); // переводит char* в какое-то число, необходимо для bind();

		base = reinterpret_cast<struct sockaddr *>(&inet);
		if (bind(sock, base, sizeof(inet)) == -1) {
			throw std::strerror(errno);
		}
		if (::listen(sock, 5) == -1) {
			throw std::strerror(errno);
		}

		addSocket(sock, POLLPRI);
	}
	std::cout << "\e[1;32m[OK]\e[0m" << std::endl;
}

Server::~Server() {
	delete []fds;
}

void Server::start() {
	int event;
	struct sockaddr_in base;
	socklen_t len;
	int new_client;
	std::string rec;
	char buffer[BUFFER_SIZE];
	ssize_t check;

	std::cout << "\e[1;32m Polling   -> \e[0m";
	while(1) {
		event = poll(fds, nfds + 1, -1);
		if (event == -1)
			throw std::strerror(errno);
		else if (event == 0)
			throw "poll timeout"; // поменять на continue для неблокируемого опроса.
		for(int i = 0; event != 0; i++)
		{
			if (fds[i].revents == POLLPRI)
			{
				fds[i].revents = 0;
				new_client = accept(fds[i].fd, NULL, NULL);
				if (new_client == -1)
					throw std::strerror(errno);
				addSocket(new_client, POLLRDBAND);

			}
			else if (fds[i].revents == POLLRDBAND)
			{
				fds[i].revents = 0;
				if (recv(fds[i].fd, &buffer, BUFFER_SIZE, 0) == -1)
					throw "recv has broken";
				if (i % 2 == 0)
					rec = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 20\n\nMessage from server!";
				else
					rec = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 20\n\nKicking from server!";
				send(new_client, rec.c_str(), rec.length(), 0);
			}
			else if (fds[i].revents == POLLHUP)
			{
				close(fds[i].fd);
				fds[i].fd = -1;
				fds[i].revents = 0;
			}
			else if (fds[i].revents == POLLERR)
			{
				throw "POLLERR detected!";
			}
			else if (fds[i].revents == POLLNVAL)
			{
				throw "Файловый дескриптор не открыт";
			}
			event--;
		}
		std::cout << "\e[1;32m[OK]\e[0m" << std::endl;
	}
//	if (check == -1)
//		throw "какая-то ошибка от recv";
//	else if (check == 0)
//		throw "Клиент сбросил соединение";
//	rec[check] = '\0';
//	std::cout << buffer;

//	rec = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 20\n\nMessage from server!";
//	send(new_client, rec.c_str(), rec.length(), 0);

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
			return ;
		}
	}
	expandPoll();
	addSocket(sock, event);
}

void Server::expandPoll() {
	pollfd	*tmp;
	int 	i;

	fds_size += 100;
	tmp = new pollfd[fds_size];
	tmp[fds_size - 1].fd = -2;
	for(i = 0; fds[i].fd != -2; i++)
	{
		tmp[i] = fds[i];
	}
	for(int a = i; tmp[a].fd != -2; a++)
	{

		tmp[a].fd = -1;
	}
	delete []fds;
	fds = tmp;
}