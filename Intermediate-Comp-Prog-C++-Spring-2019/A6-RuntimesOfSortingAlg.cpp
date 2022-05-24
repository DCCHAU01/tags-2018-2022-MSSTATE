/*
	Program Description:
		Sorting lists with sorting algorithms
*/
#include <iostream>
#include <chrono>

using namespace std::chrono;
using namespace std;

long long calc_time();
void print_array(int arr[], int size);

void setS(int n);
void setQ(int n);

void select(int arr[], int n);
void quick(int arr[], int n);
template<class T>
void quick_sort(T* arr, int size);

int * sorted(int arr[], int n);
int * reversed(int arr[], int n);
int * random(int n);

int main()
{
	int n1 = 10000;
	int n2 = 50000;
	int n3 = 100000;

	cout << "Selection Sort Runtime \n";
	setS(n1);
	cout << endl;
	setS(n2);
	cout << endl;
	setS(n3);
	cout << endl;
	cout << "Quick Sort Runtime \n";
	setQ(n1);
	cout << endl;
	setQ(n2);
	cout << endl;
	setQ(n3);
	cout << endl;

	return 0;
}

void print_array(int arr[], int size)
{

	for (int i = 0; i < size; i++)
	{
		cout << arr[i];
		if (i < size - 1)
		{
			cout << ", ";
		}
	}
	cout << endl;
}

long long calc_time()
{
	microseconds ms = duration_cast<microseconds>(system_clock::now().time_since_epoch());
	return ms.count();
}

void select(int arr[], int n)
{
	long long a = calc_time();
	for (int i = 0; i < n; i++)
	{
		int mark = i;
		for (int j = i + 1; j < n; j++)
		{
			if (arr[mark] > arr[j])
			{
				mark = j;
			}
		}
		swap(arr[i], arr[mark]);
	}
	long long b = calc_time();
	cout << (b - a) << endl;
}

int * sorted(int arr[], int n)
{
	for (int i = 0; i < n; i++)
	{
		int mark = i;
		for (int j = i + 1; j < n; j++)
		{
			if (arr[mark] > arr[j])
			{
				mark = j;
			}
		}
		swap(arr[i], arr[mark]);
	}
	return arr;
}

int * reversed(int arr[], int n)
{
	for (int i = 0; i < n; i++)
	{
		int mark = i;
		for (int j = i + 1; j < n; j++)
		{
			if (arr[mark] < arr[j])
			{
				mark = j;
			}
		}
		swap(arr[i], arr[mark]);
	}
	return arr;
}

int * random(int n)
{
	int size = n;
	int* arr = new int[size];

	for (int i = 0; i < size; i++)
	{
		arr[i] = rand() % n;
	}
	return arr;
}

void setS(int n)
{
	int* list1 = sorted(random(n), n);
	int* list2 = reversed(random(n), n);
	int* list3 = random(n);
	int* list4 = random(n);
	int* list5 = random(n);

	select(list1, n);
	select(list2, n);
	select(list3, n);
	select(list4, n);
	select(list5, n);
}

void setQ(int n)
{
	int* list1 = sorted(random(n), n);
	int* list2 = reversed(random(n), n);
	int* list3 = random(n);
	int* list4 = random(n);
	int* list5 = random(n);

	quick(list1, n);
	quick(list2, n);
	quick(list3, n);
	quick(list4, n);
	quick(list5, n);
}

template<class T>
int partition(T* arr, int start, int finish)
{
	int p_index = (start + finish) / 2;
	int pivot_value = arr[p_index];
	int new_location = start - 1;

	swap(arr[p_index], arr[finish]);

	for (int i = start; i < finish; i++)
	{
		if (arr[i] <= pivot_value)
		{
			new_location++;
			swap(arr[new_location], arr[i]);
		}
	}
	swap(arr[new_location + 1], arr[finish]);
	return new_location + 1;
}

template<class T>
void quick_sort(T* arr, int start, int finish)
{
	if (start < finish)
	{
		int p_index = partition(arr, start, finish);
		quick_sort(arr, start, p_index - 1);
		quick_sort(arr, p_index + 1, finish);
	}
}

template<class T>
void quick_sort(T* arr, int size)
{
	long long a = calc_time();

	quick_sort(arr, 0, (size - 1));

	long long b = calc_time();
	cout << (b - a) << endl;	
}

void quick(int arr[], int n)
{
	quick_sort<int>(arr, n);
}
