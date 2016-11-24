/* 
 * File:   UDPSocket.h
 * Author: msobral
 *
 * Created on 6 de Julho de 2016, 16:14
 */

#ifndef UDPSOCKET_H
#define	UDPSOCKET_H

#include <string>
#include <list>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "BaseSocket.h"

using namespace std;

// classe UDPSocket: não deve ser instanciada diretamente ..
// pois é a classe base para UDPClientSocket e UDPServerSocket
class UDPSocket : public BaseSocket {
 public:     
  UDPSocket(const string & addr, unsigned short port);
  UDPSocket(int socket_descriptor);
  UDPSocket();
  UDPSocket(const UDPSocket& orig);
  virtual ~UDPSocket();
  
  // envia os bytes contidos em "data"
  int send(const string & data, const string & addr, int port);
  
  // envia os "numbytes" bytes contidos em "buffer"
  int send(const char* buffer, int num_bytes, const string & addr, int port);

  // recebe até "max_bytes", e retorna-os como uma string
  string recv(int max_bytes, string & addr, int & port);
  
  // recebe até "max_bytes", e grava-os em "buffer"
  int recv(char * buffer, int max_bytes, string & addr, int & port);
  
};

#endif	/* UDPSOCKET_H */

