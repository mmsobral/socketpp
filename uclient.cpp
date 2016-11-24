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
    UDPSocket client;
    
    try {
      // conecta no servidor da wiki
      client.send("hello world ! \n\n", "localhost", 8000);
    } catch (UDPSocket::SocketException e) {
      perror("ao enviar");
    }
    
    
    return 0;
}

