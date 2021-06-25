/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/19 16:43:26 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/19 16:43:27 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP
# include <iostream>
# include "../parser/CParser.hpp"
# include <sys/socket.h>
# include <poll.h>
# include <vector>
# include <string>
# include <unistd.h>

class RequestParser
{
	private:
		ConfigParser	_config;
		std::string		method;
		std::vector<std::string> _header_fields;
		char			*_data;
		const char 	*strjoin(const char* s1, const char *s2) const;
		bool		collectHeader(const char *data);
		bool		collectBody(const char *data);
	public:
		RequestParser();
		explicit RequestParser(ConfigParser &configParser);
		RequestParser &operator=(const RequestParser &r);
		~RequestParser();
		bool recieve(pollfd sock);
		std::string getMethod(void) const;
		std::string getData(void) const;
};

#endif