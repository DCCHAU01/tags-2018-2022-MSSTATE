/*
	Program Description:
		Debugging a small program 2 part 2.
*/
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::cin;
using namespace std;

void print(string a, int b, int c);

int main() {
	string userStr = "Hello World";
	int x;
	int len = userStr.length();

	cout << "Enter a number: ";
	cin >> x;
	
	print(userStr, x, len);
}

void print(string a, int b, int c) {
	for ( int i = 0; i < c; i++) {
		if ((i + 1) % b == 0) {
			cout << a[i];
		}
	}
	
}
