cmake_minimum_required(VERSION 3.22)
project(Socketpp)

set(CMAKE_CXX_STANDARD 17)

add_library(socketpp BaseSocket.cpp UDPSocket.cpp TCPBaseSocket.cpp)
