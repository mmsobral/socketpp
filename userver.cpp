/* 
 * File:   client.cpp
 * Author: msobral
 *
 * Created on 6 de Julho de 2016, 20:13
 */

#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include "UDPSocket.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    UDPSocket server("0.0.0.0", 8000);
    
    while (true) {
      try {
        string addr;
        int port;

        string dados = server.recv(64, addr, port);
        cout << "Recebeu " << dados.size() << " caracteres de " << addr << ":" << port <<" --> " << dados << endl;
      } catch (UDPSocket::SocketException e) {
        perror("ao enviar");
      }
    }
    
    
    return 0;
}

