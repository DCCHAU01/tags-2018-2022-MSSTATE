/*
	Program Description:
		Area of circle formula
*/
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
using std::cin;

double aoe(double radius);  

int main() {      
	double r;
	double AREA;

	cout << "Find Area of Circle Calculator" << endl;
    
	cout << "Enter Radius of Circle: ";
	cin >> r;

	AREA = aoe(r);

	cout << "area is: " << AREA << endl;
}

double aoe(double radius) {
	double pi = 3.14;

	double Area_of_Circle = pi * (pow(radius,2));

	return Area_of_Circle;
}
