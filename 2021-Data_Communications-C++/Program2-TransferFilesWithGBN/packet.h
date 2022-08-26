// Author: Maxwell Young
// Date: Feb 1, 2016

#ifndef PACKET_H
#define PACKET_H
 
//using namespace std; 
 
class packet{
	
private:
	
    int type;       // 0 if an ACK, 1 if a data packet
	int seqnum;     // sequence number
	int length;     // number of characters carried in data field 
	char * data;    // remember this should be 0 for ACK packets
 
public:
	
	packet(int t, int s, int l, char * d); // constructor
	//EOT = end of transmission, EOF = end of file
//0 = ACK, 1 = data packet, 2 = EOT from server to client, 3 = EOT from client to server
//ACK packet: seqnum is the sequence number of the packet being acknowledged
//ACK packet: length should be set to 0.  data should be NULL.  
//length should be in range of 0 to 30, less than 30 if EOF
//EOT packet is same as ACK packet, except for its type number
	int getType();
	int getSeqNum();
	int getLength();
	char * getData();
	void printContents(); // print function for testing
	void serialize(char * s);
	void deserialize(char * s);
};
 
#endif