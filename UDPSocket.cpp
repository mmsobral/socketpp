/* 
 * File:   UDPSocket.cpp
 * Author: msobral
 * 
 * Created on 6 de Julho de 2016, 16:14
 */

#include <netdb.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <exception>
#include <sstream>
#include "UDPSocket.h"

using namespace std;

UDPSocket::UDPSocket() {
  init(SOCK_DGRAM, 17, "0.0.0.0", 0);
}

UDPSocket::UDPSocket(const UDPSocket& orig) : BaseSocket(orig) {
}

UDPSocket::UDPSocket(const string& addr, unsigned short port) {
  init(SOCK_DGRAM, 17, addr, port);
}

UDPSocket::UDPSocket(int socket_descriptor) : BaseSocket(socket_descriptor) {
}

UDPSocket::~UDPSocket() {
}

int UDPSocket::send(const string& data, const string & addr, int port) {
    const char * buffer = data.c_str();
    int len = data.size();

    return send(buffer, len, addr, port);
}

int UDPSocket::send(const char* buffer, int num_bytes, const string & addr, int port) {
    if (not sd) throw SocketException(EINVAL); 
    
    if (not num_bytes) return 0;

    sockaddr_in saddr = make_addr(addr, port);        
    
    int n = ::sendto(sd, buffer, num_bytes, 0, (sockaddr*)&saddr, sizeof(saddr));
    
    if (n < 0) throw SocketException(errno);
    
    return n;
}

string UDPSocket::recv(int max_bytes, string & addr, int & port) {
    char buffer[max_bytes];
    buffer[0] = 0;
    
    int n = recv(buffer, max_bytes, addr, port);
    return string(buffer, n);    
}

int UDPSocket::recv(char* buffer, int max_bytes, string & addr, int & port) {
    if (not sd) throw SocketException(EINVAL); 
    sockaddr_in saddr;
    socklen_t addrlen = sizeof(addr);

    int n = ::recvfrom(sd, buffer, max_bytes, 0, (sockaddr*)&saddr, &addrlen);
    if (n < 0) {
        if (errno != EAGAIN) throw SocketException(errno);
        n = 0;
    }
    addr = inet_ntoa(saddr.sin_addr);
    port = ntohs(saddr.sin_port);    
    return n;    
}

