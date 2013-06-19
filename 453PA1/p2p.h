#ifndef P_P
#define P_P

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include "Connect.h"
#include "sc.h"
using namespace std;

/**
 * Class P2P
 * Inherit from Class Connect
 * 
 * Reimplementation of function in Connect
 * Basic functionality of P2P
 */
class P2P : public Connect{
public:

  /**
   * Constructor
   * 
   * Taking port number and server as parameters
   * Result in getting information of server
   * connection. Filling fields in private section.
   */
  P2P(int port, char* server);

  /**
   * Function getmeta
   * 
   * Taking a string of get command as parameter
   * Result in constructing specific metadata in
   * this class.
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
};

#endif
