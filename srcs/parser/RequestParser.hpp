#ifndef WEBSERV_REQUEST_PARSER_HPP
#define WEBSERV_REQUEST_PARSER_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <cstring>
# include <list>
# include "../parser/ConfigParser.hpp"

using std::cout;

# define npos std::string::npos

class RequestParser {
public:
	explicit	RequestParser(Config &config);
	bool 		parseHeader(std::vector<std::string> &header);

	std::string getMethod() const;
	std::string getURI() const;
	int 		getError() const;

	std::pair<std::string, int> getHost() const;
	size_t 		getContentLength() const;
	std::string getContentType() const;

//	std::string getBoundary(void) const;


private:
	int _error;
	std::map<std::string, std::string> _headers;

	Config						_config;

	std::string					_method;
	std::string 				_uri;
	std::pair<std::string, int>	_host;
	size_t 						_content_length;
	std::string					_content_type;

	std::string 				_connection;
	std::list<std::string>		_accept;
	std::string 				_charset;
	std::string 				_boundary;

	RequestParser();
	RequestParser(RequestParser const &copy);

};

#endif //WEBSERV_REQUEST_PARSER_HPP
