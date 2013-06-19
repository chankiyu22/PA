#ifndef CONNECT
#define CONNECT

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/**
 * Struct Tuple
 * 
 * Storing a combination of IP and port
 * got from P2P metadata
 */
struct Tuple {
  char IP[16];
  int port;
};

/**
 * Struct Metadata
 *
 * Storing metadata of file we are going
 * to get.
 * It specially contains getmsg to store
 * the prototype of get message type in
 * main
 */
struct Metadata {
  char* getmsg;
  unsigned int offset;
  unsigned int num_blocks;
  unsigned int size;
  Tuple tuple[2];
};

/**
 * Class Connect
 *
 * Base class of server-client and P2P connection
 * construction.
 *
 * It contains information of server connection.
 */
class Connect {
public:
  Connect() {}
  Connect(int port, char* server) {}
  virtual void getmeta(char* getCommand) = 0;
  virtual void getdata(char* filename) = 0;
  Metadata mdata() const { return metadata; }

protected:
  int portno;
  int sockfd;
  struct hostent* server;
  struct sockaddr_in serv_addr;
  Metadata metadata;
};

#endif
