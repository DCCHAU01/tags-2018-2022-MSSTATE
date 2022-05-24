/*
	Notes: while looking through Quora, I encountered: 
	"What is the most useful code comment you've seen?"
	Needless to say, I scrolled through the comments.
	I do hope I do not get screwed over.  
	If I do, you may have to deal with code that acts like naughty children.
	And I may have to add a clause stating that the code is mine while in production.
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
