#include "ConfigParser.hpp"

static std::string noComment(std::fstream &fin) {
	std::string s;
	for (; fin >> s, s == "#"; )
		for (; fin.get() != '\n'; );
	return s;
}

static const std::string throwIn(std::fstream &fin, const char *ref) {
	std::string s = noComment(fin);
	if (!ref && s != ref) {
		throw ref;
	}
	return s;
}

static bool noSemi(std::fstream &fin, std::string &tmp) {
	fin >> tmp;
	if (tmp.find(';')) {
		tmp.erase(tmp.end() - 1);
		return true;
	}
	return false;
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
	{
		if (throwIn(fin, nullptr) == "error_pages") {
			prev = true;
		} else {
			std::string tmp;
			noSemi(fin, tmp);
			limit = std::stoi(tmp);
		}
	}
	/// error_pages
	{
		if (!prev)
			throwIn(fin, "error_pages");
		std::vector<int>v;
		std::string tmp;
		for (; !noSemi(fin, tmp); )
			v.push_back(std::stoi(tmp));
		errorPages.insert(std::pair<std::vector<int>, std::string>(v, tmp));
	}
	/// location
	{
		for (std::string tmp; fin >> tmp, tmp != "}"; ) {
			locations.push_back(Location(fin));
		}
	}
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

std::map<std::vector<int>, std::string> Config::Host::getErrorPages() const {
	return errorPages;
}

std::vector<Config::Host::Location> Config::Host::getLocations() const {
	return locations;
}
