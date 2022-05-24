//============================================================================
//Compiler Used:Visual Studio
//Program Description:file containing Adjacency List and Node methods
//============================================================================
#include "List.h"
#include <iostream>
#include <fstream>
#include <string>
using std::string;
using std::ofstream;

void adjList::addvertex(int data) { 
	if (top == nullptr) {//creates 2d array if it doesn't exist
		top = new Node(data);
		bot = top;
		countr += 1;//increases matrix row and column by 1: size is #ofVertex^2
		std::cout << "adding vertex " << data << endl;
	}
	else {
		temp2 = top;
		while (temp2 != bot->below) {//traverse until bottom
			if (temp2->data == data) {//if vertex already exists, print message and return
				std::cout << "vertex " << data << " already exists" << endl;
				temp2 = bot->below;
				return;
			}
			if (temp2 != bot->below) {//if not bottom, go one row down
				temp2 = temp2->below;
			}
			
		}
		bot->below = new Node(data);//creates a new vertex underneath
		bot = bot->below;           //moves one row down
		bot->below = nullptr;       //sets underneath to null
		countr += 1;                //increase matrix size
		
	}
	std::cout << "adding vertex " << data << endl;
	temp2 = top;
	temp = temp2;
	while (temp2 != bot->below) {//traverse until bottom
		for (int i = 0; i < countr; i++) {//traverse across until end
			if (temp->next == nullptr) {
				temp->next = new Node(0);//if null is found, replace null with new node
			}
			temp = temp->next;//move right one column
		}
		temp2 = temp2->below;//move down one row
		temp = temp2;//resets horizontal search to beginning of next row
	}
}
void adjList::removevertex(int data) {
	temp2 = top;
	bool x = false;
	while ((temp2 != bot->below)&&(x==false)) {//traverse down until bottom or if vertex is found
		if (temp2->data == data) {
			std::cout << "vertex " << data << "  found.  removing..." << endl;
			x = true;
			temp2 = bot->below;
		}
		else {
			temp2 = temp2->below;
		}
		
	}
	if (x == false) {//error message if vertex is not found
		std::cout << "vertex " << data << " not found.  can't remove vertex" << endl;
	}
	if (x == true) {//executes if vertex is found
		int counti = 0;
		temp2 = top;
		while (temp2 != bot->below) {//traverse until bottom
			if (temp2->data == data) {//traverse until vertex is found
				if (counti == 0) {//if vertex is top, this executes to delete vertex and set top as the vertex below it
					temp = temp2;
					temp = temp->next;
					while (temp->next != nullptr) {//traverse across until end
						temp3 = temp->next;//holds next location
						temp = nullptr;//deletes current location
						temp = temp3;//goes to next location and repeat
					}
					temp2->next = nullptr;//cuts off vertex's link to corresponding matrix row
					temp3 = temp2;        //--|
					temp2 = temp2->below; //  |these lines delete top 
					top = temp2;          //  |and sets the vertex below as top
					temp2 = nullptr;      //--|then erases where top used to be
					temp2 = top;
					while (temp2 != bot->below) {//traverse until bottom
						temp = temp2;
						for (int i = 0; i < counti + 1; i++) {//traverse horizontally
							temp3 = temp;                     //until 0th row of matrix is found
							temp = temp->next;
						}
						temp4 = temp;        //--|sets index 0's next to index 2
						temp = temp->next;   //  |erases index 1
						temp3->next = temp;  //  |index 2 replaces index 1
						temp = temp4;        //  |
						temp = nullptr;      //--|
						temp2 = temp2->below;//ends loop
					}
					countr -= 1;//reduces matrix size
				}
				if (counti != 0) {//does the same as "if (count == 0)" but relocation of top 
					temp3 = temp; //is replaced with code that follows the same logic:
					temp = temp2; //                          ex: 1's next is set to 3
					temp = temp->next;//                          2 is erased
					while (temp->next != nullptr) {        //     3 fills in 2's place
						temp4 = temp->next;
						temp = nullptr;
						temp = temp4;
					}
					temp2->next = nullptr;
					temp4 = temp2;
					temp2 = temp2->below;
					temp3->below = temp2;
					temp2 = temp4;
					temp2 = nullptr;
					temp2 = top;
					while (temp2 != bot->below) {
						temp = temp2;
						for (int i = 0; i < counti + 1; i++) {
							temp3 = temp;
							temp = temp->next;
						}
						temp4 = temp;
						temp = temp->next;
						temp3->next = temp;
						temp = temp4;
						temp = nullptr;
						temp2 = temp2->below;
					}
					countr -= 1;
				}
				temp2 = bot->below;
			}
			else {
				counti += 1;//keeps track of index until vertex-.to-be-removed is found
				temp = temp2;
				temp2 = temp2->below;
			}
		}
	}
}
void adjList::addedge(int initial, int final, int weight) 
{
	if (weight == 0) {//returns if weight is 0.  an edge can only have a weight > 0
		std::cout << weight << " is not a valid weight.  Weight must be 1 or higher.  Can't add edge " << endl;
		std::cout << endl;
		return;
	}
	bool initia = false;//false until initial is found
	bool fina = false;//false until final is found
	int counti = 0;//keeps count of initial's index if found
	int countf = 0;//keeps count of final's index if found
	temp2 = top;
	while (temp2 != bot->below) {//traverse until bottom
		//ends loop if both initial and final are found
		if (temp2->data == initial) {
			std::cout << "initial vertex " << initial << " found" << endl;
			initia = true;
		}
		if (temp2->data == final) {
			std::cout << "final vertex " << final << " found" << endl;
			fina = true;
		}
		if ((initia == true) && (fina == true)) {
			std::cout << "initial " << initial << " and final " << final << " found.  adding edge " << weight << endl;
			temp2 = bot->below;
		}
		if (temp2 != bot->below) {
			temp2 = temp2->below;
		}
	}
	//error message if initial and/or final are not found
	if (initia == false) {
		std::cout << "initial " << initial << " not found.  can't add edge" << endl;
	}
	if (fina == false) {
		std::cout << "final " << final << " not found.  can't add edge" << endl;
	}
	if ((initia == true) && (fina == true)) {//executes if initial and final's index is found
		temp2 = top;
		while (temp2 != bot->below) {//traverse until bottom
			if (temp2->data == initial) {//ends loop if initial's index is found
				temp2 = bot->below;
			}
			else {
				counti += 1;//keeps track of initial's index
				temp2 = temp2->below;
			}
		}
		temp2 = top;
		while (temp2 != bot->below) {//same as the while loop above except for final's index
			if (temp2->data == final) {
				temp2 = bot->below;
			}
			else {
				countf += 1;
				temp2 = temp2->below;
			}
		}
		temp2 = top;
		for (int i = 0; i < counti; i++) {//traverse down until initial's index
			temp2 = temp2->below;
			temp = temp2;
		}
		temp = temp2;
		for (int i = 0; i < countf + 1; i++) {//traverse across until final's index
			temp = temp->next;
		}
		temp->data = weight;//set edge's weight
		std::cout << endl;
	}
}
void adjList::removeedge(int initial, int final) {
	bool initia = false;//remains false as long as initial isn't found
	bool fina = false;//remains false as long as final isn't found
	int counti = 0;//keeps index of initial if initial is found
	int countf = 0;//keeps index of final if final is found
	temp2 = top;
	while (temp2 != bot->below) {//traverse until bottom
		//edge can be removed if both initial and final are found in vertex list
		if (temp2->data == initial) {
			std::cout << "initial vertex " << initial << " found" << endl;
			initia = true;
		}
		if (temp2->data == final) {
			std::cout << "final vertex " << final << " found" << endl;
			fina = true;
		}
		if ((initia == true) && (fina == true)) {
			std::cout << "initial " << initial << " and final " << final << " found.  removing edge." << endl;
			temp2 = bot->below;
		}
		if (temp2 != bot->below) {
			temp2 = temp2->below;
		}
	}
	//outputs error messages if initial and/or final is not found
	if (initia == false) {
		std::cout << "initial " << initial << " not found.  can't remove edge" << endl;
	}
	if (fina == false) {
		std::cout << "final " << final << " not found.  can't remove edge" << endl;
	}
	if ((initia == true) && (fina == true)) {//executes if both initial and final are found
		temp2 = top;
		while (temp2 != bot->below) {//traverse until bottom
			if (temp2->data == initial) {//ends loop if initial's index is found
				temp2 = bot->below;
			}
			else {
				counti += 1;//keeps count of initial's index
				temp2 = temp2->below;
			}
		}
		temp2 = top;
		while (temp2 != bot->below) {//traverse until bottom
			if (temp2->data == final) {//ends loop if final's index is found
				temp2 = bot->below;
			}
			else {
				countf += 1;//keeps count of finals' index
				temp2 = temp2->below;
			}
		}
		temp2 = top;
		for (int i = 0; i < counti; i++) {//traverse down until initial's index
			temp2 = temp2->below;
			temp = temp2;
		}
		temp = temp2;
		for (int i = 0; i < countf + 1; i++) {//traverse across until final's index
			temp = temp->next;
		}
		if (temp->data == 0) {//error message if no edge is found.  an edge must have a weight > 0
			std::cout << "error, there is no edge in initial: " << initial << " and final: " << final <<  ".  Can't remove edge.  \n";
			return;
		}
		if (temp->data != 0) {//if weight is found, remove weight by setting it to 0
			temp->data = 0;
		}
	}
	std::cout << endl;
}
void adjList::findedge(int initial, int final) {
	bool initia = false;//remains false as long as initial isn't found
	bool fina = false;//remains false as long as final isn't found
	int counti = 0;//keeps index of initial if initial is found
	int countf = 0;//keeps index of final if final is found
	temp2 = top;//starts search at top left of 2d array
	while (temp2 != bot->below) {//traverses until bottom
		if (temp2->data == initial) {//if initial is found, initia is true
			initia = true;
		}
		if (temp2->data == final) {//if final is found, fina is true
			fina = true;
		}
		if ((initia == true) && (fina == true)) {//if both are found, end while loop
			temp2 = bot->below;
		}
		if (temp2 != bot->below) {//traverse until bottom
			temp2 = temp2->below;
		}
	}
	if (initia == false) {//error message if initial not found
		std::cout << "initial " << initial << " not found." << endl;
	}
	if (fina == false) {//error message if final not found
		std::cout << "final " << final << " not found." << endl;
	}
	if ((initia == false) || (fina == false)) {//error message if both are not found
		std::cout << "can't find edge" << endl;
	}
	if ((initia == true) && (fina == true)) {//executes if both are found
		temp2 = top;
		while (temp2 != bot->below) {//traverse down vertex list
			if (temp2->data == initial) {
				temp2 = bot->below;//ends while loop if initial vertex is found
			}
			else {
				counti += 1;//keeps count of initial's index
				temp2 = temp2->below;
			}
		}
		temp2 = top;
		while (temp2 != bot->below) {//does the same as the while loop above
			if (temp2->data == final) {//except for final vertex
				temp2 = bot->below;
			}
			else {
				countf += 1;
				temp2 = temp2->below;
			}
		}
		temp2 = top;
		for (int i = 0; i < counti; i++) {//traverse down vertex list to initial's index
			temp2 = temp2->below;
			temp = temp2;
		}
		temp = temp2;
		for (int i = 0; i < countf + 1; i++) {//traverses across matrix to final's index
			temp = temp->next;
		}//
		if (temp->data == 0) {//error message if no edge is found.  0 weight = no edge
			std::cout << "error, there is no edge in initial: " << initial << " and final: " << final << endl;
			return;
		}
		else {//prints edge and weight if both are found
			std::cout << "weight of edge " << initial << " to " << final << " is " << temp->data << endl;
		}
	}
	std::cout << endl;
}
void adjList::displaymatrix() {
	if (top != nullptr) {//code runs if starting point of array exists
		temp2 = top;
		temp = temp2;
		std::cout << "display matrix\n";
		while (temp2 != bot->below) {//traverse down until bottom
			while (temp->next != nullptr) {//traverse across until end
				temp = temp->next;//move one step right across 2d array
				std::cout << temp->data << "--";//print edge's weight.  0 means no weight
			}
			std::cout << endl;
			temp2 = temp2->below;//move one row down
			temp = temp2;//resets horizontal search at left of 2d array and one row down
		}
		std::cout << endl;
	}
	else {//executes if 2d array doesn't't exist
		std::cout << "There is no matrix to display" << endl << endl;
		return;
	}
}
void adjList::displayvertex() {
	if (top != nullptr) {//code runs if starting point of array exists
		temp2 = top;
		std::cout << "display vertexes\n";
		while (temp2 != bot->below) {//traverse downwards until bottom
			std::cout << temp2->data ;//prints vertex
			std::cout << endl;
			temp2 = temp2->below;//move one row down
		}
		std::cout << endl;
	}
	else {//returns error message if 2d array doesn't exist
		std::cout << "There is no vertex to display" << endl << endl;
		return;
	}
}
void adjList::displaylist() {
	if (top != nullptr) {//code runs if starting point of array exists
		temp2 = top;
		temp = temp2;
		int weight = 0;
		int index = -1;//vertex is at index -1.  matrix starts at index 0
		std::cout << "display list\n";
		while (temp2 != bot->below) {//traverse downwards until bottom
			std::cout << temp2->data << " ";//outputs vertex
			while (temp->next != nullptr) {//traverse across until end
				temp = temp->next;
				index += 1;//keeps count of indexes with weight values
				if (temp->data != 0) {//code executes if weight value is found
					weight = temp->data;
					temp3 = top;
					for (int i = 0; i < index; i++) {//when weight value is found
						temp3 = temp3->below;//traverse downwards until vertical index matches horizontal index
					}
					std::cout << temp3->data << ":" << weight << " ";//outputs final edge and weight
				}
			}
			index = -1;//steps back to maintain search precision
			std::cout << endl;
			temp2 = temp2->below;//placeholder goes down one row down vertex list
			temp = temp2;//sets horizontal search variable to next row underneath the previously searched row
		}
		std::cout << endl;
	}
	else {//return error message if 2d array doesn't exist
		std::cout << "There is no list to display" << endl << endl;
		return;
	}
}
void adjList::displayall() {
	displaymatrix();
	displayvertex();
	displaylist();
}
void adjList::output(string cmd) {
	ofstream file; //creates variable "file" that can only be written in
	file.open(cmd);//opens file "cmd"
	if (top != nullptr) {//conditional to check if 2d array can be outputted
		temp2 = top;
		temp = temp2;
		std::cout << "writing to file: written content should match what you see below.\n";
		std::cout << "first column = vertix list \nother columns = matrix \n";
		file << "first column is vertices \nother columns is the matrix \n";
		while (temp2 != bot->below) {//traverses down 2d array
			std::cout << temp2->data << " | ";
			file << temp2->data << " | ";//writes vertex into file
			while (temp->next != nullptr) {//traverses across 2d array
				temp = temp->next;
				std::cout << temp->data << " ";
				file << temp->data << " ";//writes matrix data into 2d array
			}
			file << "\n";//writes a new line into file
			std::cout << endl;
			temp2 = temp2->below;
			temp = temp2;//at end of while loop, starts from next vertex and repeats
		}
		std::cout << endl;
	}
}
