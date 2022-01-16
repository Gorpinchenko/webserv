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
        srcs/Connection.cpp
        srcs/Location.cpp
        srcs/Daemon.cpp
        srcs/HttpRequest.cpp
        srcs/HttpResponse.cpp
        srcs/AHttpMessage.cpp
        srcs/Print.cpp
        srcs/main.cpp

        srcs/Config.hpp
        srcs/Events.hpp
        srcs/Server.hpp
        srcs/Socket.hpp
        srcs/Connection.hpp
        srcs/Location.hpp
        srcs/Daemon.hpp
        srcs/HttpRequest.hpp
        srcs/HttpResponse.hpp
        srcs/AHttpMessage.hpp
        srcs/Print.hpp
        srcs/IEventSubscriber.hpp
)
