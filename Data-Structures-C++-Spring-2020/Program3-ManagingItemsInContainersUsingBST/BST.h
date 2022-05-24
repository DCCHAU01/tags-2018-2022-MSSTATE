//============================================================================
//Compiler Used:Visual Studio
//Program Description:file containing BST and Node definitions
//============================================================================
#pragma once
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::string;

class Node//class for Node definitions
{
public:
	int data;//stores item count
	string cmd;//stores string data like item name or container name
	Node*left;//for left child
	Node*right;//for right child
	Node*next;//for traversing from root to root
	Node(int data)//creates node with int stored
	{
		this->data = data;
	}
	Node(string cmd) {//creates node with string stored
		this->cmd = cmd;
	}
	Node() {//creates empty node
		this->right = nullptr;
		this->left = nullptr;
	}
};

class BST//BST class definitions
{
public:
	Node*start = nullptr;//the first root and placeholder
	Node*root;//root of the BST
	Node*hold;//holds former spot when traversing horizontally from root to root
	Node*moveto;//will test if root's next exists or not
	Node*temp = nullptr;//for traversing from container to container
	Node*temp2 = nullptr;//temp2 to 6 act as holders for traversing and modifying memory
	Node*temp3 = nullptr;//and replacing the role of children and parent
	Node*temp4 = nullptr;
	Node*temp5 = nullptr;
	Node*temp6 = nullptr;
	Node*templ = nullptr;//node for testing if left exists
	Node*tempr = nullptr;//node for testing if right exists
	bool countl = false;//bool for testing if left exists
	bool countr = false;//bool for testing if right exists

	BST() :root(nullptr) {}//initializer

	void create(string container = "");//creates a container
	void displaycontainer();//displays list of containers
	void insertinto(string item = "", int count = 0, string container = "");//inserts item and amount into specified container
	bool search(string x = "");//method for searching a container
	bool insert(string x = "");//method for inserting into a container
	void find(string x = "");//uses search to search all containers
	void findin(string x = "", string y = "");//uses search to search a specific container
	bool findcontainer(string y = "");//finds a container
	bool delet(string x = "");//method for deleting an item from a container
	bool findleft(Node*lef);
	bool findright(Node*rig);
	void remove(string x = "");//removes item from every container that has the item
	void removefrom(string x = "", string y = "");//removes item from specified container
	bool findallx(string x = "");//a find function used for testing true/false in all containers.  returns true if any container has the the item
	//the following order traversals were taken from geeksforgeeks.org
	void displaypre(string y = "");
	void pretraverse(Node*y);//root, left, right
	void displaypost(string y = "");
	void postraverse(Node*y);//left, right, root
	void displayin(string y = "");
	void intraverse(Node*y);//left, root, right

	void destroy(string container = "");//destroys a container
	void travdest(Node*y);//uses postorder traverse from line 78 to delete a container BST tree
};
