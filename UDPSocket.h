/* 
 * File:   UDPSocket.h
 * Author: msobral
 *
 * Created on 6 de Julho de 2016, 16:14
 */

#ifndef UDPSOCKET_H
#define	UDPSOCKET_H

#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "BaseSocket.h"

namespace sockpp {

// classe UDPSocket: não deve ser instanciada diretamente ..
// pois é a classe base para UDPClientSocket e UDPServerSocket
    class UDPSocket : public BaseSocket {
    public:
        UDPSocket(const AddrInfo &addr);

        UDPSocket(int socket_descriptor);

        UDPSocket();

        UDPSocket(const UDPSocket &orig);

        virtual ~UDPSocket();

        // envia os bytes contidos em "data"
        int send(const std::vector<char> &data, const AddrInfo &addr);

        int send(const std::string& buffer, const AddrInfo &addr);

        // envia os "numbytes" bytes contidos em "buffer"
        int send(const char *buffer, int num_bytes, const AddrInfo &addr);

        // recebe até "max_bytes", e retorna-os como uma string
        std::vector<char> recv(int max_bytes, AddrInfo &addr);

        std::vector<char> recv(int max_bytes);

        // recebe até "max_bytes", e grava-os em "buffer"
        int recv(char *buffer, int max_bytes, AddrInfo &addr);

        int recv(char *buffer, int max_bytes);

    };

}
#endif	/* UDPSOCKET_H */

