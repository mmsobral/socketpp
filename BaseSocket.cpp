/* 
 * File:   BaseSocket.cpp
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
#include "BaseSocket.h"

using namespace std;

sockaddr_in BaseSocket::make_addr(const string& addr, unsigned short port) const {
    sockaddr_in myaddr;
    hostent * haddr = gethostbyname(addr.c_str());

    if (not haddr) throw BaseSocket::SocketException(errno);
    
    myaddr.sin_family = AF_INET;
    memcpy(&myaddr.sin_addr, haddr->h_addr_list[0], haddr->h_length);
    myaddr.sin_port = htons(port);    
    
    return myaddr;
}

void BaseSocket::init(int type, int proto, const string & addr, int port) {
    sockaddr_in myaddr = make_addr(addr, port);

    if ((sd = socket(AF_INET, type, proto)) < 0) {
        throw BaseSocket::SocketException(errno);
    }

    int opt = 1;
    if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt)) < 0) {
        throw BaseSocket::SocketException(errno);        
    }
/*    if (fcntl(sd, F_SETFL, O_NONBLOCK) < 0) {
        throw BaseSocket::SocketException(errno);        
    }
*/    
    if (bind(sd, (sockaddr*)&myaddr, sizeof(myaddr)) < 0) {
        throw BaseSocket::SocketException(errno);        
    }
}

BaseSocket::BaseSocket() : sd(0) {
}

BaseSocket::BaseSocket(const BaseSocket& orig) {
    sd = orig.sd;
}

BaseSocket::BaseSocket(const string& addr, unsigned short port) {
}

BaseSocket::BaseSocket(int socket_descriptor) : sd(socket_descriptor) {
    if (fcntl(sd, F_SETFL, O_NONBLOCK) < 0) {
        throw BaseSocket::SocketException(errno);        
    }
}

BaseSocket::~BaseSocket() {
    if (sd) ::close(sd);
}

void BaseSocket::get_peer(string& addr, unsigned short& port) {
    sockaddr_in paddr;
    socklen_t len = sizeof(paddr);
    
    if (getpeername(sd, (sockaddr*)&paddr, &len) < 0) {
        throw BaseSocket::SocketException(errno);
    }

    addr = inet_ntoa(paddr.sin_addr);
    port = ntohs(paddr.sin_port);
}

void BaseSocket::get_self(string& addr, unsigned short& port) {
    sockaddr_in paddr;
    socklen_t len = sizeof(paddr);
    
    if (getsockname(sd, (sockaddr*)&paddr, &len) < 0) {
        throw BaseSocket::SocketException(errno);
    }

    addr = inet_ntoa(paddr.sin_addr);
    port = ntohs(paddr.sin_port);
}

void BaseSocket::connect(const string & addr, unsigned short port) {
  sockaddr_in saddr = make_addr(addr, port);

  if (::connect(sd, (sockaddr*)&saddr, sizeof(saddr)) < 0) {
    throw BaseSocket::SocketException(errno);
  }
}

int BaseSocket::send(const string& data) {
    const char * buffer = data.c_str();
    int len = data.size();

    return send(buffer, len);
}

int BaseSocket::send(const char* buffer, int num_bytes) {
    if (not sd) throw BaseSocket::SocketException(EINVAL); 
    
    if (not num_bytes) return 0;

    int n = ::send(sd, buffer, num_bytes, 0);
    
    if (n < 0) throw BaseSocket::SocketException(errno);
    
    return n;
}

string BaseSocket::recv(int max_bytes) {
    char buffer[max_bytes];
    buffer[0] = 0;
    
    int n = recv(buffer, max_bytes);
    return string(buffer, n);    
}

int BaseSocket::recv(char* buffer, int max_bytes) {
    if (not sd) throw BaseSocket::SocketException(EINVAL); 

    int n = ::recv(sd, buffer, max_bytes, 0);
    if (n < 0) {
        if (errno != EAGAIN) throw BaseSocket::SocketException(errno);
        n = 0;
    }
    return n;    
}

bool BaseSocket::hasData(){
    char x;
    struct timeval tv= {0,0};
    fd_set socks;
    FD_ZERO(&socks);
    FD_SET(sd, &socks);
            
    int n = select(sd+1, &socks, NULL, NULL, &tv);
    if (n) {
        ssize_t r = ::recv(sd, &x, 1, MSG_DONTWAIT|MSG_PEEK);
        return (r > 0);
    }
    return false;
}

