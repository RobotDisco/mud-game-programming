// Code Block 2.1 - Header Includes
// This code block includes all of the standard Sockets API/Winsock headers
#ifdef WIN32                // Windows 95 and above
#include "winsock2.h"
#include "Ws2tcpip.h"
#else                       // UNIX/Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
// End Code Block 2.1 - Header Includes

// Code Block 2.2 - Redefinitions and globals for cross-compatibility
#ifdef WIN32                // Windows 95 and above
WSADATA g_wsadata;      // Winsock data holder
#define CloseSocket closesocket
#define GetSocketError WSAGetLastError
#define StartSocketLib WSAStartup( MAKEWORD( 2, 2 ), &g_wsadata );
#define CloseSocketLib WSACleanup();
#ifndef socklen_t
typedef int socklen_t;
#endif
#else                       // UNIX/Linux
#define CloseSocket close
#define GetSocketError errno
#define StartSocketLib {}
#define CloseSocketLib {}
#endif
// End Code Block 2.2 - Redefinitions and globals for cross-compatibility

#include <iostream>
#include <cstring>
using namespace std;
int main() {
  int err;
  StartSocketLib;

  // BEGIN CODE BLOCK 2.3 - Create a Listening Socket on port 4000
  int sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
  if( sock == -1 ) {
    cout << "Socket creation error!" << endl;
    return 0;
  }
  cout << "Socket created!" << endl;

  // create a sockaddr_in for binding, listening on port 4000
  struct sockaddr_in socketaddress;
  socklen_t sa_size = sizeof( struct sockaddr_in );
  socketaddress.sin_family = AF_INET;
  socketaddress.sin_port = htons( 4000 );
  socketaddress.sin_addr.s_addr = htonl( INADDR_ANY );
  memset( &(socketaddress.sin_zero), 0, 8 );

  // bind the socket
  err = bind( sock, (struct sockaddr*)&socketaddress, sa_size );

  // listen on the socket
  err = listen( sock, 16 );
  // END CODE BLOCK 2.3 - Create a Listening Socket on port 4000

  // wait for an incomming connection now
  int datasock;
  datasock = accept( sock, (struct sockaddr*)&socketaddress, &sa_size );

  // receive data1
  char buffer[128];
  err = recv( datasock, buffer, 128, 0 );

  cout << "Data received:" << endl;
  cout << buffer << endl;

  shutdown( datasock, 2 );
  CloseSocket( datasock );

  shutdown( sock, 2 );
  CloseSocket( sock );

  CloseSocketLib;
}
