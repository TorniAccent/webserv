/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: swquinc <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 19:27:48 by swquinc           #+#    #+#             */
/*   Updated: 2021/07/02 19:29:33 by swquinc          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <pthread.h>

void 	*test(void *data)
{
	int *a;
	a = reinterpret_cast<int*>(data);
	pid_t 		i;
//	std::cout << *a << std::endl;
	if (*a == 1)
	{
		while (true)
		{
			printf("%s\n", "work");
//			std::cout << "work\n";
			sleep(2);
		}
	}
	i = fork();
	if (i < 0) {
//		std::cerr << "fork error" << std::endl;
		return (NULL);
	}
	else if (i == 0)
	{
		int fd;
		fd = open("test", O_CREAT | O_WRONLY, 0777);
		while (true)
		{
			write(fd, "child", 6);
			printf("%s\n", "child cry");
//			std::cerr << "child also hork\n";
			sleep(3);
		}
	}
	else
	{
		printf("%s\n", "I'am wait you");

//		std::cout << "I'am wait you" << std::endl;
		waitpid(i, NULL, WUNTRACED);
//		std::cout << "dee, no i'm not wait" << std::endl;
		printf("%s\n", "dee, no i'm not wait");

	}
	return (NULL);
}

int main(int argc, char **argv)
{
	pthread_t	a;
	pthread_t	b;
	int			data = 1;
	int			data2 = 2;
	size_t 		check;

	std::string str;

//	str = "/www/taraa";
//
//	check = str.find_last_of('/') + 1;
//
//	std::cout << str.substr(check, str.length() - check) << std::endl;
	std::cout << str.length() - check << std::endl;

	data = open("Makefile", O_RDONLY);

	std::cout << data << std::endl;

	data2 = open("Makefile", O_RDONLY);
	std::cout << data2 << std::endl;
	close(data);

	std::cout << open("config", O_RDONLY);
//	str = ;
//	std::cout << argv[0] << std::endl;
//	std::cout << argv[1] << std::endl;
	return (0);
	pthread_create(&a, NULL, test, &data);
	pthread_create(&b, NULL, test, &data2);

	pthread_join(a, NULL);
	pthread_join(b, NULL);
	return (0);
}
