//
//  Socket.cpp: Classes for TCP/IP and UDP Datagram INET Sockets.
//  (c) Eric Lecolinet - Telecom ParisTech - 2013.
//  http://www.telecom-paristech.fr/~elc
//
//  Classes:
//  - Socket: TCP/IP or UDP Datagram INET Socket.
//  - ServerSocket: TCP/IP or UDP Datagram INET Socket Server.
//  - SocketBuffer: utility class for exchanging data blocks or text lines between
//    TCP/IP sockets (which are connected but do not preserve record boundaries)
//

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>      // fcntl.h  won't compile without unistd.h !
#include <fcntl.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <csignal>
#include "Socket.h"
using namespace std;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


Socket::Socket(int _type)
// Family is AF_INET (other families such as AF_UNIX or AF_INET6 not supported)
// Type can be SOCK_STREAM (TCP/IP) or SOCK_DGRAM (datagram connection)
// Protocol is 0 (meaning it is chosen automatically)
: sockfd(::socket(AF_INET, _type, /*protocol*/0)) {}


Socket::Socket(int _type, int _sockfd)
: sockfd(_sockfd) {}


Socket::~Socket() {
  close();  // closes the socket.
}


// for INET4 sockets
int Socket::setLocalAddress(struct sockaddr_in& addr, int port) {
  ::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  return 0;
}


// for INET4 sockets
int Socket::setAddress(struct sockaddr_in& addr, const string& host, int port) {
  struct hostent* hostinfo = NULL;
  // gethostbyname() is obsolete!
  if (host.empty() || !(hostinfo = ::gethostbyname(host.c_str())))
    return -1;  // host not found
  
  ::memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  // creer l'adresse du remote host a partir de son nom
  //::memcpy(&addr.sin_addr, hostinfo->h_addr, hostinfo->h_length);
  ::memcpy((void *)&addr.sin_addr, hostinfo->h_addr_list[0], hostinfo->h_length);
  return 0;
}


int Socket::bind(int port) {
  if (sockfd < 0) return INVALID_SOCKET;
  
  // for INET4 sockets
  struct sockaddr_in addr;
  setLocalAddress(addr, port);
  
  // assigns the address specified by addr to sockfd (returns -1 on error, 0 otherwise)
  return ::bind(sockfd, (const struct sockaddr*)&addr, sizeof(addr));
}


int Socket::bind(const string& host, int port) {
  if (sockfd < 0) return INVALID_SOCKET;
  
  // for INET4 sockets
  struct sockaddr_in addr;
  if (setAddress(addr, host, port) < 0) return UNKNOWN_HOST;
  
  // assigns the address specified by addr to sockfd (returns -1 on error, 0 otherwise)
  return ::bind(sockfd, (const struct sockaddr*)&addr, sizeof(addr));
}


int Socket::connect(const string& host, int port) {
  if (sockfd < 0) return INVALID_SOCKET;
  
  // for INET4 sockets
  struct sockaddr_in addr;
  if (setAddress(addr, host, port) < 0) return UNKNOWN_HOST;
  
  // connects sockfd to the address specified by addr (returns -1 on error, 0 otherwise)
  return ::connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
}


int Socket::close() {
  int stat = 0;
  if (sockfd >= 0) {
    stat = ::shutdown(sockfd, SHUT_RDWR);  // SHUT_RDWR=2
    stat += ::close(sockfd);
  }
  sockfd = -1;
  return stat;
}


void Socket::shutdownInput() {
  ::shutdown(sockfd, 0);
}


void Socket::shutdownOutput() {
  ::shutdown(sockfd, 1/*SD_SEND*/);
}


int Socket::setOption(int level, int optname, const void* optval, socklen_t optlen) {
  return ::setsockopt(sockfd, level, optname, optval, optlen);
}


int Socket::getOption(int level, int optname, void* optval, socklen_t* optlen) {
  return ::getsockopt(sockfd, level, optname, optval, optlen);
}


int Socket::setReceiveBufferSize(int size) {
  return ::setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int));
}


int Socket::setSendBufferSize(int size) {
  return ::setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(int));
}


int Socket::setReuseAddress(bool state) {
  int set = state;
  return ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
}


int Socket::setSoLinger(bool on, int time) {
  struct linger l;
  l.l_onoff = on;          // Linger active
  l.l_linger = time;       // How long to linger for
  return ::setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l));
}


int Socket::setSoTimeout(int timeout) {
  struct timeval tv;
  tv.tv_sec = timeout / 1000;             // ms to seconds
  tv.tv_usec = (timeout % 1000) * 1000;   // ms to microseconds
  return ::setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}


int Socket::setTcpNoDelay(bool state) {
  int set = state;
  return ::setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &set, sizeof(int));
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


ServerSocket::ServerSocket()    // See Socket's notes.
: sockfd(::socket(AF_INET, SOCK_STREAM, 0)) {}


ServerSocket::~ServerSocket() {
  close();
}


int ServerSocket::bind(int port, int backlog) {
  if (sockfd < 0) return Socket::INVALID_SOCKET;
  
  // ne marche que pour IN4 !
  struct sockaddr_in addr = {0}; //memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;
  
  if (::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) return -1;
  
  // verifications sur le serveur
  socklen_t taille = sizeof addr;
  if (::getsockname(sockfd, (struct sockaddr*)&addr, &taille) < 0) return -1;
  
  // le serveur se met en attente sur le socket d'ecoute
  // listen s'applique seulement aux sockets de type SOCK_STREAM ou SOCK_SEQPACKET.
  if (::listen(sockfd, backlog) < 0) return -1;
  
  //int nport = ntohs(addr.sin_port);
  //port = nport;
  return 0;
}


int ServerSocket::close() {
  int stat = 0;
  if (sockfd >= 0) {
    //::shutdown(sockfd, SHUT_RDWR);
    ::close(sockfd);
  }
  sockfd = -1;  // indiquer com_sock inutilisable par write/read*()
  return stat;
}


Socket* ServerSocket::accept() {
  int sock_com = -1;
  // cf. man -s 3n accept, attention EINTR ou EWOULBLOCK ne sont pas geres!
  if ((sock_com = ::accept(sockfd, NULL, NULL)) < 0)
    return NULL;
  else
    return new Socket(SOCK_STREAM, sock_com);
}


int ServerSocket::setOption(int level, int optname, const void* optval, socklen_t optlen) {
  return ::setsockopt(sockfd, level, optname, optval, optlen);
}


int ServerSocket::getOption(int level, int optname, void* optval, socklen_t* optlen) {
  return ::getsockopt(sockfd, level, optname, optval, optlen);
}


int ServerSocket::setReceiveBufferSize(int size) {
  return ::setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int));
}


int ServerSocket::setReuseAddress(bool state) {
  int set = state;
  return ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
}


int ServerSocket::setSoTimeout(int timeout) {
  struct timeval tv;
  tv.tv_sec = timeout / 1000;             // ms to seconds
  tv.tv_usec = (timeout % 1000) * 1000;   // ms to microseconds
  return ::setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}


int ServerSocket::setTcpNoDelay(bool state) {
  // turn off TCP coalescence for INET sockets (useful in some cases to avoid delays)
  int set = state;
  return ::setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*)&set, sizeof(int));
}


void ServerSocket::ignoreSigPipe() {
  ::signal(SIGPIPE, SIG_IGN);
}


void ServerSocket::handleSigPipe(void (*func)(int signal)) {
  ::signal(SIGPIPE, func);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


SocketBuffer::SocketBuffer(Socket* _sock) :
sock(_sock),
remaining_buffer(NULL),
remaining_buffer_size(0) {
}

SocketBuffer::SocketBuffer(Socket& _sock) :
sock(&_sock),
remaining_buffer(NULL),
remaining_buffer_size(0) {
}

SocketBuffer::~SocketBuffer() {
  ::free(remaining_buffer);
}


ssize_t SocketBuffer::read(void* buffer, size_t buffer_size) {
  if (sock == NULL || buffer == NULL) return -1;
  char* p = (char*)buffer;
  char* end = p + buffer_size;
  ssize_t received = 0;
  
  do {
    // - received > 0: data received
    // - received = 0: nothing received (file was shutdown)
    // - received < 0: an error occurred
    received = sock->receive(p, end-p);
    if (received < 0) return -1;  // IO error
    else p += received;
  }
  while (received > 0 && p < end);
  
  return p - (char*)buffer;
}


ssize_t SocketBuffer::write(const void* buffer, size_t buffer_size) {
  if (sock == NULL || buffer == NULL) return -1;
  const char* p = (const char*)buffer;
  const char* end = p + buffer_size;
  ssize_t sent = 0;
  
  do {
    // - sent > 0: data sent
    // - sent = 0: file was shutdown
    // - sent < 0: an error occurred
    sent = sock->send(p, end-p);
    if (sent < 0) return -1;  // IO error
    else p += sent;
  }
  while (sent > 0 && p < end);
  
  return p - (const char*)buffer;
}

  
ssize_t SocketBuffer::writeLine(const char* str, size_t num) {
  char buffer[num];
  ::memcpy(buffer, str, num-1);
  buffer[num-1] = '\n';              // add delimiter (instead of final 0)
  return write(buffer, num);
}


ssize_t SocketBuffer::writeLine(const string& str) {
  size_t len = str.length();
  char buffer[len+1];
  ::memcpy(buffer, str.c_str(), len);
  buffer[len] = '\n';                  // add delimiter (instead of final 0)
  return write(buffer, len+1);
}


// lit au plus num-1 !
ssize_t SocketBuffer::readLine(char* str, size_t num, bool& truncated) {
  truncated = false;
  if (sock == NULL || str == NULL) return -1;
  char* pcurrent = str;
  char* pend = str + num - 1;  // num-1 pour garder place pour nul terminator
  char* remaining = remaining_buffer;
  size_t remaining_size = remaining_buffer_size;
  
  while (true) {
    ssize_t received = 0;
    
    if (remaining_size > 0) {
      // récuperer les données en attente depuis la derniere lecture
      received = min(remaining_size, (size_t)(pend-pcurrent));
      ::memcpy(pcurrent, remaining, received);
      remaining += received;
      remaining_size -= received;
    }
    else {
      // lire les donnees envoyees par le client sur le socket
      received = sock->receive(pcurrent, pend-pcurrent);
    }
    
    // Debug:
    //cout << endl << "# received: '";
    //for (ssize_t k = 0; k < received; ++k) cout << *(pcurrent+k);
    //cout << "'" << endl;
    
    // received < 0 => erreur de lecture
    if (received < 0) {
      *str = '\0';
      return -1;
    }
    // received = 0 => fin de connection  et si on envoie un message vide ????
    else if (received == 0) {
      *pcurrent = '\0';
      return pcurrent - str;  // = 0 si end-of-stream
    }
    
    // chercher le delimiteur
    char* pdelim = pcurrent;
    while (pdelim < pcurrent+received && *pdelim != '\n' && *pdelim != '\r')
      ++pdelim;
    
    if (*pdelim == '\n' || *pdelim == '\r') {
      *pdelim = '\0';   // delimiteur trouvé => le remplacer par nul terminator
      // sauver ce qui suit pour appel suivant de la fonction
      saveRemaining(pdelim+1, received-(pdelim-str+1), remaining, remaining_size);
      return pdelim-str+1;
    }
    else pcurrent += received;   // preparer l'iteration suivante

    // delimiteur pas trouvé, le buffer n'est pas assez grand
    if (pcurrent >= pend) {
      truncated = true;
      *pend = '\0';
      saveRemaining(NULL, 0, remaining, remaining_size);
      return pend - str;
    }
  }
}


void SocketBuffer::saveRemaining(const char* buf1, size_t size1,
                                 const char* buf2, size_t size2) {
  remaining_buffer_size = size1 + size2;
  remaining_buffer = (char*) ::realloc(remaining_buffer, remaining_buffer_size);
  // NB: pas de risque d'écrasement par construction
  if (size1 > 0)
    ::memcpy(remaining_buffer, buf1, size1);
  if (size2 > 0)
    ::memcpy(remaining_buffer+size1, buf2, size2);
}


ssize_t SocketBuffer::readLine(string& str) {
  char buffer[1024];
  ssize_t recv = 0, total_recv = 0;
  bool truncated = false;
  str.clear();
  
  do {
    recv = readLine(buffer, sizeof(buffer), truncated);
    //cout << "readLine: received: " << recv << endl;
    if (recv < 0) return recv;
    else if (recv > 0) {str += buffer; total_recv += recv;}
  }
  while (truncated && recv > 0);
  return total_recv;
}


