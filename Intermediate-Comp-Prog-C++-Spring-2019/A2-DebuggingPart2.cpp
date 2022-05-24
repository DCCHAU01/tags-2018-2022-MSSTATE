/*
	Program Description:
		Debugging a small program part 2.
*/
#include <iostream> 

using std::cout;
using std::endl;

int main() 
{
	int length = 4, width = 8;
	int area_rect = length * width;  
	int parameter = 2 * length + 2 * width;

	cout << "The Rectangle's dimensions are: " << length << " x " << width << endl;  
	cout << "The area of this rectangle is: " << area_rect << endl;  
	cout << "The parameter of the rectangle is: " << parameter << endl;
	cout << endl << endl;

	int base = 3, height = 4;
	int area_tri = (base * height) / 2;

	cout << "The Triangle's dimensions are: Base - ";  
	cout << base << ", Height - " << height << endl;  
	cout << "The area of this triangle is: " << area_tri;
}
