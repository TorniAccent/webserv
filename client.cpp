/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 19:27:48 by swquinc           #+#    #+#             */
/*   Updated: 2021/07/02 19:29:33 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

int main()
{
	std::stringstream request;



	request << "POST /test.html HTTP/1.1\r\n"
			<< "Host: foo.com\r\n"
			<< "Content-Type: application/x-www-form-urlencoded\r\n"
			<< "Content-Length: 13\r\n\r\n"
			<< "say=Hi&to=Mom\r\n";

	return (0);
}
