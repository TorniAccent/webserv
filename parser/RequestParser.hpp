#ifndef WEBSERV_REQUEST_PARSER_HPP
#define WEBSERV_REQUEST_PARSER_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <cstring>

using std::cout;

# define npos std::string::npos

std::string					noComment(std::fstream &fin);
bool						noSemi(std::fstream &fin, std::string &tmp);
size_t						findField(std::string &str, const char *ref);
std::vector<std::string>	split(std::string &str);

struct Request {
	std::map<std::string, std::string> headers;
	int error;
	std::string method;
	std::string uri;

	Request(std::vector<std::string> lines);
private:
	Request();
	Request(Request const &copy);
};

#endif //WEBSERV_REQUEST_PARSER_HPP
