//============================================================================
//Compiler Used:Visual Studio
//Program Description: a file managing an adjacency graph with adjacency matrix/list
//============================================================================
#include "List.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::stringstream;

adjList L;

void exit()//exits program when executed
{
	exit(0);
}

//speaking from Jul 7, 2022 : I can only wince at this implementation
void loadfile(string filename) {
	ifstream file;  //creates variable that accepts input from txt file
	file.open(filename);  //opens given txt file "filename"
	Node*A = new Node(0);//nodes A to F will store commands and/or data
	Node*B = new Node(0);//every even numbered node: B, D, F; will be tried for integers
	Node*C = new Node(0);
	Node*D = new Node(0);
	Node*E = new Node(0);
	Node*F = new Node(0);
	if (file.is_open())  //executes if file is open
	{
		std::cout << "file is open" << endl;
		int numWords = 0;
		int numLines = 0;
		string line;
		string word;
		int temp = 0;
		while (!file.eof()) {//executes as long as end of file is not reached
			getline(file, line);//gets one line ending at newline/enter sign
			std::stringstream ss(line);//creates stringstream variable ss
			numWords = 0;//numWords reset to 0 when a new command is inputted.  used to assign command strings and data integers to node list
			temp = 0;//temp is reset to 0 when a new command is inputted.  used to determine maximum number of words in each command line
			while (getline(ss, word, ' '))//gets one word ending at space(' ') character
			{//creates singly linked list with A as head
				numWords += 1;//keeps track of number of words
				if (numWords == 1) {//stores first command word
					A->cmd = word;  //important because the first word is the greatest indicator
					B = A;//starts command line traversal at head of list
				}
				else {//if any of the following conditionals return true, then it is a given that the following word is a data integer
					if ((B->cmd == "add") || (B->cmd == "remove") || (B->cmd == "find") || (B->cmd == "to") || (B->cmd == "weight")) {
						B->next = new Node(word);//"if" executes and stores integer data
						B = B->next;
						B->data = stoi(word);
						B->next = nullptr;
					} else {//"else" executes if the following word is not an integer, storing command string
						B->next = new Node(word);
						B = B->next;
						B->next = nullptr;
					}
					
				}
			}
			numLines += 1;//keeps track of number of lines
			B = A;//starts command line traversal at head of list
			while (B != nullptr) {//traverses across until end
				B = B->next;
				temp += 1;////keeps track of maximum number of words in command line
			}
			if (temp == 1) {//executes if there is at most one word in command line
				if (A->cmd == "exit") {
					exit();//executes an exit program command
				}
			}
			if (temp == 2) {//executes if there are at most 2 words in command line
				if (A->cmd == "add") {//executes addvertex if command is confirmed to be add #
					B = A;
					B = A->next;
					L.addvertex(B->data);
				}
				if (A->cmd == "remove") {//executes remove vertex if command is confirmed to be remove #
					B = A;
					B = A->next;
					L.removevertex(B->data);
				}
				if (A->cmd == "display") {//executes display [display type]
					B = A;
					B = A->next;
					if (B->cmd == "all") {//displays all display types
						L.displayall();
					}
					else if (B->cmd == "matrix") {//displays adjacency matrix
						L.displaymatrix();
					}
					else if (B->cmd == "vertices") {//displays vertex list
						L.displayvertex();
					}
					else if (B->cmd == "list") {//displays adjacency list
						L.displaylist();
					}
				}
				if (A->cmd == "output") {//executes output [filename] 
					B = A;
					B = A->next;
					L.output(B->cmd);
				}
			}
			if (temp == 4) {//executes "find initial to final" only if 4 words at most in command line
				if (A->cmd == "find") {//continue if "final" is found
					B = A;
					B = A->next;
					C = B->next;
					D = C->next;
					if (C->cmd == "to") {//execute command if "to" is found
						L.findedge(B->data, D->data);
					}
				}//executes "remove initial to final" only if 4 words at most in command line
				if (A->cmd == "remove") {//continue if "remove" is found
					B = A;
					B = A->next;
					C = B->next;
					D = C->next;
					if (C->cmd == "to") {//execute command if "to" is found
						L.removeedge(B->data, D->data);
					}
				}
			}
			if (temp == 6) {//executes if 6 words at most in command line
				if (A->cmd == "add") {//executes if "add" is found.  no need for further conditionals
					B = A;            //since there is no other command type that has 6 words besides this one
					B = A->next;
					C = B->next;
					D = C->next;
					E = D->next;
					F = E->next;
					L.addedge(B->data, D->data, F->data);
				}
			}
		}
		file.close();//closes file
	}
	else {//prints error message if file is not open
		std::cout << "file is not open" << endl;
	}
}

int main() {//will implement adjacency list/matrix ordered by indexing and not value
	loadfile("file.txt");
	return 0;
}
