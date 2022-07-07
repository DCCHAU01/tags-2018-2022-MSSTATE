//============================================================================
//Compiler Used:Visual Studio
//Program Description:file containing heap and Node methods
//============================================================================
#include "heap.h"
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>//rounding down from cplusplus.com
using std::string;
using std::ofstream;
Node*hold = nullptr;
heap H;

void patient::admit(string lastname, string firstname) {
	tikcount += 1;

	patientfile = new Node(1);
	last = new Node(lastname);
	first = new Node(firstname);
	priority = new Node("ambulatory");
	ticket = new Node(tikcount);
	complaint = new Node("");
	symptom = new Node("");

	patientfile->below = last;
	last->below = first;
	first->below = priority;
	priority->below = ticket;
	ticket->below = complaint;
	complaint->below = symptom;

	std::cout << "creating a patient record for " << lastname << ", " << firstname << endl;
	std::cout << "priority: " << priority->cmd << endl;
	std::cout << "ticket#: " << ticket->data << endl;
	std::cout << "if any, add complaint and/or symptom(s)\n";
}
void patient::setcomplaint(string complaint) {
	this->complaint->cmd = complaint;
	std::cout << "complaint: " << complaint << endl;
}
void patient::addsymptom(string symptom) {
	this->symptom->cmd = symptom;
	std::cout << "symptom: " << symptom << endl;
}
void patient::setpriority(string priority) {
	this->priority->cmd = priority;
	if (priority == "ambulatory") {
		patientfile->data = 1;
	}
	else if (priority == "non-urgent") {
		patientfile->data = 2;
	}
	else if (priority == "urgent") {
		patientfile->data = 3;
	}
	else if (priority == "critical") {
		patientfile->data = 4;
	}
	else if (priority == "code") {
		patientfile->data = 5;
	}
	else {
		std::cout << "invalid priority\n";
	}
	std::cout << "priority: " << priority << endl;
}
void patient::timda() {
	hold = patientfile;
	patientfile = nullptr;
	H.timda();
}
void heap::timda() {
	temp = hold;
	hold = nullptr;
	if (root == nullptr) {
		root = new Node("start");
		root->data = 6;
		root->next = temp;
		temp->next = nullptr;
		size += 1;
		sizec = 1;
		//display();
		times += 1;
	}
	else {
		times += 1;
		travend(root);
		traverse->next = temp;
		temp->next = nullptr;
		size += 1;
		sizec = size;
		//if (size == 1) {
		//	display();
		//}
		//display();
		
		std::cout << "times: " << times << endl;
		if (size > 1) {
			std::cout << endl;
			display();
			sort(temp);
		}
		std::cout << endl;
		display();
		//std::cout << temp << "-1-" << temp2 << "-2-" << temp3 << "-3-" << temp4 << "-4-" << temp5 << "-5-" << temp6 << "-6-";
	}
}
void heap::findnode(Node*y) {
	traverse = root;
	while (traverse != y) {
		//std::cout << traverse << "--";
		temp4 = traverse;
		traverse = traverse->next;
	}
	std::cout << endl << "temp4: " << temp4 << "--traverse: " << traverse << endl;
}
void heap::sort(Node*y) {
	std::cout << "\nsize: " << size << endl;
	std::cout << "sizec: " << sizec << endl;
	//while (comp == false) {
		if (root->next != y) {//while swapped node is not head of list, continue
			findparent(y);
			if (y->data == temp2->data) {
				return;
			}
		}
		else { return; }//once swapped node reaches node, there's nothing more to compare
		if (temp2->data > y->data) {//return if parent is greater than child
			return;
		}
		if (temp2->data < y->data) {//swap if parent is less than child
			if (child == 1) {
				findnode(y);
				temp6 = y->next;
				temp5 = temp2->next;
				temp3->next = y;
				temp4->next = y->next;
				y->next = temp4;
				display();
				std::cout << "swapped: " << y << " with " << temp2 << endl;
				sort(y);
			}
			else {
				findnode(y);
				temp6 = y->next;
				temp5 = temp2->next;
				temp3->next = y;
				y->next = temp5;
				temp4->next = temp2;
				temp2->next = temp6;
				std::cout << "swapped: " << y << " with " << temp2 << endl;
				display();
				sort(y);
			}
		}
	//}
}
void heap::findparent(Node*y) {
	int childindex = findindex(y);
	int parentindex = floor((childindex - 1) / 2);
	int counts = -1;
	temp2 = root;
	std::cout << "child: " << childindex << "parent: " << parentindex << endl;
	child = childindex;
	while (counts != parentindex) {
		temp3 = temp2;
		temp2 = temp2->next;
		counts += 1;
	}
	std::cout << counts << ":temp3: " << temp3 << ":temp2 " << temp2 << ":y: " << y << endl;
}
int heap::findindex(Node*y) {
	int count = -1;
	traverse = root;
	while (traverse->next != y->next) {
		//std::cout << traverse << "--";
		temp4 = traverse;
		traverse = traverse->next;
		count += 1;
	}	
	return count;
}
void heap::display() {
	traverse = root;
	std::cout << "display: ";
	while (traverse != nullptr) {
		std::cout << traverse->data<< ":" << traverse << "--";
		traverse = traverse->next;
	}
	if (traverse != nullptr) {
		std::cout << traverse->data << ":" << traverse << "--";
	}
}
void heap::travend(Node*y) {
	traverse = y;
	while (traverse->next != nullptr) {
		traverse = traverse->next;
	}
}

void patient::next() {
	H.next();
}
void heap::next() {
	temp5 = root->next;
	temp6 = temp5->next;
	root->next = temp6;
	size -= 1;
	display();
	build();
	display();
}
void heap::build() {
	for (int i = size; i > 0; i--) {
		traverse = root;
		for (int j = 0; j < i; j++) {
			traverse = traverse->next;
		}
		sort(traverse);
	}
}
void patient::display() {
	H.patdisplay();
	patientfile = hold;
	hold = nullptr;

	last = patientfile->below;
	first = last->below;
	priority = first->below;
	ticket = priority->below;
	complaint = ticket->below;
	symptom = complaint->below;

	std::cout << "patient record: " << last->cmd << ", " << first->cmd << endl;
	std::cout << "priority: " << priority->cmd << endl;
	std::cout << "ticket#: " << ticket->data << endl;
	std::cout << "complaint: " << complaint->cmd << endl;
	std::cout << "symptom(s): " << symptom->cmd << endl;
}
void heap::patdisplay() {
	hold = root->next;
}
void patient::fulldisplay() {
	H.fulldisplay();
}
void heap::fulldisplay() {
	std::cout << endl;
	hold = root;
	while (hold != nullptr) {
		temp = hold;
		while (temp != nullptr) {
			std::cout << temp << "--";
			try {
				std::cout << (temp->cmd) << " : ";
			}
			catch (...) {}
			temp = temp->below;
		}
		hold = hold->next;
		std::cout << endl;
	}
}
