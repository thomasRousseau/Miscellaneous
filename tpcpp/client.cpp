//
// Client C++ pour communiquer avec un serveur TCP
// eric lecolinet - telecom paristech - 2013
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Socket.h"
using namespace std;

static const char* DEFAULT_HOST = "localhost";
static const int DEFAULT_PORT = 3331;


int main(int argc, char* argv[]) {
  const char* host = DEFAULT_HOST;
  int port = DEFAULT_PORT;
  
  if (argc >=2) host = argv[1];
  if (argc >=3) port = atoi(argv[2]);
  
  Socket sock(SOCK_STREAM);
  
  int status = sock.connect(host, port);
  if (status >= 0)
    cout << "Connected to "<< host<<":"<<port << endl;
  else {
    cerr << "Couldn't connect to: "<< host<<":"<<port << endl;
    return 1;
  }
  
  SocketBuffer sockbuf(sock);

  while (cin.good()) {
    cout << "Message: ";
    string message;
    getline(cin, message);

    ssize_t sent = sockbuf.writeLine(message);
    if (sent < 0) {
      cerr << "Could not send message to "<<host<<":"<<port<< endl;
      return 2;
    }
    
    string response;
    ssize_t received = sockbuf.readLine(response);
    if (received < 0) {
      cerr << "Could not receive message from "<<host<<":"<<port<< endl;
      return 3;
    }

    cout << "Response: '" << response << "'" << endl;
  }
  
  return 0;
}


