#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-auto"
#include "RequestParser.hpp"

/// 503 - "Retry-After" field found. thru.
/// 414 - 'request-line' is bigger then the size written in the config. 1st.
/// 400 - just any syntax error.
/// 401 - "Authorization" field not found, or not valid. thru.
/// 403 - * we are not allowed to go to certain directory
/// 415 - "Content-Type" or "Content-Encoding" fields found. thru.
/// 413 - * useful load is too large
/// 405 - HEAD request. 1st.
/// 501 - not implemented methods, except HEAD. 1st.
/// ---

/// 404
/// 410
/// 411
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

Request::Request(std::vector<std::string> lines) : error(0) {
	size_t tmp;

	if ((tmp = lines[0].find(' ', 0)) == npos)
		error = 1;

	if ()
	for (std::vector<std::string>::iterator it = lines.begin() + 1;
								!error && it != lines.end(); it++) {
		if (colon_split(*it) == std::pair<std::string, std::string>())
			error = 400;
	}

}

//int Request::fill() {
//	int flag = 0;
//	/// 503
//	for (std::vector<std::string>::iterator it = lines.begin(); it != lines.end(); it++) {
//		if (it->find("Retry-After") != npos)
//			return 503;
//		if (it->find("Authorization") != npos)
//			flag |= 1;
//		if (it->find("Content-Type") != npos || it->find("Content-Encoding") != npos)
//			flag |= 2;
//	}
//	flag ^= 1;
//	std::vector<std::string> vec;
//	/// 1st line
//	{
//		vec = split(lines[0]);
//		if (vec[1].length() > )
//
//		_method = vec[0];
//		_requestURI = vec[1];
//		_httpVersion = vec[2];
//	}
//}


#pragma clang diagnostic pop