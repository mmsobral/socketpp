/* 
 * File:   client.cpp
 * Author: msobral
 *
 * Created on 6 de Julho de 2016, 20:13
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "../UDPSocket.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    sockpp::UDPSocket server(sockpp::AddrInfo( 8000));
    
    while (true) {
      try {
        sockpp::AddrInfo addr;

        auto dados = server.recv(64, addr);
        cout << "Recebeu " << dados.size() << " caracteres de " << addr.addr << ":" << addr.port <<" --> ";
        cout.write(dados.data(), dados.size());
        cout << endl;
      } catch (sockpp::UDPSocket::SocketException e) {
        perror("ao enviar");
      }
    }
    
    
    return 0;
}

