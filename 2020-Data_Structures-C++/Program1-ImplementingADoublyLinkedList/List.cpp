//============================================================================
//Compiler Used:Eclipse using MinGW
//Program Description:file containing List and Node methods
//============================================================================
#include "List.h"
#include <iostream>
#include <string>
using std::string;

void List::Display()
{//displays list by starting from head and ending at tail
	std::cout << endl;

	Node*start = head;//starts at head
	Node*end = tail;//just to initialize end

	std::cout << "The List contains the following" << endl;
	while (start != nullptr)//transverses list till tail and outputs each data pair in each node
	{
		std::cout << start->datas << "(" << start->datan << ")" << endl;
		end = start;
		start = start->next;	
	}
}

void List::Insert(string uid, int sec_lv)
{//inserts data pair at end of list
	if (head == nullptr)  //if list is empty, stores data pair in head.  points head next to tail next
	{
		head = new Node(uid);
		head->datan = sec_lv;
		tail = head;  //makes tail same as head in terms of location
		head->next = tail->next;  //head's next location is same tail's next location
	}
	else { //begins double linking process and stores data pair in a new node
		temp = tail;  //stores memory location in 1
		tail->next = new Node(uid);  //creates memory locaiton 2 and stores uid in it
		tail->next->datan = sec_lv; //stores sec_lv in same node
		tail = tail->next;  //sets tail's memory location to 2
		tail->prev = temp;  //tail's previous memory location is now 1

	}
}
void List::InsertB(string uid, string uid2, int sec_lv2)//inspired by List::Insert and testing file.txt
{//inserts data pair:uid2,sec_lv2 before uid
	if (BoolFind(uid) == true)//if the before node can be found, then the following is executed
	{
		Node*start = head;
		Node*end = nullptr;
		while (start->datas != uid)//transverses list until the node containing uid is found
		{
			end = start;
			start = start->next;
		}
		Node*temp1 = end;
		temp = start;
		if (end == nullptr)//if data pair is to be inserted before head, then this executes
		{
			end = new Node(uid2);//creates new node
			end->datan = sec_lv2;
			end->next = head;//sets end's next to head
			start->prev = end;//set head's prev to end
			head = end;//the head is relocated to end
		}
		else 
		{
		while (end != start)//if data pair is not before head, then this executes
				{
					end->next = new Node(uid2);//creates new node at end's next
					end->next->datan = sec_lv2;
					end = end->next;//end is moved to end's next
					end->prev = temp1;//end's previous is now its former location
					end->next = temp;//end's next now points to start
					end = end->next; //end is moved to end's next
					start->prev = end;//start's previous is now end
					
				}
		}
	}
	else
	{
		std::cout << uid << " not found!  Can't insert!" << endl; //outputs error message if node containing uid can't be found
	}
	
}
void List::InsertA(string uid, string uid2, int sec_lv2)//taken from List::InsertB and modified
{//inserts data pair:uid2,sec_lv2 after uid
	if (BoolFind(uid) == true) //if the after node can be found, then the following is executed
	{		
		Node*start = head;
		Node*end = nullptr;
		while (start->datas != uid)//traverses list until node containing uid is found
		{
			end = start;//holds position of node before start
			start = start->next;//moves start to its next loccation
		}
		end = start;//end is now in same location as start
		start = start->next;//start is moved to its next location
		Node*temp2 = end;//holds end's current location
		temp = start;//holds start's current location
		while (end != start)
		{
			end->next = new Node(uid2);//creates a new node at end's next location
			end->next->datan = sec_lv2;
			end = end->next;//end moves to its next location
			end->prev = temp2;//points end's previous to temp2's location
			end->next = temp;//points end's next to temp's location
			start->prev = end;//points start's previous to end's location
			end = end->next;//moves end to its next location
		}
	}
	else
	{
		std::cout << uid << " not found!  Can't insert!" << endl;//outputs error message if node containing uid can't be found
	}
	


}

void List::Remove(string uid)
{//searches list for uid and removes it if it can be found.  Returns a console message if it can't be found
	if (BoolFind(uid)==true){
		temp = head;
		Node*temp3 = nullptr;
		while (temp != nullptr && temp->datas != uid)  //uses conditions to transverse list until node containing uid can be found
		{
			temp3 = temp;
			temp = temp->next;
		}
		if (temp == nullptr)//returns nothing if temp ends up in a nullptr location
		{
			return;
		}
		else if (temp == head) {//if temp is in same spot as head, then head is moved to next spot and its previous location is turned into null.  
			head = head->next;
			head->prev = nullptr;
			delete temp;
		}
		else {
			temp3->next = temp->next;//moves temp3's next to temp's next
			Node*temp6 = temp3;//holds temp3's current location
			temp3->prev = temp6;//temp3's previous location is remembered as temp6
			temp3 = temp3->next;//temp3 is moved to its next location but its previous location is still temp6
			delete temp;
		}
 	}
	else
	{
		std::cout << uid << " not found!  Can't remove!" << endl;//returns error message if node containing uid can't be found
	}

}
bool List::Find(string uid)
{//finds a uid from a list and outputs a message displaying uid and security level.  Returns false if uid can't be found.
	std::cout << "command: Find " << uid << endl;
	temp = head;//sets temp to beginning of list
	while (temp != nullptr)//condition to loop until temp reaches tail
	{
		if (temp->datas == uid)//outputs details if node containing uid can be found
		{
			std::cout << uid << " found!" << endl;
			std::cout << "uid: " << temp->datas << endl << "security_level: " << temp->datan << endl << endl;
			return true;
		}
		temp = temp->next;  //moves temp to its next location
	}
	if (temp == nullptr) {  //returns false if temp == null
		std::cout << uid << " not found!" << endl << endl;
		return false;
	}
	else { return false; }//catches any exception and returns false
}

void List::SRemove(string uid)
{//the none output message version of List::Remove.  Aside from deleting cout, there is no difference.  Refer to List::Remove for detailed breakdown
	if (BoolFind(uid) == true) {
		temp = head;
		Node*temp3 = nullptr;
		while (temp != nullptr && temp->datas != uid)
		{
			temp3 = temp;
			temp = temp->next;
		}
		if (temp == nullptr)
		{
			return;
		}
		else if (temp == head) {
			head = head->next;
			head->prev = nullptr;
			delete temp;
		}
		else {
			temp3->next = temp->next;
			Node*temp6 = temp3;
			temp3->prev = temp6;
			temp3 = temp3->next;
			delete temp;
		}
		//std::cout << uid << " removed!" << endl;
	}
	else
	{
		return;
	}

}
bool List::BoolFind(string uid)
{//returns a boolean value:true if uid can be found and false if it can't be found
	temp = head;
	while (temp != nullptr)
	{
		if (temp->datas == uid)
		{
			return true;
		}
		temp = temp->next;
	}
	return false;
}
