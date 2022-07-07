//Language: C++
//Server: Pluto
//Date: 10/20/2021
//Modified from PA1
//Desc: server side communication using UDP, handshake, and acks
//Includes GBN

#include<iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <time.h>        // used for random number generation and timer
#include <string.h> // using this to convert random port integer to string
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <errno.h> //used to access perror for precise error handling
#include <typeinfo> //used to check a variable's type
#include "packet.h"
#include <cstring>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "INSUFFICIENT ARGUMENTS\n";
        exit(EXIT_SUCCESS);
    }
    int i;//i and j for loops
    int j;
    char payload[512];//will hold messages from receive
    memset(payload, 0, 512);
    char spacket[512];
    //memset(spacket, 0, 512);
    srand(time(NULL));
    struct sockaddr_in server;
    struct sockaddr_in client;
    int hssocket = 0;
    int mysocket = 0;
    socklen_t clen = sizeof(client);
    if ((hssocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)//create hand shake socket
    {
        cout << "Error in socket creation.\n";
    }
    memset((char*)&server, 0, sizeof(server));//setting family, port and address of server
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
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
    for (i = 0; i < sizeof(handshake) - 1; i++) {
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
        stringstream ss;//converting uint16_t to char array
ss << newport;//because sendto is incompatible with uint16_t
string s;
ss >> s;
char cnewport[s.length()];
for (i = 0; i < s.length(); i++)
{
    cnewport[i] = s[i];
}
if (sendto(hssocket, cnewport, sizeof(cnewport) + 1, 0, (struct sockaddr*) & client, clen) == -1) {
    cout << "Error in sendto function.\n";
}
cout << "\nThe random port chosen is " << cnewport << endl << endl;
    }
    close(hssocket);//closes the handshake socket after the handshake
    server.sin_port = newport;//changing to new port for file exchange
    if ((mysocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)//creating socket for file exchange
    {
        cout << "Error in socket creation.\n";
    }
    if (bind(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1)//binding socket to new port
    {
        perror("Error in binding mysocket");
    }
    //MODIFICATION STARTS HERE----------------------------------------------------------
    string buffer[7];
    for (i = 0; i < 7; i++) {
        buffer[i] = "0";
    }
    int SB = 0;
    int NS = 0;
    int FL = 0;
    int SN = 0;
    int RC = 1;
    int w = 0;
    char TST[50];
    //bool NOTEOT = true;
    //bool shift = true;
    ofstream logA("arrival.log");
    ofstream file;//opening file for writing
    file.open(argv[2]);
    if (!file.is_open()) {
        cout << "file not open!" << endl;
    }
    if (file.fail())
    {
        cout << "file fail\n";
        file.clear(ios::failbit);//if write didn't work the failbit might be on.  Clears the fail bit.
    }

    //file << "TEST";
    packet rcvpacket(0, 0, 0, payload);
    packet temp(0, 0, 0, payload);
    while (1) {
        if (RC == 1) {//Tells when program can receive or not
            //cout << "RC executing\n";
            while (((buffer[6] == "0")) /*&& (NOTEOT == true)*/) {//receives while window is not full
                //cout << "buffer[6] == 0 executing\n";
                //cout << buffer[6] << endl;
                listen(mysocket, 1);
                accept(mysocket, (struct sockaddr*) & client, &clen);
                cout << "receiving next sequence: " << NS << endl;
                if (recvfrom(mysocket, payload, 50, 0, (struct sockaddr*) & client, &clen) == -1) {
                    perror("recvfrom");//exits if -1 is returned
                    exit(EXIT_FAILURE);
                }
                buffer[w] = payload;
                //cout << "payload: " << payload << endl;
                //strcpy(payload, buffer[w].c_str());
                //cout << "buffed payload: " << payload << endl;
                strcpy(spacket, buffer[w].c_str());
                // cout << "buffed spacket: " << spacket << endl;
                 //strcpy(spacket, payload);
                 //cout << "paid spacket: " << spacket << endl;
                 //cout << "received: " << payload << endl;
                rcvpacket.deserialize(payload);
                //rcvpacket.printContents();
                logA << rcvpacket.getSeqNum() << endl;
                w++;
                NS++;
                NS %= 8;
                if (rcvpacket.getType() == 3) {
                    cout << "Received client EOT packet\n";
                    //NOTEOT = false;
                    RC = 0;
                }
            }
            //cout << buffer[6] << endl;
            //cout << "end buffer[6] loop\n";
        }
        //sleep(1);
        if ((buffer[0] != "0") /*&& (NOTEOT == true)*/) {//gets data from first frame in buffer
            strcpy(payload, buffer[0].c_str());
            strcpy(TST, payload);
            //cout << "Buffer[0]: " << buffer[0] << endl;
            //cout << "Payload Before: " << payload << endl;
            temp.deserialize(payload);
            //cout << "Payload After: " << payload << endl;
        }
        if ((temp.getSeqNum() == SN) && (temp.getType() != 3)) {//runs if not an EOT packet and is an expected packet
            //cout << temp.getSeqNum() << " == " << SN << endl;
            for (j = 0; j < 30; j++){
                if (TST[j + 7] == payload[j]) {
                    file << payload[j];
                }
                else {
                    payload[j] = '\0';
                }
            }
            payload[j] = '\0';
            //cout << payload << endl;
            packet ackpacket(0, temp.getSeqNum(), 0, 0);
            ackpacket.serialize(spacket);
            if (connect(mysocket, (struct sockaddr*) & client, sizeof(client)) == -1) {//connect to waiting server
                perror("connect fail");
                exit(EXIT_FAILURE);
            }
            cout << "sending ack: " << SN << endl;
            if (sendto(mysocket, spacket, strlen(spacket), 0, (struct sockaddr*) & client, clen) == -1) {//sends ack to client
                perror("sendto");
                exit(EXIT_FAILURE);
            }
            for (j = 0; j < 6; j++) {
                buffer[j] = buffer[j + 1];
            }
            buffer[j] = "0";
            SN++;
            SN %= 8;
            w -= 1;
        }
        else {
            if (temp.getSeqNum() != SN) {//runs if unexpected packet is received
                cout << temp.getSeqNum() << " != " << SN << endl;
                SN--;
                if (SN == -1) {
                    SN = 7;
                }
                packet ackpacket(0, SN, 0, 0);
                ackpacket.serialize(spacket);
                if (connect(mysocket, (struct sockaddr*) & client, sizeof(client)) == -1) {//connect to waiting server
                    perror("connect fail");
                    exit(EXIT_FAILURE);
                }
                if (sendto(mysocket, spacket, 50, 0, (struct sockaddr*) & client, clen) == -1) {//sends ack to client
                    perror("sendto");
                    exit(EXIT_FAILURE);
                }
                SN++;
                SN %= 8;
                for (i = 0; i < 7; i++) {
                    buffer[i] = "0";
                }
                NS = SN;
                RC = 1;
            }
        }
        if (temp.getType() == 3) {//runs if frame in window is a client EOT packet.  sends a server EOT packet
            cout << "Encountered client EOT packet in window\n";
            cout << "sending server EOT packet\n";
            packet EOTpacket(2, SN, 0, 0);
            EOTpacket.serialize(spacket);
            if (connect(mysocket, (struct sockaddr*) & client, sizeof(client)) == -1) {//connect to waiting server
                perror("connect fail");
                exit(EXIT_FAILURE);
            }
            if (sendto(mysocket, spacket, strlen(spacket), 0, (struct sockaddr*) & client, clen) == -1) {//sends ack to client
                perror("sendto");
                exit(EXIT_FAILURE);
            }
            file.close();//closes file
            close(mysocket);//closes mysocket
            exit(EXIT_SUCCESS);
        }

    }

    file.close();//closes file
    logA.close();
    close(mysocket);//closes mysocket
    exit(EXIT_SUCCESS);
}
