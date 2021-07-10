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
# include "../parser/ConfigParser.hpp"
# include <sys/socket.h>
# include <poll.h>
# include <vector>
# include <sstream>
# include <string>
# include <list>
# include <unistd.h>

class RequestParser
{
	private:

		Config						_config;
		std::string					_method;
		std::string 				_uri;
		std::string 				_connection;
		std::list<std::string>		_accept;
		std::string					_media_type;
		std::string 				_charset;
		size_t 						_content_length;
		std::string 				_boundary;
		std::pair<std::string, int>	_host;

	public:
		RequestParser();
		explicit RequestParser(Config &configParser);
		RequestParser &operator=(const RequestParser &r);
		~RequestParser();

		bool 		parseHeader(std::vector<std::string> &header);
		std::string getMethod(void) const;
		std::string getContentType(void) const;
		std::string getURI(void) const;
		int 		getError(void) const;
		size_t 		getContentLength() const;
		std::string getBoundary(void) const;
		std::pair<std::string, int> getHost(void) const; // host and port из request

};

//struct Request {
//	std::map<std::string, std::string> headers;
//	int parseHeader(std::vector<std::string>);
//	std::string find(std::string key);
//};

//typedef headers.find tt
/// tt("key") == "value";  tt("content_type")

#endif