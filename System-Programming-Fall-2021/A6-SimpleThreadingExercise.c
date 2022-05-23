/* A6-SimpleThreadingExercise.c */
/************************************************************************
Notes: I read on Quora "Do programmers read long code comments".
	 I arrived to the conclusion that comments are better used for
	 "Why I did this".
Date due: 10/20/2021
OS: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Desc: creates 2 threads.  
Thread A outputs even numbers 0 to 60.  0, 2, 4...
Thread B outputs odd number 1 until greater than 60.  1, 3, 5...
When executed, will output 0 to 60 in increasing order.  1, 2, 3...
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>

int limit = 60;

struct argVals//needed to access value in thread
{
	int integer;
};
//system print-----------------------------------------------------------
int myPrint(const char *str)
{
 	int i = 0;
 	int numBytes;
 	if ((numBytes = write(STDOUT_FILENO, str, strlen(str))) > 0){ 
 		return 1;
	}
	if (numBytes == -1) {return 0;}
}

int myPrintInt(const int val)
{
	int b = val;
	int d = 0;
	int i;
	if (val == 0){
		char str[1];
		str[0] = 0 + '0';
		return myPrint(str);
	}
	while (b > 0){
		b /= 10;
		d++;
	}
	char str[d];
	str[d] = '\0';
	b = val;
	for (i = d-1; i >= 0; i--){
		str[i] = (b % 10) + '0';
		b = b / 10;
	}
	return myPrint(str);
}
//-----------------------------------------------------------------------
//instead of making one thread for even integers and one thread for odd
//integers, I decided to make one function that takes in even or odd input then incrementing that twice.
void *threadFunc(void *arg)
{
	pthread_detach(pthread_self());//makes thread detach itself
	struct argVals *argv = (struct argVals *) arg;
	int i, j;
	j = argv->integer;
	struct timespec ts;
	ts.tv_sec = 1;
	ts.tv_nsec = 0;
	for (i = j; i <= limit; i++)//print from j to limit every second
	{
		myPrintInt(i);
		myPrint("\n");
		i++;
		nanosleep(&ts, NULL);
	}
	pthread_exit(NULL);
}
//-----------------------------------------------------------------------
int main(int argc, char *argv[]) 
{
	pthread_t tid;
	int s;
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 500000000;
	struct argVals args1, args2;
	args1.integer = 0;
	args2.integer = 1;
	s = pthread_create(&tid, NULL, threadFunc, &args1);
	if (s != 0)
	{
		errno = s;
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	//separates execution times: integers are now printed in increasing order.  
	nanosleep(&ts, NULL);
	s = pthread_create(&tid, NULL, threadFunc, &args2);
	if (s != 0)
	{
		errno = s;
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	sleep((limit/2)+1);
	exit(EXIT_SUCCESS);
}
