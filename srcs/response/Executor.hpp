/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Executor.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/26 16:27:54 by swquinc           #+#    #+#             */
/*   Updated: 2021/06/26 16:27:55 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_HPP
# define EXECUTOR_HPP
# include "ConfigParser.hpp"
# include "RequestParser.hpp"
# include <iostream>
# include <sys/socket.h>
# include <poll.h>
# include <vector>
# include <sstream>
# include <string>
# include <list>
# include <unistd.h>
# include <fcntl.h>
# include <fstream>
# include <sys/stat.h>
# include <sys/wait.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# define MAX_HEADER_SIZE 8192
# define EXECUTABLE_FILE 137
# define FILE 666
# define DIRECTORY 444

class Executor {
	public:
		Executor(Config &configParser, RequestParser &requestParser);
		bool receiveRequest(pollfd &sock); // header
		bool executeMethod(); // POST GET DELETE
		bool sendResponse(pollfd &sock); // send response

	private:
		typedef Config::Host::Location Location;

		int						_error;
		pollfd					*_sock;
		size_t					_max_body_size;
		char					*_data;
		Config					&_configParser;
		RequestParser			&_requestParser;
		size_t 					_header_size;
		std::stringstream 		_body; // postBody
		std::stringstream 		_responseBody;
		int 					_post;
		Location 				*_location;

		//add
		bool 		splitHeader(std::vector<std::string> &main_strings, std::string &tmp);
		int			selectLocation(std::string uri);
		int 		getError();
		int 		readChunkedBody();
		int 		readFixBody(size_t	content_length);
		int 		selectFunction(std::string content_type);
		int			postMultiPartFD();
		int 		postApplicationXWFU();
		int 		writeToFile(std::string filename, char *data, size_t size);
		int 		getResourceType(std::string uri);
		int 		toOctal(int decimal);
		std::string getInterpreter(std::string uri);
		char 		**assembleEnv();
		//core
		bool 		executeCGI(std::string method);
		bool 		methodDelete(int obj);
		bool 		methodPost(int obj);
		bool 		methodGet(int obj);
};

#endif
