#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-auto"
#include "RequestParser.hpp"

/// 503 - "Retry-After" field found. thru.
/// 414 - 'uri' is bigger then the size written in the config. 1st.
/// 400 - just any syntax error.
/// 401 - "Authorization" field not found, or not valid. thru.
/// 403 - * we are not allowed to go to certain directory
/// 405 - not implemented methods, except HEAD. 1st.
/// 415 - "Content-Type" or "Content-Encoding" fields found (other -*). thru.
/// 411 - "Content-Length" field not found thru.
/// 413 - * useful load is too large
/// ---

/// 404
/// 410
/// 429


static std::pair<std::string, std::string> colon_split(std::string const &line) {
	size_t tmp;

	if ((tmp = line.find(':', 0)) == npos)
		return std::pair<std::string, std::string>();
	std::string first = line.substr(0, tmp);

	if ((tmp = line.find_first_not_of(' ', tmp + 1)) == npos)
		return std::pair<std::string, std::string>();
	std::string second = line.substr(tmp, line.length() - tmp);

	return std::pair<std::string, std::string>(first, second);
}

RequestParser::RequestParser(std::vector<std::string> lines) : _error(0) {
	size_t tmp = 0;
	std::string str;

/// parsing the 1st line
	if ((tmp = lines[0].find(' ', tmp)) == npos)
		_error = 400;
	str = lines[0].substr(0, tmp);
	if (str != "GET" && str != "POST" && str != "DELETE")
		_error ? 0 : _error = 405;
	_method = str;

	if ((tmp = lines[0].find_first_not_of(' ', tmp)) == npos)
		_error = 400;
	str = lines[0].substr(tmp, lines[0].find(' ', tmp) - tmp);
	if (str.length() > 4096)
		_error ? 0 : _error = 414;
	_uri = str;

	tmp = lines[0].find(' ', tmp);
	if ((tmp = lines[0].find_first_not_of(' ', tmp)) == npos)
		_error = 400;
	str = lines[0].substr(tmp, lines[0].length());
	if (str != "HTTP/1.1")
		_error = 400;

/// the map filling
	for (std::vector<std::string>::iterator it = lines.begin() + 1;
		 !_error && it != lines.end(); it++) {
		std::pair<std::string, std::string> tmp;
		if ((tmp = colon_split(*it)) == std::pair<std::string, std::string>())
			_error = 400;
		_headers.insert(tmp);
	}

/// the fields filling

}

//std::string		RequestParser::getBoundary() const {
//	return (_boundary);
//}
//
//std::string		RequestParser::getMethod() const {
//	return (_method);
//}
//
//std::string 	RequestParser::getURI() const {
//	return ("/images/favicon.ico");
//}
//
//std::string 	RequestParser::getContentType() const {
//	return ("multipart/form-data");
//}
//
//int 			RequestParser::getError() const {
//	return (400);
//}
//
//size_t			RequestParser::getContentLength() const {
//	return (_content_length);
//}
//
////RequestParser&	RequestParser::operator=(const RequestParser &r) {
////	return (*this);
////}
//
//std::pair<std::string, int> RequestParser::getHost() const {
//	return (std::make_pair("MailRu", 2021));
//}



#pragma clang diagnostic pop