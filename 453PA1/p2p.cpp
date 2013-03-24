#include "p2p.h"

/**
 * Connection Construction
 * Through UDP
 */
P2P::P2P(int port, char* cserver) : Connect(port, cserver){
  portno = port;

  /**
   * Open socket
   * With type DGRAM, UDP Connection
   */
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0) {
    cerr << "ERROR opening socket" << endl;
    exit(0);
  }

  /**
   * Find Host
   * With given string of server
   */
  server = gethostbyname(cserver);
  if (server == NULL) {
    cerr << "ERROR, no such host" << endl;
    exit(0);
  }

  /**
   * Assignment to server information
   */
  bzero((char*) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char*) server->h_addr,
        (char*) &serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);

}

/**
 * Get metadata
 * Through UDP Connection
 * Constructed by constructor
 */
void P2P::getmeta(char* getCmd) {
  
  /**
   * Constructing a valid get command
   * accepted by the server
   */
  string get_cmd(getCmd);
  get_cmd += ".torrent";

  /**
   * Send to command to server
   */
  int bufflen = strlen(get_cmd.c_str());
  int slen = sizeof(serv_addr);
  int n = sendto(sockfd, get_cmd.c_str(), bufflen+1, 0, (struct sockaddr*) &serv_addr, slen);
  if (n < 0) {
    cerr << "ERROR sending \"" << get_cmd << "\"" << endl;
    exit(0);
  }

  /**
   * Receive meatadata raw data from server
   */
  socklen_t tlen = sizeof(serv_addr);
  unsigned char meta[24];
  n = recvfrom(sockfd, meta, 25, 0, (struct sockaddr*) &serv_addr,  &tlen);
  if (n < 0) {
    cerr << "ERROR receiving data" << endl;
    exit(0);
  }

  /**
   * In class metadata construction
   */
  Metadata mdata;
  unsigned
  int numBlocks = meta[3] +
                  meta[2] * 256 +
                  meta[1] * 256 * 256 +
                  meta[0] * 256 * 256 * 256;
  mdata.num_blocks = numBlocks;
  cout << "Number of blocks: " << numBlocks << endl;

  unsigned
  int size = meta[7] +
             meta[6] * 256 +
             meta[5] * 256 * 256 +
             meta[4] * 256 * 256 * 256;
  mdata.size = size;
  cout << "Size of the file: " << size << endl;

  unsigned 
  char ip11 = meta[8], ip12 = meta[9], 
       ip13 = meta[10], ip14 = meta[11];
  unsigned
  int port1 = meta[15] +
              meta[14] * 256 +
              meta[13] * 256 * 256 +
              meta[12] * 256 * 256 * 256;

  string ip;
  ostringstream convert;
  convert << (int)ip11 << '.' << (int)ip12 << '.' << 
             (int)ip13 << '.' << (int)ip14;
  ip = convert.str();
  Tuple t;
  strcpy(t.IP, ip.c_str());
  t.port = port1;
  mdata.tuple[0] = t;
  cout << "Assigned IP 1: " << t.IP << ", Port 1: " << t.port << endl;

  unsigned
  char ip21 = meta[16], ip22 = meta[17],
       ip23 = meta[18], ip24 = meta[19];
  unsigned
  int port2 = meta[23] +
              meta[22] * 256 +
              meta[21] * 256 * 256 +
              meta[20] * 256 * 256 * 256;

  ostringstream convert2;
  string ip2;
  convert2 << (int)ip21 << '.' << (int)ip22 << '.'
          << (int)ip23 << '.' << (int)ip24;
  ip2 = convert2.str();
  strcpy(t.IP, ip2.c_str());
  t.port = port2;
  mdata.tuple[1] = t;
  cout << "Assigned IP 2: " << t.IP << ", Port 2: " << t.port << endl;

  mdata.getmsg = new char[bufflen + 1];
  strcpy(mdata.getmsg, getCmd);
  metadata =  mdata;
}

/**
 * Struct Info
 * Thread infomation helper
 */
struct Info {
  int count;
  Metadata mdata;
  char* filename;
};

/**
 * File writing thread
 * 
 * Get data using TCP Connection
 */
void* fthread(void* _info) {
  Info info = *((Info*) _info);
  Tuple tuple = info.mdata.tuple[info.count % 2];

  ofstream ofs;
  ofs.open(info.filename, fstream::out | fstream::binary);

  Server_Client sc(tuple.port, tuple.IP);

  /**
   * Construct a valid TCP get string
   */
  string msg(info.mdata.getmsg);
  msg += " ";
  char ai[3];
  sprintf(ai, "%d", info.count);
  msg += ai;
  msg += "\n";
  char* get = new char[strlen(msg.c_str()) + 1];
  sc.getmeta(strcpy(get, msg.c_str()));
 
  /**
   * Synchronization
   */
  long pos = sc.mdata().offset;
  ofs.seekp(pos);

  /**
   * Read from socket
   */
  char buff[2];
  for (int j = 0; j < sc.meta().size; j++) {
    bzero(buff, 2);
    int n = recv(sc.Sockfd(), buff, 1, 0);
    if (n < 0) {
      cerr << "ERROR reading from socket" << endl;
      exit(0);
    }
    ofs.write(buff, 1);
  }

  ofs.close();
}

/**
 * Get data
 * Using thread
 */
void P2P::getdata(char* filename) {
  
  /**
   * An array of thread
   */
  pthread_t* dthread = new pthread_t[metadata.num_blocks];
  for (int i = 0; i < metadata.num_blocks; i++) {
    /**
     * Constructing helper information
     */
    Info* info = new Info;
    info->filename = filename;
    info->mdata = metadata;
    info->count = i+1;
    pthread_create(dthread + i, NULL, fthread, (void*) info);
  }

  /**
   * Run thread
   */
  for (int i = 0; i < metadata.num_blocks; i++) 
    pthread_join(dthread[i], NULL);
}
