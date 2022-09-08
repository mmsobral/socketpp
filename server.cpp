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
    sockpp::TCPServerSocket server(8000);
    
    // fica eternamente recebendo novas conexões
    // e dados de conexões existentes
    while (true) {
        try {
            // aguarda uma nova conexão ou dados em 
            //uma conexão existente
            auto conn = server.wait(0);
            if (conn) {
                auto sock = conn.value();
                // obtém o IP e port do socket da outra ponta da
                // conexão
                auto addr = sock.get_peer();

                if (sock.isNew()) {
                    // caso contrário, deve ser uma  nova conexão
                    cout << "Nova conexão: " << addr.str() << endl;
                } else {
                    // tenta receber até 1024 bytes no socket retornado
                    // por "wait"
                    auto data = sock.recv(1024);

                    // conseguiu ler algo desse socket ...
                    if (data.size()) {
                        // ...mostra-os na tela e envia-os de volta
                        // para a outra ponta da conexão
                        cout << "recebeu de " << addr.str() << ": ";
                        cout.write(data.data(), data.size()) << endl;

                        string resp = "recebido: " + string(data.begin(), data.end());
                        sock.send(resp);
                    }
                }
            }
        } catch (sockpp::TCPServerSocket::DisconnectedException e) {
            // esta exceção informa que uma conexão foi encerrada
            // o socket correspondente foi invalidado automaticamente
            cout << e.what() << ": " << e.get_addr().str() << endl;
        }
    }
    
    return 0;
}

