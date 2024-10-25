/*
* A UDP client socket demo: using the SocketPP API
*/

#include <iostream>
#include <string>
#include <UDPSocket.h>

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char * argv[]) {
    if (argc < 3) {
        std::cerr << "Use: " << argv[0] << " IP_server_address port" << endl;
        return 0;
    }
    
    // IP server address and port must be provided as command line arguments
    sockpp::AddrInfo addr{argv[1], static_cast<uint16_t>(std::stoi(argv[2]))};

    // creates a UDP client socket
    sockpp::UDPSocket cliente;
    cliente.set_default_peer(addr);
    
    // send forever ...
    while (true) {
      string algo;
      
      // reads a line from standard inpuit, and send it to the server
      getline(cin, algo);
      algo += '\n';
      try {
        cliente.send(algo.c_str(), algo.size());
      } catch (sockpp::UDPSocket::SocketException e) {
        perror("send error");
      }
    }
    
}

