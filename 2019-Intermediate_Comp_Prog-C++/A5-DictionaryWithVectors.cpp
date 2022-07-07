/*
	Program Description:
		Input term and definition then outputs term with definition in a numbered list.
*/
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;

int main()
{
	int numt;
	vector<string> term;
	vector<string> def;
	
	cout << "How many terms do you want the dictionary to hold? ";
	cin >> numt;
	cout << endl;

	term.resize(numt);
	def.resize(numt);
	for (int i = 0; i < numt; i++)
	{
		cout << "Enter term " << (i + 1) << ": ";
		cin >> term.at(i);
		cout << "Enter the definition for " << term.at(i) << ": ";
		cin >> def.at(i);
		cout << endl;
	}

	cout << "You entered: " << endl;
	for (int i = 0; i < numt; i++)
	{
		cout << (i + 1) << ". " << term.at(i) << ": " << def.at(i) << endl;
	}
}
