/* 
 * File:   TCPBaseSocket.h
 * Author: msobral
 *
 * Created on 6 de Julho de 2016, 16:14
 */

#ifndef TCPBASESOCKET_H
#define	TCPBASESOCKET_H

#include <string>
#include <list>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "BaseSocket.h"

namespace sockpp {

// classe TCPBaseSocket: não deve ser instanciada diretamente ..
// pois é a classe base para TCPClientSocket e TCPServerSocket
    class TCPBaseSocket : public BaseSocket {
    public:
        TCPBaseSocket(const AddrInfo &addr);

        TCPBaseSocket(int socket_descriptor);

        TCPBaseSocket();

        TCPBaseSocket(const TCPBaseSocket &orig);

        virtual ~TCPBaseSocket();

        // testa se o socket está conectado
        bool isConnected() const;

        // termina a conexão ... porém o destructor a termina
        // automaticamente se necessário
        virtual void close() { ::close(sd); }
    };

// classe TCPClientSocket: implementa um socket TCP cliente
    class TCPClientSocket : public TCPBaseSocket {
    public:
        // cria um socket TCP cliente associado a um endereço IP e port
        TCPClientSocket(const AddrInfo &addr);

        // cria um socket TCP cliente associado a um endereço IP e port
        // escolhidos pelo sistema operacional
        TCPClientSocket();

        virtual ~TCPClientSocket() {}
    };

/*class TCPConnectedSocket : public TCPBaseSocket {
  TCPConnectedSocket(int socket_descriptor);
  virtual ~TCPConnectedSocket() {}
  bool isNew() const { return novo;}
  void set_used() { novo = false;}
 private:
  bool novo;
};*/

    class Connection : public TCPBaseSocket {
    public:
        bool isNew() const { return novo; }

        friend class TCPServerSocket;

    private:
        Connection(int socket_descriptor);

        virtual ~Connection() {}

        void set_used() { novo = false; }

        bool novo;
    };

// classe TCPServerSocket: implementa um socket TCP servidor
    class TCPServerSocket : public TCPBaseSocket {
    public:
        // cria um socket TCP servidor associado a um endereço IP e port
        TCPServerSocket(const AddrInfo &addr);

        // cria um socket TCP cliente associado endereço IP
        // escolhido pelo sistema operacional, e ao port informado
        TCPServerSocket(uint16_t port);

        virtual ~TCPServerSocket();

        // aceita uma conexão. Isso retorna um outro socket,
        // que deve ser usado para envio e recepção
        Connection &accept();

        // espera por novas conexões OU por dados disponíveis
        // nas conexões existentes por no máximo "timeout_ms" milissegundos.
        // Retorna o primeiro socket que tiver
        // dados para serem lidos. Novas conexões têm prioridade em relação
        // a dados existentes ...
        Connection &wait(long timeout_ms);

        // espera indefinidamente por novas conexões OU por dados disponíveis
        // nas conexões existentes. Retorna o primeiro socket que tiver
        // dados para serem lidos. Novas conexões têm prioridade em relação
        // a dados existentes ...
        Connection &wait();

        // encerra uma das conexões
        // isso é opcional: se um socket for encerrado explicitamente
        // com seu método "close", o socket servidor que o originou
        /// irá detectar isso quando apropriado ...
        void close_connection(Connection &sock);

        // retorna a quantidade de conexões existentes
        int get_num_connections() const;

        // verifica sockets desconectados, removendos da lista
        void check_disconnected();

        // classe para informar timeout na forma de uma exceção
        class TimeoutException : public std::exception {
        public:
            TimeoutException() {}

            ~TimeoutException() throw() {}

            const char *what() { return "timeout"; }
        };

        // classe para informar que uma ou mais conexões foram
        // terminadas
        class DisconnectedException : public std::exception {
        public:
            DisconnectedException(const AddrInfo &addr) : addr(addr) {}

            ~DisconnectedException() throw() {}

            const char *what() { return "disconnected"; }

            AddrInfo get_addr() const { return addr; }

        private:
            AddrInfo addr;
        };

    protected:
        std::list<Connection *> conns;
    };

}
#endif	/* TCPBASESOCKET_H */

