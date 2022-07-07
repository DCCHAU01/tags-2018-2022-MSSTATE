/*********************************************************************
  Programming Environment: wsl ubuntu
  Purpose of File: contains production parsing nodes and interpreting functions
**********************************************************************/

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"
#include <cstring>

using namespace std;
//static int count = 0;		//originally used to identify line numbers for debugging

extern map<string, int> symbolTable;

// Forward declaration of Expression node
class ExpressionNode;

// Forward declaration of operator<< for ExprNode
ostream& operator<<(ostream& os, ExpressionNode& a);

//*****************************************************************************
// class FactorNode (Factor Node) (base for TerminalNode , nestedFactorNode, NestedExprNode)
class FactorNode {
public:
	virtual ~FactorNode();					// allows subclass destructors to be called first
	virtual void printTo(ostream& os) = 0;	// pure virtual method, makes the class Abstract
	virtual int interpret() = 0;
};

FactorNode::~FactorNode() {}

// overloads << operator.  uses double dispatch to get os from subclass
ostream& operator<<(ostream& os, FactorNode& a) {
	a.printTo(os);
	return os;
}
//*****************************************************************************
// class TerminalNode (int, float, identifier Node)
class TerminalNode : public FactorNode {
public:
	string* data = nullptr;

	TerminalNode(string a);
	~TerminalNode();
	void printTo(ostream& os);
	int interpret();
};

TerminalNode::TerminalNode(string a) {
	//cout << "Creating a factorNode1 : " << this << endl;
	data = new string(a);
}

TerminalNode::~TerminalNode() {
	//cout << "Deleting a factorNode1 : " << this << " : " << *data << endl;
	cout << "Deleting a factorNode\n";
	delete data;
	data = nullptr;
}

void TerminalNode::printTo(ostream& os) {
	os << "factor( ";
	try {
		//will run successfully if data can be converted into a number
		os << stof(*data);
	}
	catch (...) {
		//if data is not a number, then it is an identifier
		os << *data;
	}
	os << " )";
}

int TerminalNode::interpret() {
	try {
		//will return a number if data can be converted into a number
		return stof(*data);
	}
	catch (...) {
		//will return the value (in the symbol table) associated with the key: the identifier
		map<string, int>::iterator it;
		it = symbolTable.find(*data);
		return it->second;
	}
}
//*****************************************************************************
// class nestedFactorNode (Nested Factor Node Node) handles NOT and MINUS
class nestedFactorNode : public FactorNode{
public:
	string* data = nullptr;
	FactorNode* node = nullptr;

	nestedFactorNode(FactorNode* a, string b);
	~nestedFactorNode();
	void printTo(ostream& os);
	int interpret();
};

nestedFactorNode::nestedFactorNode(FactorNode* a, string b) {
	//cout << "Creating a factorNode2 : " << this << endl;
	node = a;
	data = new string(b);
}

nestedFactorNode::~nestedFactorNode() {
	//cout << "Deleting a factorNode2 : " << this << " : " << *data << endl;
	cout << "Deleting a factorNode\n";
	delete data;
	data = nullptr;
	delete node;
	node = nullptr;
}

void nestedFactorNode::printTo(ostream& os) {
	os << "factor( ";
	os << *data << " " << *node;
	os << " )";
}

int nestedFactorNode::interpret() {
	if (*data == "NOT") {
		//return the logical negation of the result
		return !node->interpret();
	}
	//return the negative version of the result
	return -node->interpret();
}
//*****************************************************************************
// class TermNode (Term Node)
class TermNode {
public:
	FactorNode* startFactor = nullptr;
	vector<string*> ops;
	vector<FactorNode*> restFactors;

	TermNode();
	~TermNode();
	int interpret();
};

TermNode::TermNode() {}

TermNode::~TermNode() {
	cout << "Deleting a termNode\n";
	delete startFactor;
	startFactor = nullptr;
	int length = ops.size();
	for (int i = 0; i < length; ++i) {
		delete ops[i];
		ops[i] = nullptr;
	}
	length = restFactors.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}

ostream& operator<<(ostream& os, TermNode& a) {
	os << "term( ";
	//do the first factor
	os << *(a.startFactor);
	//do the other factors after the first factor
	if (a.ops.empty() == false) {
		int length = a.ops.size();
		for (int i = 0; i < length; ++i) {
			os << " " << *(a.ops[i]) << " " << *(a.restFactors[i]);
		}
	}
	os << " )";
	return os;
}

int TermNode::interpret() {
	//f is the first factor
	int f = startFactor->interpret();
	if (ops.empty() == false) {
		int length = ops.size();
		//loops until all operators are used in calculating f 
		for (int i = 0; i < length; ++i) {
			//s is the second factor
			int s = restFactors[i]->interpret();
			if (*ops[i] == "*") {
				f *= s;
			}
			else if (*ops[i] == "/") {
				f /= s;
			}
			else if (*ops[i] == "AND") {
				if ((f != 0) && (s != 0)) {
					f = 1;
				}
				else {
					f = 0;
				}
			}
		}
	}
	return f;
}
//*****************************************************************************
// class SimpleExpressionNode (Simple Expression Node)
class SimpleExpressionNode {
public:
	TermNode* startTerm = nullptr;
	vector<string*> ops;
	vector<TermNode*> restTerms;

	SimpleExpressionNode();
	~SimpleExpressionNode();
	int interpret();
};

SimpleExpressionNode::SimpleExpressionNode() {}

SimpleExpressionNode::~SimpleExpressionNode() {
	cout << "Deleting a simpleExpressionNode\n";
	delete startTerm;
	startTerm = nullptr;
	int length = ops.size();
	for (int i = 0; i < length; ++i) {
		delete ops[i];
		ops[i] = nullptr;
	}
	length = restTerms.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}

}

ostream& operator<<(ostream& os, SimpleExpressionNode& a) {
	os << "simple_expression( ";
	os << *(a.startTerm);
	if (a.ops.empty() == false) {
		int length = a.ops.size();
		for (int i = 0; i < length; ++i) {
			os << " " << *(a.ops[i]) << " " << *(a.restTerms[i]);
		}
	}
	os << " )";
	return os;
}

int SimpleExpressionNode::interpret() {
	//f is the first factor
	int f = startTerm->interpret();
	if (ops.empty() == false) {
		int length = ops.size();
		//loops until all operators are used in calculating f 
		for (int i = 0; i < length; ++i) {
			//s is the second factor
			int s = restTerms[i]->interpret();
			if (*ops[i] == "+") {
				f += s;
			}
			else if (*ops[i] == "-") {
				f -= s;
			}
			else if (*ops[i] == "OR") {
				if ((f != 0) || (s != 0)) {
					f = 1;
				}
				else {
					f = 0;
				}
			}
		}
	}
	return f;
}
//*****************************************************************************
// class ExpressionNode (Expression Node)
class ExpressionNode {
public:
	SimpleExpressionNode* firstChild = nullptr;
	string* op = nullptr;
	SimpleExpressionNode* secondChild = nullptr;

	ExpressionNode();
	~ExpressionNode();
	int interpret();
};

ExpressionNode::ExpressionNode() {}

ExpressionNode::~ExpressionNode() {
	cout << "Deleting an expressionNode\n";
	delete firstChild;
	firstChild = nullptr;
	delete secondChild;
	secondChild = nullptr;
	delete op;
	op = nullptr;
}

ostream& operator<<(ostream& os, ExpressionNode& a) {
	os << "expression( ";
	os << *(a.firstChild);
	if (a.op != nullptr) {
		os << " " << *a.op << " ";
		os << *(a.secondChild);
	}
	os << " )";
	return os;
}

int ExpressionNode::interpret() {
	//f is the first factor
	int f = firstChild->interpret();
	//will calculate a single logical result if an operator exists in string* op
	if (op != nullptr) {
		//s is the second factor
		int s = secondChild->interpret();
		if (*op == "=") {
			return (f == s);
		}
		else if (*op == "<") {
			return (f < s);
		}
		else if (*op == ">") {
			return (f > s);
		}
		else if (*op == "<>") {
			return (f != s);
		}
	}
	return f;
}
//*****************************************************************************
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
	ExpressionNode* a = nullptr;

	NestedExprNode(ExpressionNode* b);
	~NestedExprNode();
	void printTo(ostream& os);
	int interpret();
};

NestedExprNode::NestedExprNode(ExpressionNode* b) {
	//cout << "Creating a factorNode3 : " << this << endl;
	a = b;
}

NestedExprNode::~NestedExprNode() {
	//cout << "Deleting a factorNode3 : " << this << " : " << *a << endl;
	cout << "Deleting a factorNode\n";
	delete a;
	a = nullptr;
}

void NestedExprNode::printTo(ostream& os) {
	os << "nested_expression( ";
	os << *a;
	os << " )";
}

int NestedExprNode::interpret() {
	//pass the baton to expression node's interpret
	return a->interpret();
}
//*****************************************************************************
// class StatementNode (Statement Node)
class StatementNode {
public:
	virtual ~StatementNode();
	virtual void printTo(ostream& os) = 0;//pure virtual method, makes class abstract
	virtual int interpret() = 0;
};

StatementNode::~StatementNode() {}
//uses double dispatch to get os from subclass
ostream& operator<<(ostream& os, StatementNode& a) {
	//count++;
	//os << "statement ran " << count << endl;
	a.printTo(os);
	//os << "printTo ran\n";
	return os;
}
//*****************************************************************************
// class NestedCompoundNode (Nested Compound Node)
class NestedCompoundNode : public StatementNode {
public:
	StatementNode* startStatement = nullptr;
	vector<StatementNode*> restStatements;

	NestedCompoundNode();
	~NestedCompoundNode();
	void printTo(ostream& os);
	int interpret();
};

NestedCompoundNode::NestedCompoundNode() {}

NestedCompoundNode::~NestedCompoundNode() {
	cout << "Deleting a compoundNode\n";
	delete startStatement;
	startStatement = nullptr;

	int length = restStatements.size();
	for (int i = 0; i < length; ++i) {
		delete restStatements[i];
		restStatements[i] = nullptr;
	}
}

void NestedCompoundNode::printTo(ostream& os) {
	os << "Begin Compound Statement\n";
	os << *(this->startStatement);

	int length = this->restStatements.size();
	//cout << "nestedcompound length: " << length << endl;
	for (int i = 0; i < length; ++i) {
		os << *(this->restStatements[i]);
	}
	os << "End Compound Statement\n";
}

int NestedCompoundNode::interpret() {
	//interpret the first statement
	startStatement->interpret();
	//interpret the rest of the statements
	int length = restStatements.size();
	for (int i = 0; i < length; ++i) {
		restStatements[i]->interpret();
	}
	return 1;
}
//*****************************************************************************
// class IfNode (If Node)
class IfNode : public StatementNode {
public://add handing for expression, then statement, and else statement
	ExpressionNode* startExpression = nullptr;
	StatementNode* startStatement = nullptr;
	vector<StatementNode*> restStatements;

	IfNode();
	~IfNode();
	void printTo(ostream& os);
	int interpret();
};

IfNode::IfNode() {}

IfNode::~IfNode() {
	cout << "Deleting an ifNode\n";
	delete startExpression;
	startExpression = nullptr;
	delete startStatement;
	startStatement = nullptr;

	int length = restStatements.size();
	for (int i = 0; i < length; ++i) {
		delete restStatements[i];
		restStatements[i] = nullptr;
	}
}

void IfNode::printTo(ostream& os) {
	os << "If "; //add to os for expression, then statement, and else statement
	os << *(this->startExpression) << endl;
	os << "%%%%%%%% True Statement %%%%%%%%\n";
	os << *(this->startStatement);
	cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";

	int length = this->restStatements.size();//could have made this a single object instead of a list of objects, but it works and i'm lazy
	if (length > 0) {
		os << "%%%%%%%% False Statement %%%%%%%%\n";
		for (int i = 0; i < length; ++i) {
			os << *(this->restStatements[i]);
		}
		os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
	}
	
}

int IfNode::interpret() {
	//evaluates the condition
	int f = startExpression->interpret();
	//if condition is true, interpret the true branch statement
	if (f != 0) {
		startStatement->interpret();
	}// if condition is false, interpret the false branch statement
	else {
		restStatements[0]->interpret();
	}
	return 1;
}
//*****************************************************************************
// class WhileNode (While Node)
class WhileNode : public StatementNode {
public://add handling for expression and statement
	ExpressionNode* startExpression = nullptr;  //i.e. a condition like i < 5
	StatementNode* startStatement = nullptr;    

	WhileNode();
	~WhileNode();
	void printTo(ostream& os);
	int interpret();
};

WhileNode::WhileNode() {}

WhileNode::~WhileNode() {
	cout << "Deleting a whileNode\n";
	delete startExpression;
	startExpression = nullptr;
	delete startStatement;
	startStatement = nullptr;
}

void WhileNode::printTo(ostream& os) {
	os << "While ";  //add os for expression and statement
	os << *(this->startExpression) << endl;
	os << "%%%%%%%% Loop Body %%%%%%%%\n";
	os << *this->startStatement;
	os << "%%%%%%%%%%%%%%%%%%%%%%%%%%%\n";
}

int WhileNode::interpret() {
	//while condition is evaluated to true, i.e. not zero, interpret the statement
	while (startExpression->interpret() != 0) {
		startStatement->interpret();
	}
	return 1;
}
//*****************************************************************************
// class ReadNode (Read Node)
class ReadNode : public StatementNode {
public:
	string* var = nullptr;

	ReadNode(string name);
	~ReadNode();
	void printTo(ostream& os);
	int interpret();
};

ReadNode::ReadNode(string name) {
	var = new string(name);
}

ReadNode::~ReadNode() {
	cout << "Deleting a readNode\n";
	delete var;
	var = nullptr;
}

void ReadNode::printTo(ostream& os) {
	os << "Read Value " << *var << endl;
}
//use cin to get input.  assign variable's value to the input
int ReadNode::interpret() { 
	int a;
	cin >> a;
	map<string, int>::iterator it;
	it = symbolTable.find(*var);
	it->second = a;
	return 1;
}
//*****************************************************************************
// class WriteNode (Write Node)
class WriteNode : public StatementNode {
public:
	string* var = nullptr;
	string type;

	WriteNode(string name, bool b);
	~WriteNode();
	void printTo(ostream& os);
	int interpret();
};

WriteNode::WriteNode(string name, bool b) {
	var = new string(name);
	if (b == true) {
		type = "String ";
	}
	else {
		type = "Value ";
	}
}

WriteNode::~WriteNode() {
	cout << "Deleting a writeNode\n";
	delete var;
	var = nullptr;
	type = '0';
}

void WriteNode::printTo(ostream& os) {
	os << "Write " << type << *var << endl;
}

int WriteNode::interpret() {
	//this clunky block of code prints from the 1st element to the next to last element of the string. 
	//this is because the apostrophes at each end were included in string* var
	if (type == "String ") {
		int length = var->length();
		char myArray[length + 1];
		strcpy(myArray, var->c_str());
		for (int i = 1; i < length - 1; i++) {
			cout << myArray[i];
		}
		cout << "\n";
	}//prints the value associated with the identifier
	else {
		map<string, int>::iterator it;
		it = symbolTable.find(*var);
		cout << it->second << "\n";
	}
	return 1;
}
//*****************************************************************************
// class AssignmentNode (Assignment Node)
class AssignmentNode : public StatementNode {
public:
	string* var = nullptr;
	ExpressionNode* startExpression = nullptr;

	AssignmentNode(string name);
	~AssignmentNode();
	void printTo(ostream& os);
	int interpret();
};

AssignmentNode::AssignmentNode(string name) {
	var = new string(name);
}

AssignmentNode::~AssignmentNode() {
	cout << "Deleting an assignmentNode\n";
	delete var;
	var = nullptr;
	delete startExpression;
	startExpression = nullptr;
}

void AssignmentNode::printTo(ostream& os) {
	os << "Assignment " << *var << " := ";
	os << *(this->startExpression);
	os << endl;
}
//finds the identifier in symboltable and assigns a int value to it
int AssignmentNode::interpret() {
	map<string, int>::iterator it;
	it = symbolTable.find(*var);
	it->second = startExpression->interpret();
	//cout << *var << " : " << it->second << "\n";
	return 1;
}
//*****************************************************************************
// class CompoundNode (Compound Node)
class CompoundNode {
public:
	StatementNode* startStatement = nullptr;
	vector<StatementNode*> restStatements;

	CompoundNode();
	~CompoundNode();
	int interpret();
};

CompoundNode::CompoundNode() {}

CompoundNode::~CompoundNode() {
	cout << "Deleting a compoundNode\n";
	delete startStatement;
	startStatement = nullptr;

	int length = restStatements.size();
	for (int i = 0; i < length; ++i) {
		delete restStatements[i];
		restStatements[i] = nullptr;
	}
}

ostream& operator<<(ostream& os, CompoundNode& a) {
	os << "Begin Compound Statement\n";
	os << *(a.startStatement);

	int length = a.restStatements.size();
	for (int i = 0; i < length; ++i) {
		os << *(a.restStatements[i]);
	}
	os << "End Compound Statement\n";
	return os;
}

int CompoundNode::interpret() {
	//interpret the first statement
	startStatement->interpret();
	//interpret the rest of the statements
	int length = restStatements.size();
	for (int i = 0; i < length; ++i) {
		restStatements[i]->interpret();
	}
	return 1;
}
//*****************************************************************************
//class BlockNode (Block Node)
class BlockNode {
public:
	CompoundNode* startCompound = nullptr;

	BlockNode();
	~BlockNode();
	int interpret();
};

BlockNode::BlockNode() {}

BlockNode::~BlockNode() {
	cout << "Deleting a blockNode\n";
	delete startCompound;
	startCompound = nullptr;
}

ostream& operator<<(ostream& os, BlockNode& a) {
	//os << "Entering Block " << endl;
	os << *(a.startCompound);
	return os;
}

int BlockNode::interpret() {
	//interpret the compound
	startCompound->interpret();
	return 1;
}
//*****************************************************************************
// class ProgramNode (Program Node)
class ProgramNode {
public:
	string* program_name = nullptr;
	BlockNode* startBlock = nullptr;

	ProgramNode(string name);
	~ProgramNode();
	int interpret();
};

ProgramNode::ProgramNode(string name) { //create node that stores program name
	program_name = new string(name);
}

ProgramNode::~ProgramNode() {
	cout << "Deleting a programNode\n";
	delete program_name;
	program_name = nullptr;
	delete startBlock;
	startBlock = nullptr;
}

ostream& operator<<(ostream& os, ProgramNode& a) {
	os << "Program Name " << *(a.program_name) << endl;
	os << *(a.startBlock);
	return os;
}

int ProgramNode::interpret() {
	//interpret the block
	startBlock->interpret();
	return 1;
}
#endif /* PARSE_TREE_NODES_H */
