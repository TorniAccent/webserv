#include "ConfigParser.hpp"

std::string noComment(std::fstream &fin) {
	std::string s;
	for (; fin >> s, s == "#"; )
		for (; fin.get() != '\n'; );
	return s;
}

bool noSemi(std::fstream &fin, std::string &tmp) {
	fin >> tmp;
	if (tmp.find(';') != npos) {
		tmp.erase(tmp.end() - 1);
		return true;
	}
	return false;
}

size_t findField(std::string &str, const char *ref) {
	size_t n;
	if ((n = str.find(ref)) == npos)
		throw ref;
	return n;
}

std::vector<std::string> split(std::string &str) {
	size_t spc = str.find_first_not_of(" \t");
	std::vector<std::string> ret;
	ret.push_back(str.substr(spc, str.find_first_of(" :;", spc) - spc));
	spc = str.find_first_of(" :;", spc);
	for (size_t tmp; ; ) {
		if ((tmp = str.find_first_of(" :;", spc + 1)) == npos || str.find_first_of(" :;", spc + 1) + 1 == str.length()) {
			ret.push_back(str.substr(spc + 1, tmp - (spc + 1)));
			break;
		} // last condition
		tmp = spc;
		spc = str.find_first_of(" :;", tmp + 1);
		ret.push_back(str.substr(tmp + 1, spc - (tmp + 1)));
	}
	return ret;
}

