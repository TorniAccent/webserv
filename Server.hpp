/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/06 14:41:49 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/06 14:41:53 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
//# include "webserv.hpp"
# include <stdio.h>
# include <iostream>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/select.h> // select - for non-blocking connection
# include <poll.h> // poll - for non-blocking connection (recommended)
# include <sys/types.h> // kqueue - for non-blocking connection
# include <sys/time.h> // kqueue - for non-blocking connection
# include <sys/un.h>
# include <sys/stat.h>
# include <vector>
# include <string>
# include <sstream>
# include <fstream>
# include <map>
# include "CParser.hpp"
# include "ResponseMaker.hpp"
# define BUFFER_SIZE 1500

class Server {
	public:
		// User interface
		Server(char *config);
		~Server();
		void start();
	private:
		// Core
		typedef std::vector<Address>	Listen;
		std::map<int, in_addr_t>		ips;
		pollfd							*fds;
		nfds_t 							nfds;
		int 							fds_size;
		Parser							configParser;

		void 			addSocket(int sock, short event);
		void 			deleteSocket(pollfd &fd);
		void 			expandPoll();
		const char		*getRequest(int fd);
		void 			startListen(Listen listen);
		void 			acceptConnection(pollfd lsocket, int i);
		char 			*strjoin(const char* s1, const char *s2);
};

#endif