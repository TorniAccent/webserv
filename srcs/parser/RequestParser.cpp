#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-auto"
#include "RequestParser.hpp"
#include <map>

// 503 - "Retry-After" field found. thru.
/// 414 - 'uri' is bigger then the size written in the config. 1st. v
/// 400 - just any syntax error. v
/// 405 - not implemented methods. 1st. v
/// 406 - "Accept-Charset" field not found thru. v
/// 411 - "Content-Length" field not found thru. v
// 403 - * we are not allowed to go to certain directory
// 415 - "Content-Type" (other -*) or "Content-Encoding" fields found. thru.
// 413 - * useful load is too large
/// ---

/// 404
/// 410
/// 429


RequestParser::RequestParser(Config const &config)
	: _error(200), _connection(false), _config(config) {
}

void RequestParser::setHost(std::string host) {
	_host.first = host;
}

void RequestParser::parseHeader(std::vector<std::string> &lines) {
/// parsing the 1st line
	{
		size_t pos = 0;
		std::string str;

		if ((pos = lines[0].find(' ', pos)) == npos)
			_error = 4005;
		str = lines[0].substr(0, pos);
		if (str != "GET" && str != "POST" && str != "DELETE")
			_error ? 0 : _error = 405;
		_method = str;

		if ((pos = lines[0].find_first_not_of(' ', pos)) == npos)
			_error = 4004;
		str = lines[0].substr(pos, lines[0].find(' ', pos) - pos);
		if (str.length() > 4096)
			_error ? 0 : _error = 414;
		{
			size_t tmp;
			if ((tmp = str.find('?', 0)) == npos)
				_uri = str;
			else {
				_uri = str.substr(0, tmp);
				_query_string = str.substr(tmp + 1, str.length());
			}
		}

		pos = lines[0].find(' ', pos);
		if ((pos = lines[0].find_first_not_of(' ', pos)) == npos)
			_error = 4003;
		str = lines[0].substr(pos, lines[0].length());
		if (str != "HTTP/1.1")
			_error = 4002;
	}

/// the map filling
	{
		for (std::vector<std::string>::iterator it = lines.begin() + 1;
			 !_error && it != lines.end(); it++) {
			std::pair<std::string, std::string> tmp;
			if ((tmp = colon_split(*it)) == std::pair<std::string, std::string>())
				_error = 4001;
			_headers.insert(tmp);
		}
	}

/// the fields filling
	{
		std::map<std::string, std::string>::iterator tmap;

		if ((tmap = _headers.find("Host")) != _headers.end()) {
			std::pair<std::string, std::string> tmpair =
				colon_split(tmap->second);
			_host.second = stoi(tmpair.second);
			_server_name = tmpair.first;
		}

		if ((tmap = _headers.find("Content-Length")) != _headers.end()) {
			_content_length = stoll(tmap->second);
		} else {
//			_error = 411;
//			return;
		}

		if ((tmap = _headers.find("Content-Type")) != _headers.end()) {
			_content_type = tmap->second;
		}

		if ((tmap = _headers.find("Accept")) != _headers.end()) {
			_user_agent = tmap->second;
		} else {
//			_error = 406;
//			return;
		}

		if ((tmap = _headers.find("Accept-Charset")) != _headers.end()) {
			_charset = tmap->second;
		} else {
//			_error = 406;
//			return;
		}

		if ((tmap = _headers.find("Accept-Encoding")) != _headers.end()) {
			_encoding = tmap->second;
		} else {
//			_error = 406;
//			return;
		}

		if ((tmap = _headers.find("Accept-Language")) != _headers.end()) {
			_language = tmap->second;
		} else {
//			_error = 406;
//			return;
		}

		if ((tmap = _headers.find("Connection")) != _headers.end()) {
			_connection = tmap->second == "keep-alive";
		} else {
//			_error = 400;
//			return;
		}



	}

/// getPath
	{
		_web = _uri.substr(0, _uri.rfind('/') + 1);

		std::vector<Config::Host> host = _config.getHosts();
		std::vector<Config::Host>::iterator ith = host.begin();
		for (; ith != host.end(); ith++) {
			std::cout << ith->getAddress().first << ith->getAddress().second << "\n"
			<< _host.first << _host.second << "\n--=-=--\n";
//			if (ith->getAddress().second == _host.second &&
//				(ith->getAddress().first == _host.first ||
//				 "localhost" == _host.first))
			if (ith->getAddress() == _host && ith->getServerName() == _server_name)
				break;
		}
		if (ith != host.end()) {
			std::vector<Config::Host::Location> locations = ith->getLocations();
			std::vector<Config::Host::Location>::iterator itl = locations.begin();
			for (; itl != locations.end() && itl->getWeb() != _web; itl++);
			if (itl != locations.end()) {
				std::string uri = _uri;
				_relative_path = uri.replace(0, 1, itl->getRoot());
				_relative_path = _relative_path.replace(0, 1, "");
				char *tmp;
				if ((tmp = realpath(_relative_path.c_str(), NULL)) == NULL)
//					throw const_cast<const char*>(std::strerror(errno));
					_error = 404;
				_absolute_path = tmp;
				if (!tmp)
					free(tmp);
//				char tmp[1000];
//				realpath(_relative_path.c_str(), tmp);
//				_absolute_path = tmp;
			} else {
				std::string uri = _uri;
				_relative_path = uri.replace(0, 1, "/root/");
				_relative_path = _relative_path.replace(0, 1, "");

			}
		}
			/// раскоментировать, если понадобится венуть путь до cgi
//			else {
//				for (; itl != locations.end() && itl->getCGI() != _web; itl++);
//				if (itl != locations.end()) {
//					_cgi_pass = itl->getRoot();
//				}
//			}
	}
}


std::string	RequestParser::getMethod() const {
	return (_method);
}

std::string RequestParser::getURI() const {
	return (_uri);
}

std::string RequestParser::getQueryString() const {
	return (_query_string);
}

int RequestParser::getError() const {
	return (_error);
}

////
std::string RequestParser::getHost() const {
	std::stringstream ss;
	ss << _host.second;
	std::string tmp;
	ss >> tmp;
	return (_host.first + ":" + tmp);
}

std::string RequestParser::getServerName() const {
	return (_server_name);
}

size_t	RequestParser::getContentLength() const {
	return (_content_length);
}

std::string RequestParser::getContentType() const {
	return (_content_type);
}

std::string	RequestParser::getAccept() const {
	return (_accept);
}

std::string RequestParser::getAcceptCharset() const {
	return (_charset);
}

std::string RequestParser::getAcceptEncoding() const {
	return (_encoding);
}

std::string RequestParser::getAcceptLanguage() const {
	return (_language);
}

std::string RequestParser::getUserAgent() const {
	return (_user_agent);
}

bool RequestParser::getConnection() const {
	return (_connection);
}

std::string RequestParser::getRelativePath() const {
	return (_relative_path);
}

std::string RequestParser::getAbsolutePath() const {
	return (_absolute_path);
}



#pragma clang diagnostic pop