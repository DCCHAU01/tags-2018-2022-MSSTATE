//Language: C++
//Server: Pluto
//Date: 10/20/2021
//Modified from PA1
//Desc: client side communication using UDP, handshake, and acks
//Includes GBN

#include <iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <netdb.h> 
#include <time.h>       //used for timer
#include <iostream>
#include <fstream>
#include <arpa/inet.h>   // if you want to use inet_addr() function
#include <string.h>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <math.h>
#include "packet.h"
#include <cstring>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;
//Added global variables and signal handler
int c;//compares received ack to expected ack
bool tSet = false;//if timer is set or not
void signalHandler(int sig) {//handles timeout
    if (sig == SIGALRM) {
        tSet = true;
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cout << "INSUFFICIENT ARGUMENTS\n";
        exit(EXIT_SUCCESS);
    }
    int i;//i and j for loops
    int j;
    char payload[512];//will hold messages to send
    memset(payload, 0, 512);
    char spacket[50];
    memset(spacket, 0, 50);
    string line;//will hold each line from text file
    struct hostent* s;
    s = gethostbyname(argv[1]);//gets host name from 1st argument
    struct sockaddr_in server;
    int hssocket = 0;
    int mysocket = 0;
    socklen_t slen = sizeof(server);
    if ((hssocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        cout << "Error in creating socket.\n";
    }
    memset((char*)&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[2]));
    bcopy((char*)s->h_addr,
        (char*)&server.sin_addr.s_addr,
        s->h_length);
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
    uint16_t a = atoi(newport);
    server.sin_port = a;
    if (connect(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1) {//connect to (presumably) waiting server
        perror("connect fail");
        exit(EXIT_FAILURE);
    }
    //MODIFICATION STARTS HERE----------------------------------------------------------
    //initializing signal struc
    struct sigaction sa;
    sa.sa_handler = signalHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGALRM, &sa, NULL) < 0)//sets signal disposition for SIGALRM to signalHandler
    {
        perror("sigaction for SIGNALRM");
            exit(EXIT_FAILURE);
    }
    //initializing timer struc
    struct itimerval tvOn, tvOff;
    tvOn.it_interval.tv_sec = 2;
    tvOn.it_interval.tv_usec = 0;
    tvOn.it_value.tv_sec = 2;
    tvOn.it_value.tv_usec = 0;
    tvOff.it_interval.tv_sec = 0;
    tvOff.it_interval.tv_usec = 0;
    tvOff.it_value.tv_sec = 0;
    tvOff.it_value.tv_usec = 0;
    //setitimer(ITIMER_REAL, &tvOn, NULL);

    packet rcvpacket(0, 0, 0, payload);

    string buffer[7];
    for (i = 0; i < 7; i++) {
        buffer[i] = "0";
    }
    int SB = 0;
    int NS = 0;
    int w = 0;
    bool full = false;
    bool stopsending = false;
    int send = 1;
    int end = 0;
    bool EOTmade = false;
    ofstream logA("clientseqnum.log");
    ofstream logB("clientack.log");
    ifstream file;
    file.open(argv[3]);
    while (1) {
        while ((full == false) && (stopsending == false)) {//sends until window full or when we need to stop sending
            //cout << "full: " << full << "SB: " << SB << "NS: " << NS << " stopsending: " << stopsending << endl;
            for (j = 0; j < 30; j++) {//grabbing bytes from file
                if (file.eof()) {
                    payload[j - 1] = '\0';
                    stopsending = true;
                    //full = true;
                    //cout << "EOF reached!\n";
                    break;
                    //sleep(1);
                }
                else {
                    payload[j] = file.get();
                }
                //cout << j << " : " << payload[j] << endl;
            }
            payload[j] = '\0';
            //if (end == 0) {
            packet senpacket(1, NS, strlen(payload), payload);
            senpacket.serialize(spacket);
            //senpacket.printContents();
            buffer[w] = spacket;
            strcpy(spacket, buffer[w].c_str());
            if (connect(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1) {//connect to waiting server
                perror("connect fail");
                exit(EXIT_FAILURE);
            }
            //set timer to 2 seconds.  if timerset == 0, set timerset to 1.  if timerset == 1, do nothing.
            cout << "Sending packet: " << NS << endl;
            //cout << spacket << endl;
            if (sendto(mysocket, spacket, strlen(spacket), 0, (struct sockaddr*) & server, slen) == -1)//send payload to server
            {
                cout << "Error in sendto function.\n";
            }
            logA << senpacket.getSeqNum() << endl;
            NS++;
            NS %= 8;
            w++;
            if (w == 7) {
                full = true;
            }
            //}
            if (stopsending == true) {
                end = 1;
            }

        }
        //if (SB == NS) {
        if ((file.eof()) && (stopsending == true) && (EOTmade == false)) {//sends client EOT
            //cout << "making client EOT packet\n";
            packet EOTpacket(3, NS, 0, 0);
            char temp[50];
            memset(temp, 0, 50);
            EOTpacket.serialize(temp);
            string tmp = temp;
            //cout << temp << endl;
            cout << "Sending client EOT packet\n";
            buffer[w - 1] = temp;
            if (connect(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1) {//connect to waiting server
                perror("connect fail");
                exit(EXIT_FAILURE);
            }
            //set timer to 2 seconds.  if timerset == 0, set timerset to 1.  if timerset == 1, do nothing.
            if (sendto(mysocket, temp, 50, 0, (struct sockaddr*) & server, slen) == -1)//send payload to server
            {
                cout << "Error in sendto function.\n";
            }
            logA << NS << endl;
            EOTmade = true;
        }

        //}
        if ((full == true) && (SB != NS)) {//receives acks, shifts window, or resend every frame in buffer on timeout
            //cout << "full: " << full << "SB: " << SB << "NS: " << NS << endl;
            if (setitimer(ITIMER_REAL, &tvOn, NULL) < 0)
            {
                perror("setitimer");
                exit(EXIT_FAILURE);
            }
            listen(mysocket, 1);//wait for server's ack
            accept(mysocket, (struct sockaddr*) & server, &slen);
            if (recvfrom(mysocket, spacket, 50, 0, (struct sockaddr*) & server, &slen) == -1) {//receiving ack
                perror("recvfrom");//exits if -1 is returned
                exit(EXIT_FAILURE);
            }
            rcvpacket.deserialize(spacket);
            c = rcvpacket.getSeqNum();
            if ((c != SB) || (tSet == true)) {//do on timeout
                tSet = false;
                cout << rcvpacket.getSeqNum() << " != " << SB << endl;
                for (j = 0; j < 7; j++) {
                    strcpy(spacket, buffer[j].c_str());
                    if (connect(mysocket, (struct sockaddr*) & server, sizeof(server)) == -1) {//connect to waiting server
                        perror("connect fail");
                        exit(EXIT_FAILURE);
                    }
                    //set timer to 2 seconds.  if timerset == 0, set timerset to 1.  if timerset == 1, do nothing.
                    cout << "Sending packet: " << NS << endl;
                    //cout << spacket << endl;
                    if (sendto(mysocket, spacket, strlen(spacket), 0, (struct sockaddr*) & server, slen) == -1)//send payload to server
                    {
                        cout << "Error in sendto function.\n";
                    }
                    packet temp(0, 0, 0, 0);
                    temp.deserialize(spacket);
                    logA << temp.getSeqNum();
                }
            }

            if (c == SB) {//do on expected ack
                cout << "received ack: " << SB << endl;
                logB << rcvpacket.getSeqNum() << endl;
                SB++;
                SB %= 8;
                if (end == 0) {
                    for (j = 0; j < 6; j++) {//shifts window to make space for sending packet
                        buffer[j] = buffer[j + 1];
                    }
                    buffer[j] = "0";
                    full = false;
                    w--;

                }
                else {
                    full = true;
                }
            }

        }
        if (buffer[1] == "0") {//i got lazy here and near deadline.  sets timer when buffer near empty
            if (setitimer(ITIMER_REAL, &tvOff, NULL) < 0)
            {
                perror("setitimer");
                exit(EXIT_FAILURE);
            }
        }
        while (SB == NS) {//when SB == NS, it indicates the end.  Expects a server EOT packet
            listen(mysocket, 1);//wait for server's ack
            accept(mysocket, (struct sockaddr*) & server, &slen);
            if (recvfrom(mysocket, spacket, 50, 0, (struct sockaddr*) & server, &slen) == -1) {//receiving ack
                perror("recvfrom");//exits if -1 is returned
                exit(EXIT_FAILURE);
            }
            rcvpacket.deserialize(spacket);
            cout << "received ack: " << rcvpacket.getSeqNum() << endl;
            //cout << "type: " << rcvpacket.getType() << endl;
            if (rcvpacket.getType() == 2) {
                cout << "received server EOT packet!\n";
                //listen(mysocket, 1);//wait for server's ack
                //accept(mysocket, (struct sockaddr*) & server, &slen);
                //if (recvfrom(mysocket, spacket, 50, 0, (struct sockaddr*) & server, &slen) == -1) {//receiving ack
                //    perror("recvfrom");//exits if -1 is returned
                //    exit(EXIT_FAILURE);
                //}
                //rcvpacket.deserialize(spacket);
                logB << rcvpacket.getSeqNum() << endl;
                file.close();
                logA.close();
                logB.close();
                close(mysocket);//closes mysocket
                exit(EXIT_SUCCESS);

            }
        }


    }

    file.close();
    logA.close();
    logB.close();
    close(mysocket);//closes mysocket
    exit(EXIT_SUCCESS);
}
