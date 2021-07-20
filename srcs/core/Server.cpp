/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchantel <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/06 14:41:58 by mchantel           #+#    #+#             */
/*   Updated: 2021/06/06 14:41:59 by mchantel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *config): _configParser(config), _listen() {
	std::vector<Config::Host> hosts;
	_fds = new pollfd[1];
	_fds[0].fd = VOID_POLLFD;
	_fds[0].events = 0;
	_fds_size = 0;
	_nfds = 0;
	hosts = _configParser.getHosts();
	std::vector<Config::Host>::iterator it;

	it = hosts.begin();
	while (it != hosts.end())
	{
		_listen.push_back(it->getAddress());
		it++;
	}
}

Server::~Server() {
	delete []_fds;
}

void Server::start() {
	int	events;

	std::cout << paintString("\n[Running server]", BOLD, BWHITE, 0) << std::endl;
	std::cout << paintString(" Parsing   -> [OK]", BOLD, GREEN, 0) << std::endl;

	startListen(_listen);

	std::cout << paintString("[Success]", BOLD, BWHITE, 0) << '\n' <<std::endl;
	std::cout << paintString("[Waiting to events]", BOLD, BWHITE, 0) << std::endl;

	while(true) {
		events = poll(_fds, _nfds, 0);
		if (events == -1)
			throw std::strerror(errno);
		else if (events == 0)
			continue ;
		findEvent(events);
	}
}

void Server::findEvent(int events) {
	(void)events;

	for (int i = 0; (i != _fds_size); i++) {
		if (_fds[i].revents == NEW_CONNECTION)
			acceptConnection(_fds[i], i);
		else if (_fds[i].revents == REQUEST_RESPONSE)
			recvRequest_sendResponse(_fds[i]);
		else if (_fds[i].revents >= POLLHUP && _fds[i].revents <= 17)
			deleteSocket(_fds[i]);
		else if (_fds[i].revents == POLLERR)
			throw "POLLERR detected!";
		else if (_fds[i].revents == POLLNVAL)
			throw "Файловый дескриптор не открыт";
		else {
			if (_fds[i].revents != 0 && _fds[i].revents != 4)
				std::cout << _fds[i].revents << "[ALERT]"<< _fds[i].fd  << " " << _fds[i].events << std::endl;
		}
	}
}

void Server::recvRequest_sendResponse(pollfd &sock) {
	RequestParser	requestParser(_configParser);
	Executor		executor(_configParser, requestParser);
	std::string 	response;
	bool 			isSuccess;

	//RequestParser ResponseMaker >> Executor

//	executor.receiveRequest(sock); //bool
//	executor.executeMethod(); //bool
//	executor.sendResponse(sock); //bool

	isSuccess = executor.receiveRequest(sock);

//	std::cout << requestParser.getRelativePath() << std::endl;
	executor.executeMethod();
//	if (!isSuccess) {
//		close(sock.fd);
//		sock.fd = VOID_POLLFD;
//		sock.revents = 0;
//		sock.events = 0;
//		return ;
//	}
	executor.sendResponse(sock);
//	if (!requestParser.getConnection()) {
//		close(sock.fd);
//		sock.fd = VOID_POLLFD;
//		sock.revents = 0;
//		sock.events = 0;
//	}
}

void Server::startListen(Listen &listen) {
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
		inet.sin_port = htons(it->second); // переводим значение порта в сетевой порядок следования байт
		inet.sin_addr.s_addr = inet_addr(it->first.c_str()); // переводит char* в какое-то число, необходимо для bind();

		base = reinterpret_cast<struct sockaddr *>(&inet);
		if (bind(sock, base, sizeof(inet)) == -1)
			throw std::strerror(errno);
		if (::listen(sock, 25) == -1)
			throw std::strerror(errno);
		addSocket(sock, NEW_CONNECTION);
	}
	std::cout << "\033[1;32m[OK]\033[0m" << std::endl;
}


void Server::acceptConnection(pollfd lsocket, int i) {
	int					new_client;
	socklen_t			len;
	struct sockaddr		*base;
	socklen_t			opt;
	struct sockaddr_in	inet = {};
	struct timeval timeout;
	timeout.tv_sec = 5;
	timeout.tv_usec = 0;
	static int avvv;

	lsocket.revents = 0;
	base = reinterpret_cast<struct sockaddr *>(&inet);
	len = sizeof(inet);
	opt = 1;

	new_client = accept(lsocket.fd, base, &len);

//	std::cout << "\033[0;93mКлиент " <<  new_client  << " привет!\033[0m" << std::endl;
	if (new_client == -1)
		throw std::strerror(errno);
	if (fcntl(new_client, F_SETFL, O_NONBLOCK) < 0)
		throw std::strerror(errno);
//	std::cout << _fds_size << std::endl;
//	setsockopt(new_client, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // от залипания tcp-порта.
//	std::cout   << paintString("================================", 0, BWHITE, 0) << std::endl;
//	std::cout	<< paintString("[NEW_CLIENT]", 1, GREEN, 0)
//				<< "\nConnected to " << lsocket.fd << " listening socket"
//				<< "\nfd		 = " << new_client << "(" << i << ")"
//				<< "\nport		 = " << inet.sin_port
//				<< "\nip		 = " << inet.sin_addr.s_addr
//				<< "\nIPv4		 = " << inet_ntoa(inet.sin_addr)
//				<< std::endl;
//	std::cout   << paintString("================================", 0, BWHITE, 0) << std::endl;
	addSocket(new_client, REQUEST_RESPONSE);
}

void Server::addSocket(int sock, short event) {
	int i;

//	if (_nfds < sock)
//		_nfds = sock;
	if (_fds_size == 0) {
		expandPoll();
		addSocket(sock, event);
		return ;
	}
	_nfds = _fds_size;
//	_nfds = sock;
	for(i = 0; i != _fds_size; i++)
	{
		if (_fds[i].fd == VOID_POLLFD)
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
//	std::cout << "\033[0;93mКлиент " <<  socket.fd  << " разорвал соединение!\033[0m" << std::endl;
//	std::cout << socket.events << std::endl;
//	std::cout << socket.revents << std::endl;
//	std::cout << _fds_size << std::endl;

	close(socket.fd);
	socket.fd = VOID_POLLFD;
	socket.events = 0;
	socket.revents = 0;
}

void Server::expandPoll() {
	pollfd	*tmp;
	pollfd	*tmp2;
	int 	i;

	_fds_size += 100;
	tmp = new pollfd[_fds_size];
//	tmp[_fds_size - 1].fd = END_OF_POLLFD_ARRAY;
	for(i = 0; i != _fds_size - 100; i++)
	{
		tmp[i].fd = _fds[i].fd;
		tmp[i].events = _fds[i].events;
		tmp[i].revents = 0;
	}
	for(int a = i; a != _fds_size; a++)
	{
		tmp[a].fd = VOID_POLLFD;
		tmp[a].events = 0;
		tmp[a].revents = 0;
	}
	tmp2 = _fds;
	_fds = tmp;
	delete []tmp2;
	tmp2 = NULL;
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