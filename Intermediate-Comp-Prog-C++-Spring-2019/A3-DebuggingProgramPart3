/*
	Program Description:
		Debugging a small program 2 part 1.
*/
#include <iostream>

using std::cout;
using std::endl;
using std::cin;

double liter_to_quart(double liters);  //Prototype 

int main() {
	//Decalre two unitialized variables, liters and quarts.       
	double liters, quarts;

	//Prompt the user with information about the program. 
	cout << "Welcome to Quart Conversions!" << endl;

	//Get the volume in liters from the user.       
	cout << "Enter the volume of your liquid in liters(l): ";
	cin >> liters;

	//Call the conversion function to get volume in quarts 
	quarts = liter_to_quart(liters);

	//Output the number of quarts 
	cout << "The number of quarts in " << liters;
	cout << " (l) is " << quarts << " (qt)." << endl;
}

//Converts a given volume in liters to quarts. 
double liter_to_quart(double liters_arg)
{
	double to_quart;

	to_quart = liters_arg * 1.057;

	return to_quart;
}
