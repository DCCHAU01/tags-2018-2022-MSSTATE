// P4-ParallelizedRadixSort-Consumer.c
/************************************************************************
Date: 11/14/2021
os: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Role: Consumer
Desc: reads data file from producer, performs a parallelized radix sort, and writes the sorted integers into a new file
The Project Assignment:
In broad strokes, Program 2 should do the following:

1 Read data from "data.dat" into an integer array
2 Determine the smallest element of data
3 Determine the largest element of data
4 Determine the total number of integers read.
5 Sort the array of integers with Radix Sort
6 Output the contents of the sorted array into the file "sorted.dat"
 Much of how you achieve this is left to the programmer.

These are the parts that aren't -- you should implement this program using multiple threads.
You should have at least three types of threads: the main thread, a processor and a sorter.

The main thread must perform the following:
     spawn the processor
     join the processor

The processor thread must perform the following:
     determine how many sorter threads are necessary
     spawn each sorter thread
     join each sorter thread
     combine the results of the sorter threads into the final sorted list
Note: There should be one sorter  thread per digit in the largest number in the list. 
Basically, you are calling a sorter thread for 1 digit numbers, 2 digit numbers, 3 digit numbers, etc.
Make certain to pass each sorter thread appropriate parameters to identify the range of numbers they are 
supposed to sort.

A sorter thread must perform the following:
     copy all values of the proper length into a new array
     perform the radix sort on the new array
     set the exit status of the thread to the new array and its size.
************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pred = 0;//a predicate variable that fell out of use
pthread_t *syncc;//a predicate variable that depends on matching thread id
int syncount = 0;

//arguably unnecessary struct stores pointer to array
struct args
{
	int *arr;
}A;
//handles error numbers-------------------
void hndlError(int error, const char *str)
{
  if (error == 0) return;
  errno = error;
  perror(str);
  exit(EXIT_FAILURE);
}
//system print function-----------------------------
void myPrint(const char *str)
{
  if (write(STDOUT_FILENO, str, strlen(str)) == -1)
    {
      perror("write");
      exit(EXIT_FAILURE);
    }
}
//computes number of digits in a number
int numDigits(const int val)
{
	int b = val;
	int d = 0;
	int i;
	if (val == 0){
		return 1;
	}
	while (b != 0){
		b /= 10;
		d++;
	}
	return d;
}
//finds the min of array that stores its own size in its first index
int findMin(int *arr, int len){
	int min, temp;
	min = arr[1];
	for (int i = 2; i < len; i++){
		temp = arr[i];
		if (temp < min){
			min = temp;
		}
	}
	return min;
}
//finds max of array that stores its own size in its first index
int findMax(int *arr, int len){
	//printf("len: %d\n", len);
	int max, temp;
	max = arr[1];
	for (int i = 2; i < len; i++){
		temp = arr[i];
		if (temp > max){
			max = temp;
		}
	}
	return max;
}
//finds the max of an array------------
int findMaxnoBias(int *arr, int len){
	//printf("len: %d\n", len);
	int max, temp;
	max = arr[0];
	for (int i = 1; i < len; i++){
		temp = arr[i];
		if (temp > max){
			max = temp;
		}
	}
	return max;
}
//prints array--------------------------------
void myprintArray(int *arr, int len){
	for (int i = 0; i < len; i++){
		printf("%d: %d\n", i, arr[i]);
	}
	return;
}
//finds min index number with range number of digits
int findMinRange(int *arr, int len, int range){
	int i = 1;
	while (1){
		if (numDigits(arr[i]) == range){
			return i;
		} else if (numDigits(arr[i]) != range){
			i++;
		}
	}
}
//finds max index number with range number of digits
int findMaxRange(int *arr, int len, int range){
	int i = findMinRange(arr, len, range);
	while (1){
		if (i >= len){
			return i - 1;
		}
		if (numDigits(arr[i]) == range){
			i++;
		} else if (numDigits(arr[i]) != range){
			return i - 1;
		}
	}
}
//an unnecessary pow function-----
int myPow(int a, int b)
{
	int c = a;
	for (int i = 1; i < b; i++)
	{
		c = c * a;
	}
	if (b == 1){return a;}
	if (b == 0){return 1;}
	return c;
}
//radixSort & countSort from geeksforgeeks.org/radix-sort
int *countSort(int *arr, int len, int exp){
	int count[10] = {0};
	int output[len];
	for (int i = 0; i < len; i++){
		count[(arr[i]/exp) % 10]++;
	}
	for (int i = 1; i < 10; i++){
		count[i] += count[i-1];
	}
	for (int i = len-1; i >= 0; i--){
		output[count[(arr[i]/exp)%10]-1] = arr[i];
		count[(arr[i]/exp)%10]--;
	}
	for (int i = 0; i < len; i++){
		arr[i] = output[i];
		//printf(" : %d %d : \n", i, arr[i]);
	}
	return arr;

}
int *radixSort(int *arr, int len){
	//myPrint("RadixSort\n");
	int max = findMaxnoBias(arr, len);
	//int i = 1;
	for (int exp = 1; max/exp>0; exp *= 10){
		//printf("Radix Running: %d\n", i);
		arr = countSort(arr, len, exp);
		//i++;
	}
	return arr;
}
//the sorter thread
//responsible for sorting the data
//the input array stores its size in the first slot
void *threadFuncC(void *a){
	pthread_mutex_lock(&mtx);
	while (pthread_self() != syncc[syncount]){
		pthread_cond_wait(&cond, &mtx);
	}
	syncount++;
	int d = *(int*)a;
	//printf("d: %d\n", d);
	int k = 1, i = 1;
	//make array[0] store size and array[1] store digitNum
	int min = findMinRange(A.arr, A.arr[0], d);
	int max = findMaxRange(A.arr, A.arr[0], d);
	int nsize = max-min+1+1;//another +1 to make space for storing size
	//printf("min: %d\n max: %d\n nsize: %d\n", min, max, nsize);
	int *arr = (int *) malloc(11*(sizeof(int)));
	arr[0] = nsize;
	int t = 0;
	int size = 11;
	while (t != 1){
		if (k == size){
			size = size * 2;
			arr = (int*) realloc(arr, size*(sizeof(int)));
		}
		if (k == nsize){
			t = 1;
		}
		if (numDigits(A.arr[i]) == d){
			arr[k] = A.arr[i];
			//printf("%d: %d\n", i, arr[k]);
			k++;
		}
		i++;
	}
	arr = (int*) realloc(arr, k*(sizeof(int))+1);
	pthread_mutex_unlock(&mtx);
	pthread_cond_broadcast(&cond);
	int len = arr[0]-1;
	int *temp = (int*)malloc(len*sizeof(int));
	for (int i = 0; i < len; i++){//copy array to temp, excluding length stored in arr[0]
		temp[i] = arr[i+1];
	}
	radixSort(temp, len);
	for (int i = 0; i < len+1; i++){
		arr[i+1] = temp[i];
	}
	//myprintArray(arr, arr[0]);
	pthread_exit((void*)arr);
}
//the processor thread
//determines number of sorter threads by subtracing number of digits in the largest number by the number of digits in the smallest number
//spawns sorter threads and joins sorter threads
//forces threads to order their returning values and writes to sorted.dat
void *threadFuncB(){
	int t;
	int i = 1;
	int size = 11;
	int *arr = (int*) calloc(size,sizeof(int));
	arr[0] = 0;
	int fd = open("data.dat", O_RDONLY, S_IRUSR | S_IWUSR);
	while (read(fd, &t, sizeof(t)) != 0){
		if (i < size){
			arr[i] = t;
			//printf("%d: %d\n", i-1, arr[i]);
			i++;
		} else if (i == size){
			size = 2*size;
			arr = (int*) realloc(arr, size*(sizeof(int)));
			arr[i] = t;
			//printf("%d: %d\n", i-1, arr[i]);
			i++;
		}
	}
	arr[0] = i;//store size of array in first slot
	arr = (int*) realloc(arr, i*(sizeof(int)));
	//printf("array size: %d\n", arr[0]);
	//myprintArray(arr, i);
	int min = findMin(arr, i);
	int max = findMax(arr, i);
	//printf("min: %d\nmax: %d\n", min, max);
	int minDigits = numDigits(min);
	int maxDigits = numDigits(max);
	//printf("minD: %d\nmaxD: %d\n", minDigits,maxDigits);
	int numSortThreads = maxDigits - minDigits + 1;
	pthread_t tid[numSortThreads];
	syncc = tid;
	int dig[numSortThreads];
	int offset = 0;
	int s;
	A.arr = arr;
	void *z;
	int *newarr;
	pred = 1;
	for (int j = minDigits; j <= maxDigits; j++){
		offset = j - minDigits;
		dig[offset] = j;
		s = pthread_create(&tid[offset], NULL, &threadFuncC, &dig[offset]);
		//printf("Running thread %d: %ld\n", offset, tid[offset]);
		hndlError(s, "create output thread fail\n");
		//pthread_join(tid[offset], &z);
		//newarr = (int*)z;
		//myprintArray(newarr, newarr[0]);
	}
	int flags = O_CREAT | O_WRONLY | O_RDONLY | O_TRUNC;
	mode_t perms = S_IRUSR | S_IWUSR;
	int output = open("sorted.dat", flags, perms);
	if (output == -1)
	{
		perror("open for write");
		exit(EXIT_FAILURE);
	}
	int writec = 0;
	for (int j = 0; j < numSortThreads; j++){
		pthread_join(tid[j], &z);
		newarr = (int*)z;
		for (int l = 1; l < newarr[0]; l++){
			writec = newarr[l];
			write(output, &writec, sizeof(int));
		}
		//myprintArray(newarr, newarr[0]);
		
	}
	close(fd);
	pthread_exit(NULL);
}
//the main thread
//spawns the processor and joins the processor
void *threadFuncA(){
	pthread_t ptid;
	int s;
	s = pthread_create(&ptid, NULL, &threadFuncB, NULL);
	hndlError(s, "create processor thread fail\n");
	pthread_join(ptid, NULL);
	pthread_exit(NULL);
}
//the main function
//creates main thread and outputs sorted data
int main(int argc, char *argv[])
{
	pthread_t mtid;
	int s;
	s = pthread_create(&mtid, NULL, &threadFuncA, NULL);
	hndlError(s, "create main thread fail\n");
	pthread_join(mtid, NULL);
	myPrint("Sorted data.dat and written to sorted.dat.  \nPlease wait a moment to see the sorted data.\n");
	sleep(2);
	int t;
	int i = 0;
	int fd = open("sorted.dat", O_RDONLY, S_IRUSR | S_IWUSR);
	while (read(fd, &t, sizeof(t)) != 0){
		printf("%d: %d\n", i, t);
		i++;
	}
	//printf("i: %d\n", i);
	exit(EXIT_SUCCESS);
}
