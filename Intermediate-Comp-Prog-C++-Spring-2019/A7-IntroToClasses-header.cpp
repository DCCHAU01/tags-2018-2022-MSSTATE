#pragma once
/*
	Program Description:
		The class file for main used to manipule the time variables by hours, minutes, and seconds
*/
#include <iostream>
#include <string>

using namespace std;

class Time
{
private:
	int hours;
	int minutes;
	int seconds;
public:
	Time(int seconds = 0)
	{
		hours = 0;
		minutes = 0;
		seconds = 0;
	}

	int GetHour()
	{
		return hours;
	}

	int GetMinute()
	{
		return minutes;
	}

	int GetSecond()
	{
		return seconds;
	}

	void SetTimeBySeconds(int second)
	{
		seconds = second;
		hours = seconds / 3600;
		seconds = seconds % 3600;
		minutes = seconds / 60;
		seconds = seconds % 60;
		cout << "Time is set to: " << ToString() << endl;
	}

	void IncreaseTimeBySeconds(int second)
	{
		cout << hours << ":" << minutes << ":" << seconds;
		cout << " + ";
		int* array1 = new int[3];
		array1[1] = second / 3600;
		array1[2] = second % 3600 / 60;
		array1[3] = second % 3600 % 60;
		cout << array1[1] << ":" << array1[2] << ":" << array1[3];

		seconds += second;
		while (seconds >= 60)
		{
			minutes += 1;
			seconds -= 60;
		}
		while (minutes >= 60)
		{
			hours += 1;
			minutes -= 60;
		}

		cout << " = " << ToString() << endl;
	}

	void DecreaseTimeBySeconds(int second)
	{
		cout << hours << ":" << minutes << ":" << seconds;
		cout << " - ";
		int* array2 = new int[3];
		array2[1] = second / 3600;
		array2[2] = second % 3600 / 60;
		array2[3] = second % 3600 % 60;
		cout << array2[1] << ":" << array2[2] << ":" << array2[3];

		seconds -= second;
		while (seconds < 0)
		{
			minutes -= 1;
			seconds += 60;
		}
		while (minutes < 0)
		{
			hours -= 1;
			minutes += 60;
		}
		if (hours < 0)
		{
			hours = 0;
			minutes = 0;
			seconds = 0;
		}

		cout << " = " << ToString() << endl;
	}

	string ToString()
	{
		string a = "";
		 a += to_string(hours);
		 a += ":";
		 a += to_string(minutes);
		 a += ":";
		 a += to_string(seconds);
		 return a;
	}

};
