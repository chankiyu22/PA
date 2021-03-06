#include "p2p.h"

P2P::P2P(int port, char* cserver) : Connect(port, cserver){
  portno = port;
  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sockfd < 0) {
    cerr << "ERROR opening socket" << endl;
    exit(0);
  }

  server = gethostbyname(cserver);
  if (server == NULL) {
    cerr << "ERROR, no such host" << endl;
    exit(0);
  }

  bzero((char*) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char*) server->h_addr,
        (char*) &serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(portno);

  cout << "P2P SERVER Build Success" << endl;
}

void P2P::getmeta(char* getCmd) {
  int bufflen = strlen(getCmd);
  int slen = sizeof(serv_addr);
  int n = sendto(sockfd, getCmd, bufflen+1, 0, (struct sockaddr*) &serv_addr, slen);
  if (n < 0) {
    cerr << "ERROR sending \"" << getCmd << "\"" << endl;
    exit(0);
  }

  socklen_t tlen = sizeof(serv_addr);
  unsigned
  char meta[24];
  n = recvfrom(sockfd, meta, 25, 0, (struct sockaddr*) &serv_addr,  &tlen);
  if (n < 0) {
    cerr << "ERROR receiving data" << endl;
    exit(0);
  }

  Metadata mdata;
  unsigned
  int numBlocks = meta[3] +
                  meta[2] * 256 +
                  meta[1] * 256 * 256 +
                  meta[0] * 256 * 256 * 256;
  cout << "Number of blocks: " << numBlocks << endl;
  mdata.num_blocks = numBlocks;

  unsigned
  int size = meta[7] +
             meta[6] * 256 +
             meta[5] * 256 * 256 +
             meta[4] * 256 * 256 * 256;
  cout << "File size: " << size << endl;
  mdata.size = size;

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

  metadata =  mdata;
}

void P2P::getdata(char* filename) {
  Tuple tuple = metadata.tuple[0];
  cout << tuple.port << endl;
  cout << tuple.IP << endl;
  
  fstream ofs;
  ofs.open(filename, fstream::out |
                     fstream::binary);
  for (int i = 1; i <= metadata.num_blocks; i++) {
    Server_Client sc(tuple.port, tuple.IP);
    cout << "1" << endl;
    string msg("GET Redsox.jpg "); 
    char ai[3];
    sprintf(ai, "%d", i);
    msg += ai;
    msg += "\n";
    cout << msg << endl;
    char get[30];
    sc.getmeta(strcpy(get, msg.c_str()));
    
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
  }
  ofs.close();
}
