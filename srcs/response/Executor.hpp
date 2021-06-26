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

class Executor {
	public:
		Executor(Config &configParser, RequestParser &requestParser);
		bool receiveRequest(pollfd &sock); // header
		bool executeMethod(); // POST GET DELETE
		bool sendResponse(pollfd &sock); // send response

	private:
		int						_error;
		char					*_data;
		Config					&_configParser;
		RequestParser			&_requestParser;

		// will remove
		bool		collectHeader(const char *data);
		bool		collectBody(const char *data);
		bool 		collectRequest(const char *data, bool &next_step);
		const char 	*strjoin(const char* s1, const char *s2) const;

		size_t 					_content_length; // 0 if absent
		bool 					_is_chunked;
		std::list<std::string>	_header_fields;
		size_t 					_header_size;
		std::stringstream 		_body;

		//core
		bool 		methodDelete();
		bool 		methodPost();
		bool 		methodGet();
};

#endif
