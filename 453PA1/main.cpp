#include "sc.h"
#include "p2p.h"
#include "timer.h"

int main(int argc, char* argv[]) {

  /**
   * Input checking
   */
  if (argc < 4) {
    cerr << "Usage: " << argv[0] << " method server port" << endl;
    exit(0);
  }

  Connect* connection;
  time_t timer_start, timer_end;
  time(&timer_start);

  if (strcmp(argv[1], "SC") == 0) {
    connection = new Server_Client(atoi(argv[3]), argv[2]);
  } 
  else if (strcmp(argv[1], "P2P") == 0) {
    connection = new P2P(atoi(argv[3]), argv[2]);
  }

  /**
   * First, we should get metadata.
   * Note that metadata in server-client connection is
   * meaningless. Please see detailed implmentation.
   * 
   * Parameter: A GET command without any newline 
   * character
   *
   * Usage: 
   * connection->getmeta("GET [filename in server]");
   */
  connection->getmeta("GET Redsox.jpg");

  /**
   * Then, we get the actual data.
   * Using metadata we just got.
   * To destinated file with desire name.
   * 
   * Parameter: A desire file name.
   *
   * Usage:
   * connection->getdata("[filename]");
   */
  connection->getdata("image.jpg");
  
  time(&timer_end);
  cout << "Total time used: " << difftime(timer_end, timer_start) 
       << " seconds" << endl;
}
