/*
	Program Description:
		Input term and definition then outputs term with definition in a numbered list.
*/
#include <iostream>
#include <string>

using namespace std;

int num(int& numt);
void input(string* &array1, string* &array2, int numt);
void output(string* &array1, string* &array2, int numt);

int main()
{
	int numt;
	num(numt);
	
	string* array1;
	string* array2;

	input(array1, array2, numt);
	output(array1, array2, numt);

	delete[] array1;
	delete[] array2;
}

int num(int& numt) 
{
	cout << "How many terms do you want the dictionary to hold? ";
	cin >> numt;

	while (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please enter a Number: ";
		cin >> numt;
	}
	return numt;
}

void input(string* &array1, string* &array2, int numt)
{
	string* term = new string[numt];
	string* def = new string[numt];
	array1 = term;
	array2 = def;

	cout << endl;
	cin.ignore();

	for (int i = 0; i < numt; i++)
	{
		cout << "Enter term " << (i + 1) << ": ";
		getline(cin, term[i]);

		cout << "Enter the definition for \"" << term[i] << "\": ";
		getline(cin, def[i]);

		cout << endl;
	}
}

void output(string* &array1, string* &array2, int numt)
{
	cout << endl << "You entered: " << endl;
	for (int i = 0; i < numt; i++)
	{
		cout << (i + 1) << ". " << array1[i] << ": " << array2[i] << endl;
	}
}
