#include "ConfigParser.hpp"

Config::Host::Location::Location(std::fstream &fin)
{ // todo: make read fields optional
	std::string str;
	std::vector<std::string> vec;
	/// location
	{
		std::getline(fin, str);
		if (!(vec = split(str, "location")).empty()) {
			web = vec.front();
		}
	}
	/// {
	{
		std::getline(fin, str);
	}
	/// root
	{
		std::getline(fin, str);
		if (!(vec = split(str, "root")).empty()) {
			root = vec.front();
		}
	}
	/// methods
	{
		std::getline(fin, str);
		if (!(vec = split(str, "methods")).empty()) {
			methods = vec;
		}
	}
	/// autoindex
	{
		std::getline(fin, str);
		if (!(vec = split(str, "autoindex")).empty()) {
			listing = (vec.front() == "on");
		}
	}
	/// index
	{
		std::getline(fin, str);
		if (!(vec = split(str, "index")).empty()) {
			indexes = vec;
		}
	}
	/// cgi
	{
		std::getline(fin, str);
		if (!(vec = split(str, "cgi")).empty()) {
			cgiFile = vec.front();
		}
	}
	std::getline(fin, str);
}

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