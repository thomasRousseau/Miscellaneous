//
//  TCPServer.h: TCP/IP INET Server.
//  (c) Eric Lecolinet - Telecom ParisTech - 2013.
//  http://www.telecom-paristech.fr/~elc
//

#ifndef __TCPServer__
#define __TCPServer__
#include <pthread.h>
#include "Socket.h"


/** TCPServer: TCP/IP INET Server.
 * This class supports TCP/IP AF_INET connections following the IPv4 Internet
 * protocol. Other families, such as AF_INET6 or AF_UNIX are not yet supported.
 */
class TCPServer {
public:
  TCPServer();
  virtual ~TCPServer();
  
  /** starts the main loop of the server on this port.
   * returns 0 for normal termination, a negative value otherwise.
   */
  virtual int run(int port);

protected:
  /** processes a message and returns the response
   * the connection with the client will be closed if false is returned.
   */
  virtual bool processMessage(const std::string& message, std::string& response);

  /// reads messages from a client.
  virtual void readMessages(Socket*);
  
  /// callback function of pthread_create() that calls readMessages().
  static void* startReadMessages(void*);

  /// closes the socket (and the corresponding thread) of a client.
  virtual void closeSocketAndThread(Socket*, const char* msg);

  ServerSocket servsock;
  pthread_rwlock_t lock;
  
private: // disables copy.
  TCPServer(const TCPServer&);
  TCPServer& operator=(const TCPServer&);
};


#endif

