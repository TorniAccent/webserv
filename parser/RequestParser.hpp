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

std::string					noComment(std::fstream &fin);
bool						noSemi(std::fstream &fin, std::string &tmp);
size_t						findField(std::string &str, const char *ref);
std::vector<std::string>	split(std::string &str);

class RequestParser {
public:
	RequestParser(std::vector<std::string> lines);
//	bool 		parseHeader(std::vector<std::string> &header);
//	std::string getMethod(void) const;
//	std::string getContentType(void) const;
//	std::string getURI(void) const;
//	int 		getError(void) const;
//	size_t 		getContentLength() const;
//	std::string getBoundary(void) const;
//	std::pair<std::string, int> getHost(void) const;

private:
	int _error;
	std::map<std::string, std::string> _headers;

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

	RequestParser();
	RequestParser(RequestParser const &copy);

};

#endif //WEBSERV_REQUEST_PARSER_HPP
