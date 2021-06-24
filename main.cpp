#include "parser/ConfigParser.hpp"

int main()
{
	std::cout << "lol" << std::endl;
	try
	{
		Config config("file");
		std::vector<Config::Host> hosts =  config.getHosts();
		cout << hosts.front().getAddress().first << "\n";
		cout << hosts.front().getAddress().second << "\n";
		cout << hosts.front().getLimitBodySize() << "\n";
		cout << hosts.front().getErrorPages().begin()->first[0] << "\n";
	}
	catch (const char *e)
	{
		cout << e << "\n";
	}
}