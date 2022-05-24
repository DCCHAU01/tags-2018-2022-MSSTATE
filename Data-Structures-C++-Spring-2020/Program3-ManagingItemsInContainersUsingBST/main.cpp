//============================================================================
//Compiler Used:Visual Studio
//Program Description: a file managing a list of containers with BST
//============================================================================
#include "BST.h"
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

BST L;

void exit()//exits program when executed
{
	exit(0);
}

void loadfile(string filename) {
	ifstream file;  //creates variable that accepts input from txt file
	file.open(filename);  //opens given txt file "filename"
	Node*a = new Node(0);//nodes a to f will store commands and/or data
	Node*b = new Node(0);//every even numbered node: b, d, f; will be tried for integers
	Node*c = new Node(0);
	Node*d = new Node(0);
	Node*e = new Node(0);
	if (file.is_open())  //executes if file is open
	{
		std::cout << "file is open" << endl;
		int numwords = 0;
		int numlines = 0;
		string line;
		string word;
		int temp = 0;
		while (!file.eof()) {//executes as long as end of file is not reached
			getline(file, line);//gets one line ending at newline/enter sign
			std::stringstream ss(line);//creates stringstream variable ss
			numwords = 0;//numwords reset to 0 when a new command is inputted.  used to assign command strings and data integers to node list
			temp = 0;//temp is reset to 0 when a new command is inputted.  used to determine maximum number of words in each command line
			while (getline(ss, word, ' '))//gets one word ending at space(' ') character
			{//creates singly linked list with a as head
				numwords += 1;//keeps track of number of words
				if (numwords == 1) {//stores first command word
					a->cmd = word;  //important because the first word is the greatest indicator
					b = a;//starts command line traversal at head of list
				}
				else {//if any of the following conditionals return true, then it is a given that the following word is a data integer
					//executes if the following word is not an integer, storing command string
					b->next = new Node(word);
					b = b->next;
					b->next = nullptr;
					}

				}
			}
			numlines += 1;//keeps track of number of lines
			b = a;//starts command line traversal at head of list
			while (b != nullptr) {//traverses across until end
				b = b->next;
				temp += 1;////keeps track of maximum number of words in command line
			}
			if (temp == 1) {//executes if there is at most one word in command line
				if (a->cmd == "exit") {
					exit();//executes an exit program command
				}
			}
			if (temp == 2) {//executes if there are at most 2 words in command line
				if (a->cmd == "create") {
					b = a;
					b = a->next;
					L.create(b->cmd);
				}
				else if (a->cmd == "destroy") {
					b = a;
					b = a->next;
					L.destroy(b->cmd);
				}
				else if (a->cmd == "remove") {
					b = a;
					b = a->next;
					L.remove(b->cmd);
				}
				else if (a->cmd == "find") {
					b = a;
					b = a->next;
					L.find(b->cmd);
				}
			}
			if (temp == 3) {//executes if there are at most 3 words in command line
				if (a->cmd == "display") {
					b = a;
					b = a->next;
					c = b->next;
					if (c->cmd == "in") {
						L.displayin(b->cmd);
					}
					else if (c->cmd == "pre") {
						L.displaypre(b->cmd);
					}
					else if (c->cmd == "post") {
						L.displaypost(b->cmd);
					}
				}
			}
			if (temp == 4) {//executes only if 4 words at most in command line
				if (a->cmd == "find") {
					b = a;
					b = a->next;
					c = b->next;
					d = c->next;
					if (c->cmd == "in") {
						L.findin(b->cmd, d->cmd);
					}
				} else if (a->cmd == "remove") {
					b = a;
					b = a->next;
					c = b->next;
					d = c->next;
					if (c->cmd == "from") {
						L.removefrom(b->cmd, d->cmd);
					}
				}
			}
			if (temp == 5) {//executes if 5 words at most in command line
				if (a->cmd == "insert") {
					b = a;            
					b = a->next;
					c = b->next;
					d = c->next;
					e = d->next;
					if (d->cmd == "into"){
						c->data = stoi(c->cmd);
						L.insertinto(b->cmd, c->data, e->cmd);
				}
			}
		}
		file.close();//closes file
	}
	else {//prints error message if file is not open
		std::cout << "file is not open" << endl;
	}
}

int main() {//requires a sample file.txt included in source files to be executed.
	loadfile("file.txt");
	return 0;
}
