#include "ConfigParser.hpp"

Config::Config(const char *config) {
	std::fstream fin;
	fin.open(config, std::fstream::in);
	if (fin.is_open() == false)
		throw "File did not open\n";

	for (; fin.eof(); ) {
		hosts.push_back(Host(fin));
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