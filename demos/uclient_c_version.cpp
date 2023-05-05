/*
* A UDP client socket demo: using the C socket API
*/

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>

using std::string;
using std::stoi;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;


int main(int argc, char * argv[]) {
    if (argc < 3) {
        cerr << "Use: " << argv[0] << " server_address port" << endl;
        return 0;
    }
    
    int port= stoi(argv[2]);
    int cliente; // socket descriptor  
    
    // creates the client socket      
    if ((cliente = socket(AF_INET, SOCK_DGRAM, 17)) < 0) {
      perror("Erro ao criar o socket UDP");
      return errno;
    }
    
    // addr contains the address to be bound to the socket
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY; // any address used by this host
    addr.sin_port = 0; // a random port chosen by the operating system
    
    // binds the address to the socket
    if (bind(cliente, (sockaddr*)&addr, sizeof(addr)) < 0) {
      perror("Erro ao iniciar o socket (bind)");
      return errno;
    }
    
    // obtains the ip address of the server
    // if it was informed as DNS name, it is translated to an IP address
    hostent * haddr;
    if ((haddr = gethostbyname(argv[1])) == NULL) {
      perror("endereço IP inválido");
      return errno;
    }
    // addr now contains the IP address and port of the server
    memcpy(&addr.sin_addr, haddr->h_addr_list[0], haddr->h_length);
    addr.sin_port = htons(port);
    
    // sends forever
    while (true) {
      string algo;
      
      // reads a line and send it to the server
      getline(cin, algo);
      algo += '\n';
      if (sendto(cliente, algo.c_str(), algo.size(), 0, (sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Erro ao enviar ...");
      } 
    }

    close(cliente);    
}

