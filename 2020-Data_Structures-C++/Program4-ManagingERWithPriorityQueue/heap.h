//============================================================================
//Compiler Used:Visual Studio
//Program Description:file containing heap and Node definitions
//============================================================================
#pragma once
#include <iostream>
#include <string>
using std::cout;
using std::cin;
using std::endl;
using std::ostream;
using std::string;

class Node
{
public:
	int data;
	string cmd;
	//Node*left;
	//Node*right;
	Node*below;
	Node*next;
	Node(int data)
	{
		this->data = data;
	}
	Node(string cmd) {
		this->cmd = cmd;
	}
	//Node() {
	//	this->left = nullptr;
	//	this->right = nullptr;
	//}
};
class patient
{
public:
	Node*priority;//1 = ambulatory 2 = non-urgent 3 = urgent
			  //4 = critical   5 = code
	Node*ticket;
	int tikcount = 0;
	Node*last;
	Node*first;
	Node*complaint;
	Node*symptom;
	Node*patientfile;
	bool x = false;

	patient() {}

	void admit(string lastname = "", string firstname = "");
	void setcomplaint(string complaint = "");
	void addsymptom(string symptom = "");
	void setpriority(string priority = "");
	void timda();
	void display();
	void next();
	void fulldisplay();
};
class heap
{
public:
	Node*root = nullptr;
	Node*temp = nullptr;
	Node*temp2 = nullptr;
	Node*temp3 = nullptr;
	Node*temp4 = nullptr;
	Node*temp5 = nullptr;
	Node*temp6 = nullptr;
	Node*traverse = nullptr;
	int size = 0;
	int sizec = 0;
	bool comp = false;
	int count = -1;
	int times = 0;
	int child = 0;


	heap() {}

	void timda();
	void travend(Node*y);
	void display();
	void sort(Node*y);
	void findparent(Node*y);
	int findindex(Node*y);
	void findnode(Node*y);
	void patdisplay();
	void next();
	void fulldisplay();
	void build();
};
