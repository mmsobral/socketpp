# socketpp
A very basic socket library for C++

SocketPP is a very simple API with TCP and UDP socket implementatins for C++, both for client and server. They are provided by the following classes:
* [TCPClientSocket](TCPBaseSocket.h#L39): TCP client socket ([demo](demos/client.cpp))
* [TCPServerSocket](TCPBaseSocket.h#L73): TCP server socket ([demo](demos/server.cpp))
* [UDPSocket](UDPSocket.h#L22): UDP socket ([client demo](demos/uclient.cpp) and [server demo](demos/userver.cpp))

## Compilation

SocketPP requires at least C++ 2017. To compile an application with SocketPP with g++ or clang, use option -std=c++17. For instance, to compile the UDP client demo with g++ you can execute:

```
g++ -std=c++17 -o uclient uclient.cpp BaseSocket.cpp UDPSOcket.cpp
```
