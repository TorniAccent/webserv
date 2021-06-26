#include "parser/ConfigParser.hpp"

int main()
{
	std::cout << npos << std::endl;
	try
	{
		Config config("file");
		std::cout << npos << std::endl;
	}
	catch (const char *e)
	{
		cout << e << "\n";
	}
}