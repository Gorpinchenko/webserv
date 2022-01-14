cmake_minimum_required(VERSION 3.19)
project(webserv)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

include_directories(./srcs)

add_compile_options(-Wall -Wextra -Werror)

add_executable(
        webserv

        srcs/Config.cpp
        srcs/Events.cpp
        srcs/Server.cpp
        srcs/Socket.cpp
        srcs/Location.cpp
        srcs/Daemon.cpp
        srcs/main.cpp

        srcs/Config.hpp
        srcs/Events.hpp
        srcs/Server.hpp
        srcs/Socket.hpp
        srcs/Location.hpp
        srcs/Daemon.hpp
)
