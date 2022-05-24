//============================================================================
//Compiler Used:Eclipse using MinGW
//Program Description:file containing List and Node definitions
//============================================================================
#pragma once
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::string;
class Node  //creates a node that handle strings and another that handles ints
{           //this allows one node to hold 2 values, datas and datan.  Can be accessed with node->datas or node->datan
public:
	string datas;
	int datan;
	Node*next;
	Node*prev;
	Node(string datas)
	{
		this->datas = datas;
	}
	Node(int datan)
	{
		this->datan = datan;
	}
};

class List  //class containing list definitions
{
public:
	Node*head;
	Node*tail;
	Node*temp = nullptr;
	List() :head(nullptr), tail(nullptr) {}
	void Display();  //displays a list
	void Insert(string uid="", int sec_lv=0);  //inserts uid,sec_lv pair at end
	void InsertB(string uid = "", string uid2 = "", int sec_lv2=0);  //same as Insert except it finds uid and places uid2,sec_lv2 before uid
	void InsertA(string uid="", string uid2="", int sec_lv2=0);  //same as InsertB but inserts after uid
	void Remove(string uid = "");  //finds and removes uid if it can be found
	bool Find(string uid = "");  //finds and displays uid,sec_lv if it can be found
	bool BoolFind(string uid = "");  //same as find except it returns true if uid can be found and false if uid can't be found
	void SRemove(string uid = "");  //cleanup version of remove and does not output a console message
};
