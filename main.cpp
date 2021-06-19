#include "Server.hpp"

int main()
{
	int sock;
	int client;
	struct sockaddr_in	inet;
	struct sockaddr		*base;
	char buf[1000];

	sock = socket(PF_INET, SOCK_STREAM, 0);
	inet.sin_family = PF_INET; // Семейство сокета.
	inet.sin_port = htons(3000); // переводим значение порта в сетевой порядок следования байт
	inet.sin_addr.s_addr = inet_addr("0.0.0.0"); // переводит char* в какое-то число, необходимо для bind();
	base = reinterpret_cast<struct sockaddr *>(&inet);
	bind(sock, base, sizeof(inet));
	listen(sock, 5);
	while (1)
	{
		client = accept(sock, NULL, NULL);
		if (client > 0)
		{
			std::cout << "begin" << std::endl;
			recv(client, &buf, 1000, 0);
			std::cout << buf << std::endl;
			bzero(buf, sizeof(buf));
			std::cout << "end" << std::endl;
		}
	}
	return (0);
}