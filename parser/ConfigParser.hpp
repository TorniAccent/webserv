#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP
# include <iostream>
# include <fstream>
# include <vector>

struct Location {
public:
	Location(std::fstream &fin);
////
	std::string web;
	std::string root;
	std::string defaultFile;
	std::string redirect;
	std::vector<std::string> methods;
	bool listing;
	std::string extension;
private:
	Location();
	Location(Location &copy);
	Location &operator=(Location &copy);
};

struct Server {
	std::string host;
	int port;
	std::vector<Location> location; // server[0].location
};

class Config {
public:
	Config(const char *config = "file");
	// ~Config();
////
	// Server &getServer(size_t index) const;
	// void addServer(Server const &server);
private:
	std::vector<Server> server; //server[0]
////
	Config();
	Config(const Config &copy);
	Config &operator=(Config &copy);
};

#endif