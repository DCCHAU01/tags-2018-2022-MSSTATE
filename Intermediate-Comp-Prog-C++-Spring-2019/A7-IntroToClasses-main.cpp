/*
	Program Description:
		program that mimics several functions/actions of a clock
*/
#include "Time.h"
#include <iostream>
#include <string>

using namespace std;

int main()
{
	Time t1, t2, t3; 
	t1.SetTimeBySeconds(9889);
	t1.IncreaseTimeBySeconds(194873);
	t1.DecreaseTimeBySeconds(136903);
	t1.DecreaseTimeBySeconds(999999);
	cout << "Final Time is: " << t1.ToString() << "\n\n";

	t2.SetTimeBySeconds(134753);
	t2.IncreaseTimeBySeconds(194873);
	t2.DecreaseTimeBySeconds(136903);
	t2.DecreaseTimeBySeconds(999999);
	cout << "Final Time is: " << t2.ToString() << "\n\n";

	t3.SetTimeBySeconds(7859940);
	t3.IncreaseTimeBySeconds(9087576);
	t3.DecreaseTimeBySeconds(7835698);
	t3.DecreaseTimeBySeconds(9999999);
	cout << "Final Time is: " << t3.ToString() << "\n\n";
}
