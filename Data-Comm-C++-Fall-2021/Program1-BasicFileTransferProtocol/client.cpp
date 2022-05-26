//Language: C++
//Compiler: Pluto
//Date: 9/18/2021
//Desc: client side communication using UDP, handshake, and acks
//Issue: sometimes recvfrom on client side refuses connection from server.
//When this happens, I change the port number. 
//This might mean that the previous port is occupied.
//Worst issue was getting connection refused two times in a row in different ports

#include <iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <netdb.h> 
#include <iostream>
#include <fstream>
#include <arpa/inet.h>   // if you want to use inet_addr() function
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]){
    int i;//i and j for loops
    int j;
    char payload[512];//will hold messages to send
    string line;//will hold each line from text file

  struct hostent *s; 
  s = gethostbyname(argv[1]);//gets host name from 1st argument
  
  struct sockaddr_in server;

  int hssocket = 0;
  int mysocket = 0;

  socklen_t slen = sizeof(server);

  if ((hssocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
      cout << "Error in creating socket.\n";
  }
  //cout << "hssocket: " << hssocket << endl;

  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[2]));
  bcopy((char *)s->h_addr, 
	(char *)&server.sin_addr.s_addr,
	s->h_length);
  //cout << server.sin_family << endl;
  //cout << server.sin_port << endl;
  //cout << server.sin_addr.s_addr << endl;

  char handshake[] = "1234\0";
  if (sendto(hssocket, handshake, sizeof(handshake), 0, (struct sockaddr*) & server, slen) == -1)
  {
      cout << "Error in sendto function.\n";
  }
  char newport[512];
  recvfrom(hssocket, newport, 512, 0, (struct sockaddr*) & server, &slen);

  close(hssocket);

  if ((mysocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
  {
      cout << "Error in creating socket.\n";
  }
  //cout << "mysocket: " << mysocket << endl;

  uint16_t a = atoi(newport);
  server.sin_port = a;

  if (connect(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1) {//connect to (presumably) waiting server
      perror("connect fail");
      exit(EXIT_FAILURE);
  }

  //cout << server.sin_family << endl;
  //cout << server.sin_port << endl;
  //cout << server.sin_addr.s_addr << endl;

  ifstream file;
  file.open(argv[3]);

  socklen_t b = sizeof(server);//duplicate of slen.  My short memory and doubt made it annoying to scroll up to see if slen was reallly there.  Now I'm too tired to change it.

  char  ack[512];
  char STOP = '1';
  char c;
while (!file.eof()) {
          for (j = 0; j < 4; j++) {
              if (file.eof()) {
                  payload[j - 1] = '\0';
              }
              else {
                  payload[j] = file.get();
              }
          }
          payload[j] = '\0';
          //cout << payload << endl;
          if (connect(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1) {//connect to waiting server
              perror("connect fail");
              exit(EXIT_FAILURE);
          }
          if (sendto(mysocket, payload, sizeof(payload), 0, (struct sockaddr*) & server, slen) == -1)//send payload to server
          {
              cout << "Error in sendto function.\n";
          }
          listen(mysocket, 1);//wait for server's ack
          accept(mysocket, (struct sockaddr*) & server, &b);
          if (recvfrom(mysocket, ack, sizeof(ack), 0, (struct sockaddr*) & server, &slen) == -1) {//receiving ack
              perror("recvfrom");//exits if -1 is returned
              exit(EXIT_FAILURE);
          }
          cout << ack << endl;
  }

payload[0] = '1';//when it's time to stop, send a 1 to server
if (connect(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1) {//connect to waiting server
    perror("connect fail");
    exit(EXIT_FAILURE);
}
if (sendto(mysocket, payload, sizeof(payload), 0, (struct sockaddr*) & server, slen) == -1)//send payload to server
{
    cout << "Error in sendto function.\n";
}

  
  close(mysocket);//closes mysocket
  
  file.close();//closes file
  return 0;
}
