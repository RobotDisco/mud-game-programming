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
#include <string.h>
using namespace std;
int main() {
  // BEGIN CODE BLOCK 2.4 - Creat a connecting data socket
  int err;
  char message[128] = "Hello Internet!";
  char ip[16] = "";
  unsigned long ipaddr;

  // start the socket library
  StartSocketLib;

  cout << "Enter the IP address to connect to: ";
  cin >> ip;

  // convert the IP address.
  ipaddr = inet_addr( ip );
  if( ipaddr == -1 ) {
    cout << "Error: invalid IP address" << endl;
    return 0;
  }

  // create a socket
  int sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

  // create a sockaddr_in for connection, on port 4000
  struct sockaddr_in socketaddress;
  socklen_t sa_size = sizeof( struct sockaddr_in );
  socketaddress.sin_family = AF_INET;
  socketaddress.sin_port = htons( 4000 );
  socketaddress.sin_addr.s_addr = ipaddr;
  memset( &(socketaddress.sin_zero), 0, 8 );

  // connect the socket
  err = connect( sock, (struct sockaddr*)&socketaddress, sa_size );
  // END CODE BLOCK 2.4 - creating a connecting data socket

  cout << "sending message: " << message << "..." << endl;

  // send data
  err = send( sock, message, strlen( message ) + 1, 0 );

  shutdown( sock, 2 );
  CloseSocket( sock );

  CloseSocketLib;
}
