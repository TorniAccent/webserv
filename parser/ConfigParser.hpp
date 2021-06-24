#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP
# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <cstring>

using std::cout;

class Config {
public:
	class Host;
////
	explicit Config(const char *config = "file");
	Config(Config const &copy);
	Config &operator=(Config const &copy);
////
	std::vector<Host> getHosts() const ;
private:
	std::vector<Host> hosts;
};


class Config::Host {
public:
	class Location;
////
	explicit Host(std::fstream &fin);
////
	std::pair<std::string, int> getAddress() const;
	std::vector<std::string> getIndexes() const;
	size_t getLimitBodySize() const; // ?
	std::map<std::vector<int>, std::string> getErrorPages() const;
		// std::vector<int> errorCode, std::string pagePass
	std::vector<Location> getLocations() const;

private:
	std::pair<std::string, int> address;
	std::vector<std::string> indexes;
	size_t limit;
	std::map<std::vector<int>, std::string> errorPages;
	std::vector<Location> locations;
};


class Config::Host::Location {
public:
	explicit Location(std::fstream &fin);
////
	std::string getRoot();
	std::vector<std::string> getMethods();
	bool autoindex();
	std::vector<std::string> getIndexes();
	std::string getCGI();
private:
	std::string web;
	std::string root;
	std::string defaultFile;
	std::string redirect;
	std::vector<std::string> methods;
	std::vector<std::string> indexes;
	bool listing;
	std::string extension;
};

#endif