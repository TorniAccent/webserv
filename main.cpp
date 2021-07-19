#include "parser/ConfigParser.hpp"
#include "parser/RequestParser.hpp"

int main()
{
	std::cout << npos << std::endl;
	try
	{
		Config config("file");

		std::vector<std::string> sample;
		sample.push_back(
			"GET /cgi-bin/grandma.exe?name=margaret+alarcon HTTP/1.1");
		sample.push_back("aaa: bbb lkemrv gproktb eptm");
		sample.push_back("Host: 127.0.0.1:80");
		RequestParser request(config);
		request.parseHeader(sample);
		std::cout << npos << std::endl;
	}
	catch (const char *e)
	{
		cout << e << "\n";
	}
}