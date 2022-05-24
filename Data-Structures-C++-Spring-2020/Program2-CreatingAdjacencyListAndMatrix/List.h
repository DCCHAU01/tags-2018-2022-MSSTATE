//============================================================================
//Compiler Used:Visual Studio
//Program Description:file containing Adjacency List and Node definitions
//============================================================================
#pragma once
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::string;

class Node//node definitions
{
public://a below pointer and next pointer for a 2d array
	int data;
	string cmd;
	Node*next;
	Node*below;
	Node(int data)//creates node to store integer data, such as weight, initial, or final
	{
		this->data = data;
	}
	Node(string cmd) {//creates node to store string commands
		this->cmd = cmd;
	}
};

class adjList//adjacency list/matrix definitions
{
public://2d array is built with a stack and a singly linked list extending next to each node in the stack
	Node*top;
	Node*bot;
	Node*temp = nullptr;//temps 1 to 4 are used to hold temporary memory locations when searching
	Node*temp2 = nullptr;
	Node*temp3 = nullptr;
	Node*temp4 = nullptr;
	int countr = 0;//used to keep count of 2d array size
	adjList() :top(nullptr),bot(nullptr) {}
	void addvertex(int data = 0);//adds a vertex
	void removevertex(int data);//removes a vertex
	void addedge(int initial = 0, int final = 0, int weight = 0);//adds an edge and a weight to that edge
	void removeedge(int initial = 0, int final = 0);//removes an edge if it has a weigh
	void findedge(int initial = 0, int final = 0);//finds an edge
	void displaymatrix();//displays adjacency matrix
	void displayvertex();//displays vertices
	void displaylist();//displays adjacency list
	void displayall();//executes display: matrix, vertex, and list
	void output(string cmd);//outputs vertex list and adjacency matrix into a given file
};
