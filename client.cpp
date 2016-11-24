/* 
 * File:   client.cpp
 * Author: msobral
 *
 * Created on 6 de Julho de 2016, 20:13
 */

#include <cstdlib>
#include <iostream>
#include "TCPBaseSocket.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    TCPClientSocket client;
    
    // conecta no servidor da wiki
    client.connect("wiki.sj.ifsc.edu.br", 80);
    
    // faz uma requisição HTTP
    client.send("GET / HTTP/1.1\n\n");
    
    // recebe e mostra a resposta do servidor
    string resp = client.recv(10240);
    
    cout << resp << endl;
    
    return 0;
}

