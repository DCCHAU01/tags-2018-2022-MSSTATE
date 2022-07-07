//============================================================================
//Compiler Used:Eclipse using MinGW
//Program Description:a file that implements and manages a list of information in file.txt
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

List L;//creates a class List called L.  L is needed to get methods of List

void exit()//exits the program when called
{
	exit(0);
}
void loadfile(string names) {
	ifstream file;
	file.open(names);
	Node*A = nullptr;  //the file.txt has a maximum of 5 words in a line == 5 nodes.  each node will hold a word
	Node*B = nullptr;
	Node*C = nullptr;
	Node*D = nullptr;
	Node*E = nullptr;
	Node*T = nullptr;
		if (file.is_open())
		{
			std::cout << "file is open" << endl;
			int numWords = 0;
			int numLines = 0;
			string line;
			int temp = 0;
			while (!file.eof()) {  //loops while program has not read till end of file.  Referenced from cplusplus.com
				getline(file, line);  //gets a line from file
				numLines += 1;
				numWords = 0;  //numWords will be used to help identify what each line of text is commanding.  Refer to the while loop below for more details
				// lines 47 & 49 referenced from stackflow
				std::stringstream ss(line);  //creates a stringstream ss that contains the contents of the above getline
				cout << "command: " << line << endl;
				while (getline(ss, line, ' '))  //parses ss into separate words for command line processing
				{
					numWords += 1;  //the following if else if will allocate each word into a node as data.  this will be used to run commands
					if (numWords == 1) {
						A = new Node(line);
						A->datas = line;
						A->prev = nullptr;
						A->next = B;

					}
					else if (numWords == 2) {
						B = new Node(line);
						A->next = B;
						B->datas = line;
						B->prev = A;
					}
					else if (numWords == 3) {
						C = new Node(line);
						B->next = C;
						C->datan = stoi(line);
						C->prev = B;
					}
					else if (numWords == 4) {
						D = new Node(line);
						C->next = D;
						D->datas = line;
						D->prev = C;
					}
					else if (numWords == 5) {
						E = new Node(line);
						D->next = E;
						E->datas = line;
						E->prev = D;
					}

					if (numWords == 2 && A->datas == "remove") {  //the remove command in file.txt has 2 words
						L.Remove(B->datas);
					}
					if (numWords != 5) {
						if (numWords == 3 && A->datas == "insert") {  //the insert command in file.txt has 3 words
							L.Insert(B->datas, C->datan);
						}
						if (numWords == 4)  //if there is a fourth word in the command line, then undo the insert
						{
							L.SRemove(B->datas);
						}
					}
					else if (numWords == 5 && D->datas == "before") {  //if the command line has 5 words, then checks for before or after, if before, then this one runs
						if (L.BoolFind(E->datas) == true) {  //a double verification to find the uid that is used as the before of the new uid,sec_lv pair
							L.InsertB(E->datas, B->datas, C->datan);
						}
						else if (L.BoolFind(E->datas) == false) { //if verification fails, then it means that the uid can't be found and aborts the insertion process
							std::cout << E->datas << " can't be found!  Can't insert " << B->datas << "!" << endl;
						}
					}
					else if (numWords == 5 && D->datas == "after") {  //the after version of insert before.  pretty much the same except it finds a uid that is used as the after of the new uid,sec_lv pair
						if (L.BoolFind(E->datas) == true) {
							L.InsertA(E->datas, B->datas, C->datan);
						}
						else if (L.BoolFind(E->datas) == false) {
							std::cout << E->datas << " can't be found!  Can't insert " << B->datas <<"!" << endl;
						}

					}
					if (numWords == 1 && A->datas == "display") {  //if the command line only has 1 word and that word is display, then a list is displayed
						L.Display();
					}
				}

			}
		} else {
			std::cout << "file is not open" << endl;  //runs if the file is not open and information the user with a console message
		}
}
int main() {
	//make sure to have file.txt in same source file as main.cpp
	loadfile("file.txt");  //loads file, reads it, parses it, and runs the parsed text as commands
	string names[] = { "eddie","tommy","bodaddy","akk","stephen","tina","monica" };  //iterates over name list and finds names
	for (int i = 0; i < 7; i++) {
		L.Find(names[i]);
	}
	exit();
	return 0;
}
