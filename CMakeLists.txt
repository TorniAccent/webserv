cmake_minimum_required(VERSION 3.19)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

include_directories(parser)

add_executable(webserv
		parser/ConfigParser.hpp
		parser/Config.cpp
		parser/Location.cpp
		main.cpp parser/Host.cpp)
