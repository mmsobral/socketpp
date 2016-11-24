/* 
 * File:   server.cpp
 * Author: msobral
 *
 * Created on 6 de Julho de 2016, 20:15
 */

#include <cstdlib>
#include <iostream>
#include "TCPBaseSocket.h"

using namespace std;
/*
 * 
 */
int main(int argc, char** argv) {
    // cria um socket servidor que recebe conexões no port 8000
    TCPServerSocket server(8000);
    
    // fica eternamente recebendo novas conexões
    // e dados de conexões existentes
    while (true) {
        try {
            // aguarda uma nova conexão ou dados em 
            //uma conexão existente
            Connection & sock = server.wait(0);

            string addr;
            unsigned short port;

            // obtém o IP e port do socket da outra ponta da 
            // conexão
            sock.get_peer(addr, port);
           
            if (sock.isNew()) {
                // caso contrário, deve ser uma  nova conexão
                cout << "Nova conexão: " << addr << ':' << port << endl;
            } else { 
              // tenta receber até 1024 bytes no socket retornado
              // por "wait"
              string data = sock.recv(1024);

              // conseguiu ler algo desse socket ...
              if (data.size()) {                
                // ...mostra-os na tela e envia-os de volta
                // para a outra ponta da conexão
                cout << "recebeu de " << addr << ':' << port;
                cout << ": " << data << endl;
                
                data = "recebido: " + data;
                sock.send(data);
              }
            }
        } catch (TCPServerSocket::DisconnectedException e) {
            // esta exceção informa que uma conexão foi encerrada
            // o socket correspondente foi invalidado automaticamente
            cout << e.what() << ": " << e.get_addr() << ':';
            cout << e.get_port()<< endl;
        }
    }
    
    return 0;
}

