#include "sc.h"

Server_Client::Server_Client(int port, char* cserver) : Connect(port, cserver){
  portno = port;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
  if (connect(sockfd, 
              (struct sockaddr*) &serv_addr, 
              sizeof(serv_addr))
      < 0) {
    cerr << "ERROR connecting" << endl;
    exit(0);
  }
  cout << "Server Connect Success" << endl;
}

void Server_Client::getmeta(char* getCmd) {
  string get_cmd(getCmd);
  get_cmd += "\n";
  int n = send(sockfd, get_cmd.c_str(), strlen(get_cmd.c_str()), 0);
  if (n < 0) {
    cerr << "ERROR writing to socket" << endl;
    exit(0);
  }

  unsigned char header[8];
  bzero(header, 8);
  n = recv(sockfd, header, 8, 0);
  if (n < 0) {
    cerr << "ERROR reading from socket" << endl;
    exit(0);
  }

  unsigned
  int filesize = header[7]
               + header[6] * 256
               + header[5] * 256 * 256
               + header[4] * 256 * 256 * 256;

  unsigned
  int offset = header[3]
             + header[2] * 256
             + header[1] * 256 * 256
             + header[0] * 256 * 256 * 256;

  Metadata mdata;
  mdata.num_blocks = 1;
  mdata.size = filesize;
  mdata.offset = offset;
  metadata = mdata;
}

void Server_Client::getdata(char* filename) {
  fstream ofs;
  ofs.open(filename, fstream::trunc |
                     fstream::out |
                     fstream::binary);

  char buff[2];
  for (int i = 0; i < metadata.size; i++) {
    bzero(buff, 2);
    int n = recv(sockfd, buff, 1, 0);
    if (n < 0) {
      cerr << "ERROR reading from socket" << endl;
      exit(0);
    }
    ofs.write(buff, 1);
  }
  ofs.close();
}
