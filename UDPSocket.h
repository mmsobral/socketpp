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

// classe UDPSocket: representa sockets UDP, podendo enviar ou receber datagramas
    class UDPSocket : public BaseSocket {
    public:
        UDPSocket(const AddrInfo &addr);

        UDPSocket(int socket_descriptor);

        UDPSocket();

        UDPSocket(const UDPSocket &orig);

        virtual ~UDPSocket();

        // envia os bytes contidos em "data"
        int send(const std::vector<char> &data, const AddrInfo &addr);

        int send(std::string_view buffer, const AddrInfo &addr);

        // envia os "numbytes" bytes contidos em "buffer"
        int send(const char *buffer, int num_bytes, const AddrInfo &addr);

        // recebe até "max_bytes", e retorna-os como um vector<char>
        // copia em "addr" o IP e port do remetente
        std::vector<char> recv(int max_bytes, AddrInfo &addr);

        // recebe até "max_bytes", e retorna-os como um vector<char>
        std::vector<char> recv(int max_bytes);

        // recebe até MaxDatagramSize e retorna-os como um vector<char>
        std::vector<char> recv();

        // recebe até MaxDatagramSize e retorna-os como um vector<char>
        // copia em "addr" o IP e port do remetente
        std::vector<char> recv(AddrInfo& addr);

        // recebe até "max_bytes", e grava-os em "buffer"
        int recv(char *buffer, int max_bytes, AddrInfo &addr);

        int recv(char *buffer, int max_bytes);

        const size_t MaxDatagramSize = 8192;
    };

}
#endif	/* UDPSOCKET_H */

