//============================================================================
//Compiler Used:Visual Studio
//Program Description: a file managing an ER with a priority queue
//============================================================================
#include "heap.h"
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

patient P;

void exit()//exits program when executed
{
	exit(0);
}
void loadfile(string filename) {
	ifstream file;  //creates variable that accepts input from txt file
	file.open(filename);  //opens given txt file "filename"
	Node*A = new Node(0);//nodes will store commands and/or data
	Node*B = nullptr;
	Node*C = new Node(0);
	Node*D = nullptr;
	if (file.is_open())  //executes if file is open
	{
		int numfiles = 0;
		bool onoff = false;
		string str = "";
		std::cout << "file is open" << endl;
		int numWords = 0;
		int numLines = 0;
		string line;
		string word;
		int temp = 0;
		while (!file.eof()) {//executes as long as end of file is not reached
			getline(file, line);//gets one line ending at newline/enter sign
			if (numLines > 0) {//due to an unidentified bug, I reset A and had it recreate itself after every command line.  the word count for each line was not working right
				A = nullptr;
			}
			std::stringstream ss(line);//creates stringstream variable ss
			numWords = 0;//numWords reset to 0 when a new command is inputted.  used to assign command strings and data integers to node list
			temp = 0;//temp is reset to 0 when a new command is inputted.  used to determine maximum number of words in each command line
			while (getline(ss, word, ' '))//gets one word ending at space(' ') character
			{//creates singly linked list with A as head
				numWords += 1;//keeps track of number of words
				if (numWords == 1) {//stores first command word
					A = new Node(word);  //important because the first word is the greatest indicator
					B = A;//starts command line traversal at head of list
				}
				else {
					B->next = new Node(word);
					B = B->next;
					B->next = nullptr;
				}
				//std::cout << word << ":" << numWords << endl;
			}
			numLines += 1;//keeps track of number of lines
			D = A;//starts command line traversal at head of list
			while (D != nullptr) {//traverses across until end
				D = D->next;
				temp += 1;////keeps track of maximum number of words in command line
			}
			//std::cout << line << ":" << temp << endl;

			if (temp == 1) {//executes if there is at most one word in command line
				B = A;
				if (A->cmd == "exit") {
					exit();//executes an exit program command
				}
				if (onoff == true) {
					if (A->cmd == "timda") {
						onoff = false;
						std::cout << "Closing patient record...\n";
						P.timda();
						numfiles += 1;
					}
				}
				if (onoff == false) {
					if (A->cmd == "display") {
						if (numfiles > 0) {
							std::cout << "Displaying...\n";
							P.display();
						}
					}
					if (A->cmd == "next") {
						if (numfiles > 0) {
							std::cout << "Next patient...\n";
							P.next();
							numfiles -= 1;
							if (numfiles == 0) {
								std::cout << "There are no more patients...\n";
							}
						}
					}
				}
			}
			else if (temp >= 3) {
				B = A;
				B = A->next;
				C = B->next;
				if (onoff == false) {
					if (A->cmd == "admit") {
						std::cout << "Admitting patient record...\n";
						onoff = true;
						P.admit(B->cmd, C->cmd);
					}
				}
				if (onoff == true) {
					if ((A->cmd == "set") && (B->cmd == "priority")) {
						std::cout << "Setting priority...\n";
						P.setpriority(C->cmd);
					}
				}	
				if ((A->cmd == "set") && (B->cmd == "complaint")) {
					std::cout << "Setting complaint...\n";
					str = "";
					while (C != nullptr) {//append method from cplusplus.com
						str.append(C->cmd);
						str.append(" ");
						C = C->next;
					}
					P.setcomplaint(str);
				}
				if ((A->cmd == "add") && (B->cmd == "symptom")) {
					std::cout << "Adding symptom(s)...\n";
					str = "";
					while (C != nullptr) {//append method from cplusplus.com
						str.append(C->cmd);
						str.append(", ");
						C = C->next;
					}
					P.addsymptom(str);
				}
			}
		}
		file.close();//closes file
	} else {//prints error message if file is not open
		std::cout << "file is not open" << endl;
	}
}
int main() {
	loadfile("file.txt");
	return 0;
}
