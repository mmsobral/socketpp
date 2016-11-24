/* 
 * File:   BaseSocket.h
 * Author: msobral
 *
 * Created on 6 de Julho de 2016, 16:14
 */

#ifndef BaseSOCKET_H
#define	BaseSOCKET_H

#include <string>
#include <list>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

// classe BaseSocket: não deve ser instanciada diretamente ..
// pois é a classe base para BaseClientSocket e BaseServerSocket
class BaseSocket {
 public:     
  BaseSocket(const string & addr, unsigned short port);
  BaseSocket(int socket_descriptor);
  BaseSocket();
  BaseSocket(const BaseSocket& orig);
  virtual ~BaseSocket();
  
  // obtém endereço IP e port do socket da outra ponta 
  // da conexão
  void get_peer(string & addr, unsigned short & port);
  
  // obtém endereço IP e port deste socket
  void get_self(string & addr, unsigned short & port);
  
  // conecta a um peer
  virtual void connect(const string & addr, unsigned short port);

  // envia os bytes contidos em "data"
  int send(const string & data);
  
  // envia os "numbytes" bytes contidos em "buffer"
  int send(const char* buffer, int num_bytes);

  // recebe até "max_bytes", e retorna-os como uma string
  string recv(int max_bytes);
  
  // recebe até "max_bytes", e grava-os em "buffer"
  int recv(char * buffer, int max_bytes);

  // retorna o descritor deste socket
  int get_descriptor() { return sd;}
  
  // testa se o há dados para ler no socket
  bool hasData();
  
  // classe SocketException: usada para informar erros
  // de socket em geral. O código de erro
  // segue os erros padrões da variável errno
  class SocketException: public exception {
  public:
      SocketException(int error) : erro(error) {}
      ~SocketException() throw() {}
      const char * what() { return strerror(erro);}
      int get_errno() { return erro;}
  private:
      int erro;
  };
  
 protected:
  int sd;
  
  void init(int type, int proto, const string & addr, int port);
  sockaddr_in make_addr(const string & addr, unsigned short port) const;
};

#endif	/* BaseSOCKET_H */

