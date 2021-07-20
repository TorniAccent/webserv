#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP
# include <iostream>
# include <fstream>
# include <vector>
# include <map>
# include <cstring>

using std::cout;

# define npos std::string::npos

std::string noComment(std::fstream &fin);
bool noSemi(std::fstream &fin, std::string &tmp);
size_t findField(std::string &str, const char *ref);
std::vector<std::string> split(std::string &str);
std::pair<std::string, std::string> colon_split(std::string const &line);

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
	Host(Host const &copy);
////
	std::pair<std::string, int> getAddress() const;
	std::string getServerName() const;
	std::vector<std::string> getIndexes() const;
	size_t getLimitBodySize() const;
	std::string getErrorPages() const;
		// std::vector<int> errorCode, std::string pagePass
	std::vector<Location> getLocations() const;

private:
	std::pair<std::string, int> address;
	std::string serverName;
	std::vector<std::string> indexes;
	size_t limit;
	std::string errorPages;
	std::vector<Location> locations;
};


class Config::Host::Location {
public:
	explicit Location(std::fstream &fin,
					  std::pair<std::string, int> &address,
					  std::string &serverName,
					  std::vector<std::string> &indexes,
					  size_t &limit,
					  std::string &errorPages);
////
	std::string getWeb() const;
	std::string getRoot() const;
	std::vector<std::string> getMethods() const;
	bool autoindex() const;
	std::vector<std::string> getIndexes() const;
	std::string getCGI() const;
private:
	std::pair<std::string, int> address;
	std::string serverName;
	size_t limit;
	std::string errorPages;
	std::vector<std::string> indexes;
////
	std::string web;
	std::string root;
	std::vector<std::string> methods;
	bool listing;
	std::string cgiFile;
////
//	Location(Config::Host::Location const &copy);
};

#endif