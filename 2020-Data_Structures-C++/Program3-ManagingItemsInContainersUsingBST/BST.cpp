//============================================================================
//Compiler Used:Visual Studio
//Program Description:file containing BST and Node methods
//============================================================================
#include "BST.h"
#include <iostream>
#include <fstream>
#include <string>
using std::string;
using std::ofstream;

void BST::create(string cmd) {
	if (findcontainer(cmd) == true) {//executes if user tries to create a duplicate container
		std::cout << "duplicate: " << temp->cmd << " : not allowed" << endl;
		return;
	}
	if (start == nullptr) {//begin by creating the first container
		start = new Node(cmd);
		start->next = nullptr;
		root = start;
		temp = start;
		std::cout << "container " << cmd << " created\n";
	}
	else {
		temp = start;//loops until end or until a duplicate is found
		while ((temp->next != nullptr) && (temp->cmd != cmd)) {
			temp = temp->next;
		}
		if (temp->cmd == cmd) {//executes if dupilcate is found
			std::cout << "duplicate: " << temp->cmd << " : not allowed" << endl;
			return;
		}
		if (temp->next == nullptr) {//creates new container if duplicate is not found and when at end of traversing the list
			temp->next = new Node(cmd);
			temp = temp->next;
			std::cout << "container " << cmd << " created\n";
			temp->next = nullptr;
		}
	}
}
void BST::displaycontainer() {//loops through list of containers and displays them
	std::cout << endl;
	temp = start;
	std::cout << "container list: ";
	while (temp != nullptr) {
		std::cout << temp->cmd << ", ";
		temp = temp->next;
	}
	std::cout << endl;
	std::cout << endl;
}
void BST::insertinto(string item, int count, string container) {
	if (count == 0) {//if user tries to insert a zero amount of item
		std::cout << "there can't be 0 items in a container";
		return;
	}
	if (findcontainer(item) == true) {//makes sure you can't fit a closet inside a closet
		std::cout << "an item can't share a name with a container\n";
		return;
	}
	temp = start;//finds container in container list
	while (temp->cmd != container) {
		temp = temp->next;
	}
	root = temp;
	temp2 = root;//then attempts to insert item into that container
	if (insert(item) == true) {
		std::cout << "inserted item " << item << " into container " << container << endl;
	}
	temp2->data = count;//in the case that a duplicate item can't be inserted, the existing item's count is overwritten
	std::cout << endl;
}
bool BST::insert(string x) {
	if (findcontainer(x) == true) {//makes sure you can't fit a closet inside a closet
		std::cout << "an item can't share a name with a container\n";
		return false;
	}
	if (x < temp2->cmd) {//traverse left if item is less than parent and creates left child
		if (temp2->left == nullptr) {
			temp2->left = new Node(x);
			temp2 = temp2->left;
			return true;
		}
		else {
			temp2 = temp2->left;//if left child already exists, execute recursion
			return insert(x);
		}
	}
	if (x > temp2->cmd) {//traverse right if item is greater than parent and creates right child
		if (temp2->right == nullptr) {
			temp2->right = new Node(x);
			temp2 = temp2->right;
			return true;
		}
		else {
			temp2 = temp2->right;//if right child already exists, execute recursion
			return insert(x);
		}
	}
	if (x == temp2->cmd) {//executes if item already exists
		std::cout << "can't insert if " << x << " already exists\n";
		return false;
	}
	std::cout << endl;
}
bool BST::search(string x) {
	if (x < temp2->cmd) {//executes if item is less than parent
		if (temp2->left == nullptr) {//stops search if a dead end is hit and returns a message
			std::cout <<"item "<< x << " not found in container: " << temp->cmd << endl;
			return false;
		}
		else {
			temp2 = temp2->left;//moves to left child and recursively calls search
			return search(x);
		}
	} 
	if (x > temp2->cmd) {//executes if item is greater than parent
		if (temp2->right == nullptr) {//stops search if a dead end is hit and returns a message
			std::cout <<"item "<< x << " not found in container: " << temp->cmd << endl;
			return false;
		}
		else {
			temp2 = temp2->right;//moves to right child and recursively calls search
			return search(x);
		}
	}
	if (x == temp2->cmd) {//executes if item is found
		std::cout << "item " << x << " found in container: " << temp->cmd << " with count " << temp2->data << endl;
		return true;
	}
	std::cout << endl;
}
void BST::find(string x) {
	if (start == nullptr) {//executes if no there isn't a containoro to begin with
		std::cout << "find " << x << " has failed.  no tree exists" << endl;
		return;
	}
	temp = start;
	while (temp != nullptr) {//loops through container list and searches each container
		temp2 = temp;
		search(x);
		temp = temp->next;
	}
	std::cout << endl;
}
void BST::findin(string x, string y) {
	if (start == nullptr) {//executes if there is no tree to begin with
		std::cout << "find " << x << " has failed.  no tree exists" << endl;
		return;
	}
	if (findcontainer(y) == true) {//loops until container is found and searches it
		temp = start;
		while (temp->cmd != y) {
			temp = temp->next;
		}
		temp2 = temp;
		search(x);
	}
	else {
		return;
	}
}
bool BST::findcontainer(string y) {//finds container from container list
	temp = start;
	while (temp != nullptr) {
		if (temp->cmd == y) {
			return true;
		}
		else {
			hold = temp;//a placeholder for cases where node 1 needs to link with node 3 where node 2 is undergoing deletion
			temp = temp->next;
		}
	}
	return false;
}
bool BST::delet(string x) {
	if (x < temp2->cmd) {//goes left if item is less than parent
		if (temp2->left == nullptr) {//stops if there's nothing to delete 
			return false;
		}
		else {//moves to left child and calls recursion
			temp3 = temp2;
			temp2 = temp2->left;
			return delet(x);
		}
	}
	if (x > temp2->cmd) {//goes right if item is greater than parent
		if (temp2->right == nullptr) {//stops if there's nothing to delete
			return false;
		}
		else {
			temp3 = temp2;//moves to right child and calls recursion
			temp2 = temp2->right;
			return delet(x);
		}
	}
	if (x == temp2->cmd) {//executes when item is found
		countl = false;
		countr = false;
		if (temp2->left != nullptr) {//checks if there is a left child
			countl = true;
		}
		if (temp2->right != nullptr) {//checks if there is a right child
			countr = true;
		}
		if (countl == false) {//executes a zero degree delete
			if (countr == false) {//if there is no right or left child
				std::cout << "search is positive.  removing " << x << " from " << temp->cmd << endl;
				temp2 = nullptr;
			}
		}
		if ((countl == true) && (countr == false)) {//executes a one degree left focused delete
			templ = temp2->left;                    //if a left child is found
			temp2->left = nullptr;
			if (x < temp3->cmd) {//deletes x and links parent to x's left child
				temp3->left = templ;
				std::cout << "search is positive.  removing " << x << " from " << temp->cmd << endl;
				temp2 = nullptr;
			}
		}
		if ((countl == false) && (countr == true)) {//executes a one degree right focused delete
			tempr = temp2->right;                   //if a right child is found
			temp2->right = nullptr;
			if (x > temp3->cmd) {//deletes x and links parent to x's right child
				temp3->right = templ;
				std::cout << "search is positive.  removing " << x << " from " << temp->cmd << endl;
				temp2 = nullptr;
			}
		}
		if ((countl == true) && (countr == true)) {//executes a two degree delete
			countl = false;
			countr = false;
			temp4 = temp2->left;//begins process of deletion by first finding largest left child item
			if (temp4->left != nullptr) {
				countl = true;
			}
			if (temp4->right != nullptr) {
				countr = true;
			}
			if (countr = true) {//traverse right until a dead end is hit
				temp5 = temp4->right;
				while (temp5->right != nullptr) {
					temp6 = temp5;
					temp5 = temp5->right;
				}
				if (findleft(temp5) == true) {//links dead end's parent to dead end's left child
					temp6->right = temp5->left;
				}
				temp2->cmd = temp5->cmd;//swaps deleted item x with dead end's items and count
				temp2->data = temp5->data;
				std::cout << "search is positive.  removing " << x << " from " << temp->cmd << endl;
				temp5 = nullptr;//trashes dead end
			}
			if (countr = false) {//executes if left child doesn't have a right child
				if (findleft(temp4) == false) {//completes deletion if left child doesn't have a left child
					temp2->data = temp4->data;
					temp2->cmd = temp4->cmd;
					std::cout << "search is positive.  removing " << x << " from " << temp->cmd << endl;
					temp4 = nullptr;
				}
				else {//links parent to left child's left child and deletes left child
					temp2->data = temp4->data;
					temp2->cmd = temp4->cmd;
					temp2->left = temp4->left;
					std::cout << "search is positive.  removing " << x << " from " << temp->cmd << endl;
					temp4 = nullptr;
				}
			}
		}
		std::cout << endl;
		return true;
	}
}
void BST::remove(string x) {
	if (start == nullptr) {//executes if there isn't a container to begin with
		std::cout << "remove " << x << " has failed.  no tree exists" << endl;
		return;
	}
	if (findcontainer(x) == true) {//it's impossible to fit a closet inside the exact same closet
		std::cout << "can't remove container.  only items.  use destroy command to remove container\n";
		return;
	}
	if (findallx(x) == true) {//searches all containers and executes delete algorithm if item is found in any one of them
		temp = start;
		while (temp != nullptr) {
			temp2 = temp;
			delet(x);
			temp = temp->next;
		}
	}
	std::cout << endl;
}
void BST::removefrom(string x, string y) {
	if (start == nullptr) {//you can't remove if there's no container to remove from
		std::cout << "remove " << x << " has failed.  no tree exists" << endl;
		return;
	}
	if (x == y) {//you can't remove a closet from a closet
		std::cout << "can't remove container.  only items.  use destroy command to remove container\n";
		return;
	}
	if ((findcontainer(y) == true) && (search(x)==true)) {//executes delete algorithm if item is found in container
		temp = start;
		while (temp->cmd != y) {
			temp = temp->next;
		}
		temp2 = temp;
		delet(x);
	}
	std::cout << endl;
}
bool BST::findleft(Node*lef) {//checks if node has a left child
	if (lef->left != nullptr) {
		return true;
	}
	else {
		return false;
	}
}
bool BST::findright(Node*rig) {//checks if node has a right child
	if (rig->right != nullptr) {
		return true;
	}
	else {
		return false;
	}
}
bool BST::findallx(string x) { //loops over all containers to find item x and returns false if none is found
	bool test = false;
	temp = start;
	while (temp != nullptr) {
		temp2 = temp;
		if (search(x) == true) {
			test = true;
		}
		temp = temp->next;
	}
	if (test == true) {
		return true;
	}
	else { return false; }
}
void BST::displaypre(string y) {//prints preorder traversal if container is found
	if (findcontainer(y) == true) {
		std::cout << "container " << temp->cmd << " preorder traversal: ";
		pretraverse(temp);
		std::cout << endl;
	}
	else {
		std::cout << "container " << y << " not found\n";
		return;
	}
}
void BST::pretraverse(Node*y) {
	if (y == nullptr) {
		return;
	}
	std::cout << y->cmd << ", ";
	pretraverse(y->left);
	pretraverse(y->right);
}
void BST::displaypost(string y) {//prints post traversal if container is found
	if (findcontainer(y) == true) {
		std::cout << "container " << temp->cmd << " postorder traversal: ";
		postraverse(temp);
		std::cout << endl;
	}
	else {
		std::cout << "container " << y << " not found\n";
		return;
	}
}
void BST::postraverse(Node*y) {
	if (y == nullptr) {
		return;
	}
	postraverse(y->left);
	postraverse(y->right);
	std::cout << y->cmd << ", ";
}
void BST::displayin(string y) {//prints in order traversal if container is found
	if (findcontainer(y) == true) {
		std::cout << "container " << temp->cmd << " inorder traversal: ";
		intraverse(temp);
		std::cout << endl;
	}
	else {
		std::cout << "container " << y << " not found\n";
		return;
	}
}
void BST::intraverse(Node*y) {
	if (y == nullptr) {
		return;
	}
	intraverse(y->left);
	std::cout << y->cmd << ", ";
	intraverse(y->right);
} 
void BST::destroy(string container) {//executes if container is found
	if (findcontainer(container) == true) {
		if (container == start->cmd) {//handles deletion if the container being deleted is the first container
			hold = start;
			temp = start;
			hold = temp->next;
			start = hold;
		}
		else {
			if (temp->next != nullptr) {//handles deletion cases where previous is a container and next is also a container
				moveto = temp->next;
				hold->next = moveto;
			}
			if (temp->next == nullptr) {//handles deletion if container being deleted is last in line
				hold->next = nullptr;
			}
		}
		travdest(temp);//deletes container
	}
}
void BST::travdest(Node*y) {//
	if (y == nullptr) {//if there isn't a container to delete, returns nothing
		return;
	}
	travdest(y->left);
	travdest(y->right);
	y = nullptr;//utilizes post order traversal to delete container
}
