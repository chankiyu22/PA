#ifndef SERVER_CLIENT
#define SERVER_CLIENT

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include "Connect.h"
using namespace std;

/**
 * Class Server_Client
 * Inherit from Connect
 *
 * Reimplementation of functions in Connect
 * Basic Functionality of TCP Server-Client
 * connection
 */
class Server_Client : public Connect{
public:

  /**
   * Constructor
   *
   * Taking port and server as parameters
   * Resulting in getting connection information
   * Filling fields in private section.
   */
  Server_Client(int port, char* server);

  /**
   * Function getmeta
   *
   * Taking a string of get command as parameter
   * Result in constructing fake metadata in
   * this class to utilise polymorphism.
   */
  void getmeta(char* getCommand);

  /**
   * Function getdata
   * 
   * Taking a string of filename as parameter
   * Result in writing downloaded data into a
   * specified destination.
   */
  void getdata(char* filename);

  /**
   * Function Sockfd
   * Extract sock representation in this class
   */
  int Sockfd() const {return sockfd;}

  Metadata meta() const {return metadata;}
};

#endif
