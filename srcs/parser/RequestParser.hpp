#ifndef WEBSERV_REQUEST_PARSER_HPP
#define WEBSERV_REQUEST_PARSER_HPP

# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <cstring>
# include <list>
# include <cstdlib>
# include "../parser/ConfigParser.hpp"

using std::cout;

# define npos std::string::npos

class RequestParser {
public:
	explicit RequestParser(Config const &config);

	void 		parseHeader(std::vector<std::string> &header);

	std::string getMethod() const;
	std::string getURI() const;
	std::string getQueryString() const;
	int 		getError() const;

	std::pair<std::string, int> getHost() const;
	size_t		getContentLength() const;
	std::string getContentType() const;
	std::string	getAccept() const;
	std::string getAcceptCharset() const;
	std::string getAcceptEncoding() const;
	std::string getAcceptLanguage() const;
	std::string getUserAgent() const;
	std::string getRelativePath() const;
	std::string getAbsolutePath() const;
	bool getConnection() const;


private:
	int							_error;
	std::map<std::string,
			 std::string>		_headers;

	Config						_config;

	std::string					_method;
	std::string 				_uri;
	std::string 				_query_string;
	std::pair<std::string, int>	_host;

	size_t 						_content_length;
	std::string					_content_type;
	std::string					_accept;
	std::string 				_charset;
	std::string 				_encoding;
	std::string 				_language;
	std::string 				_user_agent;
	bool						_connection;

	std::string 				_web;
	std::string 				_relative_path;;
	std::string 				_absolute_path;
//	std::string 				_cgi_pass;

//	std::string 				_connection;
//	std::string 				_boundary;

	RequestParser();
	RequestParser(RequestParser const &copy);

};

#endif //WEBSERV_REQUEST_PARSER_HPP
