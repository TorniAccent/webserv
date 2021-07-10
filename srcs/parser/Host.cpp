#include "ConfigParser.hpp"

static const std::string throwIn(std::fstream &fin, const char *ref) {
	std::string s = noComment(fin);
	if (ref && s != ref) {
		throw ref;
	}
	return s;
}

Config::Host::Host(std::fstream &fin) {
	bool prev = false;
	throwIn(fin, "server");
	throwIn(fin, "{");
	/// host port
	{
		throwIn(fin, "host");
		std::string host;
		noSemi(fin, host);
		throwIn(fin, "port");
		std::string port;
		noSemi(fin, port);
		address = std::pair<std::string, int>(host, std::stoi(port));
	}
	/// index
	{
		throwIn(fin, "index");
		for (std::string index; ; ) {
			bool b = noSemi(fin, index);
			indexes.push_back(index);
			if (b)
				break;
		}
	}
	/// limit
	{ //todo: make this optional
		throwIn(fin, "limit");
		std::string lim;
		noSemi(fin, lim);
		limit = std::stoi(lim);
	}
	/// error_pages
	{
		throwIn(fin, "error_pages");
		std::string err;
		noSemi(fin, err);
		errorPages = err;
	}
	/// \n
	{
		std::string tmp;
		std::getline(fin, tmp);
	}
	/// location
	{
		for (std::string tmp; std::getline(fin, tmp), tmp != "}"; ) {
			locations.push_back(Location(fin));
		}
	}
}

std::vector<std::string> Config::Host::getServerNames() const {
	std::vector<std::string> tmp;
	tmp.push_back("MailRu");
	tmp.push_back("Yandex");
	tmp.push_back("test");
	return tmp;
}

std::pair<std::string, int> Config::Host::getAddress() const {
	return address;
}

std::vector<std::string> Config::Host::getIndexes() const {
	return indexes;
}

size_t Config::Host::getLimitBodySize() const {
	return limit;
}

std::string Config::Host::getErrorPages() const {
	return errorPages;
}

std::vector<Config::Host::Location> Config::Host::getLocations() const {
	return locations;
}
