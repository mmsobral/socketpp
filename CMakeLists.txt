cmake_minimum_required(VERSION 3.21)
project(socketpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(uclient uclient.cpp BaseSocket.h BaseSocket.cpp UDPSocket.h UDPSocket.cpp)
add_executable(userver userver.cpp BaseSocket.h BaseSocket.cpp UDPSocket.h UDPSocket.cpp)
add_executable(server server.cpp BaseSocket.h BaseSocket.cpp TCPBaseSocket.h TCPBaseSocket.cpp)
add_executable(client client.cpp BaseSocket.h BaseSocket.cpp TCPBaseSocket.h TCPBaseSocket.cpp)
