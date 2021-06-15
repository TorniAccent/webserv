#ifndef CONFIG_PARSER_HPP
# define CONFIG_PARSER_HPP
# include <iostream>
# include <fstream>
// # include <string>
# include <vector>
# include <map>

struct Location {
	std::map<std::string, std::string> method; // server[0].location.method
};

struct Server {
	std::string host;
	int port;
	std::map<std::string, Location> location; // server[0].location
};

class Config {
public:
	Config(char *config);
	~Config();
////
	// Server &getServer(size_t index) const;
	// void addServer(Server const &server);
////
	std::vector<Server> server; //server[0]
private:
////
	Config();
	Config(const Config &copy);
	Config const &operator=(Config const &copy);
};

#endif