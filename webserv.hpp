/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 19:17:13 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/05 14:04:04 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP
# include <stdio.h>
# include <iostream>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
// fcntl - for non-blocking connection
# include <arpa/inet.h>
// htons, htonl, ntohs, ntohl - for convert from little-endian to big-endian, and in reverse
// inet_addr - takes ip-address in char*, and return it as 4 byte number. We should assign it to sin_addr.s_addr var.
// return bits in network order.
# include <sys/socket.h>
// socket - create socket
// bind - attach to socket ip-address, port, and address family (семейство адресов).
// connect - send request to connection.
// setsockopt - time to wait receive, tcp-port
// getsockname
// listen - open socket to listening
// accept - wait request from client, open connection if request received.
// send - like write
// recv - like read
# include <sys/select.h> // select - for non-blocking connection
# include <poll.h> // poll - for non-blocking connection (recommended)
# include <sys/types.h> // kqueue - for non-blocking connection
# include <sys/time.h> // kqueue - for non-blocking connection
# include <sys/un.h>
# include <sys/stat.h>
//# include "gnl/get_next_line.h"
# include <pthread.h>



#endif
