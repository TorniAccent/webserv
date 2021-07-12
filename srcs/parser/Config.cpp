#include "ConfigParser.hpp"

Config::Config(const char *config) {
	std::fstream fin(config);
	if (!fin.is_open())
		throw "File did not open\n";
	for (std::string tmp; !fin.eof(); ) {
		hosts.push_back(Host(fin));
		std::getline(fin, tmp);
	}
}

Config::Config(Config const &copy) {
	hosts = copy.getHosts();
}

Config &Config::operator=(Config const &copy) {
	hosts = copy.getHosts();
	return *this;
}

std::vector<Config::Host> Config::getHosts() const {
	return hosts;
}