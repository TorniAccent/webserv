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
# include <cstdio>
# include <iostream>
# include <unistd.h>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
# include <arpa/inet.h>
# include <sys/socket.h>
# include <sys/select.h> // select - for non-blocking connection - портируемый. Но старый и неудобный. Скорость - низкая.
# include <poll.h> // poll - for non-blocking connection (recommended) - портируемый. Старый, удобнее select. Скорость - низкая.
# include <sys/types.h> // kqueue - for non-blocking connection - Очень удобный, не портируемый (BSD). Скорость - высокая.
# include <ctime> // kqueue - for non-blocking connection
# include <sys/un.h>
# include <sys/stat.h>
# include <vector>
# include <string>
# include <sstream>
# include <fstream>
# include <map>
# include "../parser/CParser.hpp"
# include "../response/ResponseMaker.hpp"
# include <netinet/in.h>
//# define BUFFER_SIZE		1500
# define NEW_CONNECTION		POLLRDNORM
# define REQUEST_RESPONSE	(POLLIN | POLLOUT)
# define BLACK		30
# define RED		31
# define GREEN		32
# define YELLOW		33
# define BLUE		34
# define MAGENTA	35
# define CYAN		36
# define WHITE		37
# define BBLACK		90
# define BRED		91
# define BGREEN		92
# define BYELLOW	93
# define BBLUE		94
# define BMAGENTA	95
# define BCYAN		96
# define BWHITE		97
# define BOLD		1

class Server {
	public:
		// User interface
		explicit Server(char *config);
		~Server();
		void start();
	private:
		// Core
		typedef std::vector<Address>	Listen;

		pollfd							*_fds;
		nfds_t 							_nfds;
		int 							_fds_size;
		ConfigParser					_configParser;

		void 			addSocket(int sock, short event);
		void 			deleteSocket(pollfd &fd);
		void 			expandPoll();
		void			findEvent(int events);
		const char		*getRequest(int fd);
		void			recvRequest_sendResponse(pollfd &sock);
		void 			startListen(Listen listen);
		void 			acceptConnection(pollfd lsocket, int i);
		const char 		*strjoin(const char* s1, const char *s2) const;

};

std::string 	paintString(const std::string &string, int format, int color, int background);

#endif