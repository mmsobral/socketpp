# socketpp
A very basic socket library for C++

SocketPP is a very simple API with TCP and UDP socket implementatins for C++, both for client and server. They are provided by the following classes:
* [TCPClientSocket](TCPBaseSocket.h#L39): TCP client socket ([demo](demos/client.cpp))
* [TCPServerSocket](TCPBaseSocket.h#L73): TCP server socket ([demo](demos/server.cpp))
* [UDPSocket](UDPSocket.h#L22): UDP socket ([client demo](demos/uclient.cpp) and [server demo](demos/userver.cpp))

## Compilation using cmake

Include this in __CMakeLists.txt__:

```
include(FetchContent)
FetchContent_Declare(
        socketpp
        URL https://github.com/IFSCEngtelecomPTC/Socketpp/archive/refs/tags/v1.0.0.tar.gz
)
FetchContent_MakeAvailable(socketpp)
include_directories(${poller_SOURCE_DIR} .)

# OBS: this is just a demo to show you how to link this library to your executable

# The following line is usually generated by CLion (or by yourself, if using cmake manually)
# Replace it accordingly to your project
add_executable(test_app main.cpp)

# This command links socketpp library to your executable
# Rename test_app to yout executable name
target_link_libraries(test_app socketpp)
```

## Compilation using g++ 
SocketPP requires at least C++ 2017. To compile an application with SocketPP with g++ or clang, use option -std=c++17. For instance, to compile the UDP client demo with g++ you can execute:

```
g++ -std=c++17 -o uclient uclient.cpp BaseSocket.cpp UDPSOcket.cpp
```
