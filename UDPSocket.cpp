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
  init(SOCK_DGRAM, 17, AddrInfo{});
}

UDPSocket::UDPSocket(const UDPSocket& orig) : BaseSocket(orig) {
}

UDPSocket::UDPSocket(const AddrInfo& addr) {
  init(SOCK_DGRAM, 17, addr);
}

UDPSocket::UDPSocket(int socket_descriptor) : BaseSocket(socket_descriptor) {
}

UDPSocket::~UDPSocket() {
}

int UDPSocket::send(const vector<char>& buffer, const AddrInfo & addr) {
    return send(buffer.data(), buffer.size(), addr);
}

int UDPSocket::send(string_view buffer, const AddrInfo & addr) {
    return send(buffer.data(), buffer.size(), addr);
}

int UDPSocket::send(const char* buffer, int num_bytes, const AddrInfo & addr) {
    if (not sd) throw SocketException(EINVAL); 
    
    if (not num_bytes) return 0;

    sockaddr_in saddr = make_addr(addr);
    
    int n = ::sendto(sd, buffer, num_bytes, 0, (sockaddr*)&saddr, sizeof(saddr));
    
    if (n < 0) throw SocketException(errno);
    
    return n;
}

vector<char> UDPSocket::recv(int max_bytes) {
    AddrInfo addr;

    return recv(max_bytes, addr);
}

vector<char> UDPSocket::recv(int max_bytes, AddrInfo & addr) {
    vector<char> buffer(max_bytes, 0);

    int n = recv(buffer.data(), max_bytes, addr);
    buffer.resize(n);
    return buffer;
}

int UDPSocket::recv(char* buffer, int max_bytes) {
    AddrInfo addr;

    return recv(buffer, max_bytes, addr);
}

int UDPSocket::recv(char* buffer, int max_bytes, AddrInfo & addr) {
    if (not sd) throw SocketException(EINVAL); 
    sockaddr_in saddr;
    socklen_t addrlen = sizeof(addr);

    int n = ::recvfrom(sd, buffer, max_bytes, 0, (sockaddr*)&saddr, &addrlen);
    if (n < 0) {
        if (errno != EAGAIN) throw SocketException(errno);
        n = 0;
    }

    addr.addr = inet_ntoa(saddr.sin_addr);
    addr.port = ntohs(saddr.sin_port);
    return n;    
}

