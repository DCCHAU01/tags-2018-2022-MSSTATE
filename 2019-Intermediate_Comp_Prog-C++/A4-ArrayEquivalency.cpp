/*
	Program Description:
		Testing two arrays for equivalency
*/
#include <iostream>
#include <list>
using namespace std;

bool equalTest(int num1, int list1[], int num2, int list2[]);

int main()
{
	const int num1 = 5; const int num2 = 5;
	const int num3 = 6; const int num4 = 5; 

	int i = 0;
	int i1 = 5;
	int i2 = 5;
	int i3 = 6;
	int i4 = 7;
	int list1[num1] = {10, 20, 30, 40, 50};
	int list2[num2] = {10, 20, 30, 40, 50};
	int list3[num3] = {10, 20, 30, 40, 50, 60};
	int list4[num4] = {50, 40, 30, 20, 10};

	/*
	for (i = 0; i < num1; ++i) {
		cout << "Value " << i + 1 << ": ";
		cin >> list1[i];
	}
	for (i = 0; i < num2; ++i) {
		cout << "Value " << i + 1 << ": ";
		cin >> list2[i];
	}
	for (i = 0; i < num3; ++i) {
		cout << "Value " << i + 1 << ": ";
		cin >> list3[i];
	}
	for (i = 0; i < num4; ++i) {
		cout << "Value " << i + 1 << ": ";
		cin >> list4[i];
	}

	if (num1%num2 == 0) {
		cout << "yes \n";
	}
	else {
		cout << "no; \n";
	}

	for (i = 0; i < num1; ++i) {
		if (list1[i] == list4[i]) {
			cout << "match" << endl;
		}
		else {
			cout << "error" << endl;
		}

	}
	*/
	equalTest(num1, list1, num2, list2);
	equalTest(num2, list2, num3, list3);
	equalTest(num3, list3, num4, list4);
	equalTest(num1, list1, num4, list4);
}

bool equalTest(int num1, int list1[], int num2, int list2[]) {
	for (int i = 0; i < num1; ++i)
	{
		cout << list1[i] << ", ";
	}
	cout << "size is: " << num1 << endl;
	for (int i = 0; i < num2; ++i)
	{
		cout << list2[i] << ", ";
	}
	cout << "size is: " << num2<< endl;

	if (num1 == num2) {
		cout << "Array size matches \n";
		cout << "processing...\n\n";
		

		int qualifier = 0;
		int disqualifier = 0;

		for (int i = 0; i < num1; ++i) {
			int count = 0;
			while (count < 1) {
				if (list1[i] != list2[i]) {
					cout << "Array elements " << list1[i] << " and " << list2[i] << " do not match " << endl;
				} else {
					cout << "Array elements " << list1[i] << " and " << list2[i] << " do match " << endl;
					qualifier += 1;
				}
				count += 1;
				
			}
		}
		if (qualifier == num1) {
			cout << "All elements match, thus arrays are equal " << endl << endl;
			return true;
		}
		if (qualifier != num1) {
			cout << "Not all elements match, thus arrays are not equal " << endl << endl;
			return false;
		}
	} else {
		cout << "Array sizes do not match  " << endl;
		cout << "Thus, Arrays are not equal " << endl << endl;
		return false;
	}
}
