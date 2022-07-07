//Language: C++
//Compiler: Pluto
//Date: 9/18/2021
//Desc: server side communication using UDP, handshake, and acks
//Issue: sometimes recvfrom on client side refuses connection from server.
//When this happens, I change the port number.
//This might mean that the previous port is occupied.
//Worst issue was getting connection refused two times in a row in different ports

#include<iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <time.h>        // used for random number generation
#include <string.h> // using this to convert random port integer to string
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <errno.h> //used to access perror for precise error handling
#include <typeinfo> //used to check a variable's type

using namespace std;

int main(int argc, char* argv[]) {
    int i = 0;//i and j for loops
    int j;
    char payload[512];//will hold messages from receive
    srand(time(NULL));

    //struct sockaddr_storage their_addr;
    //socklen_t addr_size;

    struct sockaddr_in server;
    struct sockaddr_in client;

    int hssocket = 0;
    int mysocket = 0;

    socklen_t clen = sizeof(client);

    if ((hssocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)//create hand shake socket
    {
        cout << "Error in socket creation.\n";
    }

    //cout << "hssocket: " << hssocket << endl;

    memset((char*)&server, 0, sizeof(server));//setting family, port and address of server
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    //cout << server.sin_family << endl;
    //cout << server.sin_port << endl;
    //cout << server.sin_addr.s_addr << endl;

    if (bind(hssocket, (struct sockaddr*) & server, sizeof(server)) == -1)//binding hssocket to port
    {
        perror("Error in binding handshake");
    }

    char handshake[sizeof("1234")];
    char check[] = "1234";
    bool test = false;//makes sure the handshake password received from client is correct
    if (recvfrom(hssocket, handshake, sizeof(handshake), 0, (struct sockaddr*) & client, &clen) == -1)
    {
        cout << "Failed to receive.\n";
    }
    for (i = 0; i < sizeof(handshake)-1; i++){
        if (check[i] == handshake[i]) {
            test = true;
        }
        else if (check[i] != handshake[i]) {
            test = false;
            cout << "invalid handshake" << endl;
            exit(EXIT_FAILURE);
        }
    }
    uint16_t newport;
  if (test == true) {//generating random port and sending to client
       newport = htons((rand() % (65536 - 1024)) + 1024);
       //cout << "type of newport: " << typeid(newport).name() << endl;
       //cout << "type of server.sin_port: " << typeid(server.sin_port).name() << endl;
       stringstream ss;//converting uint16_t to char array
       ss << newport;//because sendto is incompatible with uint16_t
       string s;
       ss >> s;
       char cnewport[s.length()];
       for (i = 0; i < s.length(); i++)
       {
           cnewport[i] = s[i];
       }
      if (sendto(hssocket, cnewport, sizeof(cnewport)+1, 0, (struct sockaddr*) & client, clen) == -1) {
          cout << "Error in sendto function.\n";
      }
      cout << "\nThe random port chosen is " << cnewport << endl << endl;
  }
  close(hssocket);//closes the handshake socket after the handshake

  server.sin_port = newport;//changing to new port for file exchange
  //cout << server.sin_family << endl;
  //cout << server.sin_port << endl;
  //cout << server.sin_addr.s_addr << endl;

  if ((mysocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)//creating socket for file exchange
  {
      cout << "Error in socket creation.\n";
  }
  //cout << "mysocket: " << mysocket << endl;
  if (bind(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1)//binding socket to new port
  {
      perror("Error in binding mysocket");
  }

ofstream file("blah.txt");//opening file for writing
if (!file.is_open()) {
    cout << "file not open!" << endl;
}
if (file.fail())
{
    file.clear(ios::failbit);//if write didn't work the failbit might be on.  Clears the fail bit.
}

socklen_t a = sizeof(client);//duplicate of clen.  My short memory and doubt made it annoying to scroll up to see if clen was reallly there.  Now I'm too tired to change it.

listen(mysocket, 1);//listens for client from mysocket
accept(mysocket, (struct sockaddr*) & client, &a);//accepts client from mysocket

char ack[] = "1";
bool go = false;
  while (1) {
      go = false;
      while (go != true) {//keeps trying to receive . . . might not have been the best way
          if (recvfrom(mysocket, payload, 512, 0, (struct sockaddr*) & client, &clen) == -1)
          {
              perror("recvfrom");//exits if -1 is returned
              exit(EXIT_FAILURE);
          }
          else { go = true; }
      }
      if (payload[0] == '1') {
          file.close();//closes file
          close(mysocket);//closes mysocket
          return 0;
      }
      file << payload;//writes the payload to file
      for (i = 0; i < sizeof(payload); i++) {
          payload[i] = toupper(payload[i]);
      }
      if (sendto(mysocket, payload, sizeof(payload), 0, (struct sockaddr*) & client, clen) == -1) {//sends ack to client
          perror("sendto");
      }
      listen(mysocket, 1);//waits for payload from client
      accept(mysocket, (struct sockaddr*)& client, &a);
  }

  file.close();//closes file
  close(mysocket);//closes mysocket
  return 0;
}
