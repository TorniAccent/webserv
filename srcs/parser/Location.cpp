#include "ConfigParser.hpp"

Config::Host::Location::Location(std::fstream &fin,
								 std::pair<std::string, int> &address,
								 std::string &serverName,
								 std::vector<std::string> &indexes,
								 size_t &limit,
								 std::string &errorPages)
								 : address(address),
								   serverName(serverName), indexes(indexes),
								   limit(limit), errorPages(errorPages)
{ // todo: make read fields optional
	std::string str;
	std::vector<std::string> vec;
	/// location
	{
		std::getline(fin, str);
		vec = split(str);
		if (vec[0] != "location")
			throw "location";
		if (vec.size() != 1) {
			web = vec[1];
		}
	}
	/// {
	{
		std::getline(fin, str);
	}
	/// root
	{
		std::getline(fin, str);
		vec = split(str);
		if (vec[0] != "root")
			throw "root";
		if (vec.size() != 1) {
			root = vec[1];
		}
	}
	/// methods
	{
		std::getline(fin, str);
		vec = split(str);
		if (vec[0] != "methods")
			throw "methods";
		if (vec.size() != 1) {
			methods = vec;
		}
	}
	/// autoindex
	{
		std::getline(fin, str);
		vec = split(str);
		if (vec[0] != "autoindex")
			throw "autoindex";
		if (vec.size() != 1) {
			listing = (vec[1] == "on");
		}
	}
	/// index
	{ // todo: make index read make sense
		std::getline(fin, str);
		vec = split(str);
		if (vec[0] != "index")
			throw "index";
		if (vec.size() != 1) {
			indexes = vec;
		}
	}
	/// cgi
	{
		std::getline(fin, str);
		vec = split(str);
		if (vec[0] != "cgi")
			throw "cgi";
		if (vec.size() != 1) {
			cgiFile = vec[1];
		}
	}
	std::getline(fin, str);
}

//Config::Host::Location::Location(Config::Host::Location const &copy,
//								 Config::Host const &host)
//: address(host.getAddress()),
//  serverName(host.getServerName()), indexes(host.getIndexes()),
//  limit(host.getLimitBodySize()), errorPages(host.getErrorPages()),
//
//  web(copy.getWeb()), root(copy.getRoot()), methods(copy.getMethods()),
//  listing(copy.autoindex()), cgiFile(copy.getCGI())
//{
//}

std::string Config::Host::Location::getWeb() const {
	return web;
}

std::string Config::Host::Location::getRoot() const {
	return root;
}

std::vector<std::string> Config::Host::Location::getMethods() const {
	return methods;
}

bool Config::Host::Location::autoindex() const {
	return listing;
}

std::vector<std::string> Config::Host::Location::getIndexes() const {
	return indexes;
}

std::string Config::Host::Location::getCGI() const {
	return cgiFile;
}