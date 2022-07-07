// P4-ParallelizedRadixSort-Producer.c
/************************************************************************
Date: 11/14/2021
os: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Role: Producer
Desc: creates data file containing a number of integers with differing lengths.
The Project Assignment:
Program 1 should create a data file containing an equal number of integers with differing lengths; Program 2 should read that file, sort them, and write the sorted integers into a new file.

In broad strokes, Program 1 should do the following:

1 Prompt the user for how many integers to generate
2 Prompt the user for the smallest integer to generate
3 Prompt the user for the largest integer to generate
4 Compute the number of digits in the smallest integer
5 Compute the number of digits in the largest integer
6 Create a file of integers called "data.dat" using the above parameters.
This program should take advantage of threads to accomplish this task. You should have two types of threads: the main thread and an output thread.

The main thread should do the following:
    Prompt the user for the details (smallest, largest, and number to create)
    Compute the number of digits in the smallest and largest integers
    Compute the difference between the smallest number of digits and largest number of digits. This is how many buckets you need to create
    Divide the total number of digits being created equally amongst each bucket. This is the bucket size.
    Open data.dat for write
    Loop from smallest digit count to largest digit count
         Spawn an output process to generate the appropriate number of digits with that digit count.

    Loop from smallest digit count to largest digit count
         Join the thread
    Close data.dat

The output threads must perform the following:
    Loop forever
         Set the chunks output to 0
         Loop while chucks output < chunk size (choose one larger than 1)
               Generate a random number in the appropriate range (determined by the digit count and the largest and smallest values)
               Output the value into the file
               Increment the total values output, if it is equal to the bucket size then exit the thread
          Sleep the thread for 500 nanoseconds
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/time.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int pred = 0;
int fd;
//holds bucket data----
struct vals 
{
	int numDigits;
	int smallest;
	int largest;
	int smallCount;
	int largeCount;
	int numBuckets;
	int bucketSize;
}data;
//handles error numbers--------------------
void hndlError(int error, const char *str)
{
  if (error == 0) return;
  errno = error;
  perror(str);
  exit(EXIT_FAILURE);
}
//unnecessary system print function-----------------
void myPrint(const char *str)
{
  if (write(STDOUT_FILENO, str, strlen(str)) == -1)
    {
      perror("write");
      exit(EXIT_FAILURE);
    }
}
//compute number of digits in a number
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
//my version of power function-----
int myPow(int a, int b)
{
	int c = a;
	for (int i = 1; i < b; i++)
	{
		c = c * a;
	}
	if (b == 0){return 0;};
	return c;
}
//the process thread
//takes number of digits from a, calculates rand, and outputs into file
void *threadFuncB(void *a){
	pthread_mutex_lock(&mtx);
	while (pred == 0){
		pthread_cond_wait(&cond, &mtx);
	}
	pred = 0;
	int d = *(int*)a;
	//debugging commented out
	//printf("thread id: %ld\n", pthread_self());
	//printf("B: %d\n", d);
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 500;
	int chunksOutput = 0;
	int chunkSize;//size of bucket
	int n = d;
	//printf("n: %d\n", n);
	int b = myPow(10,n)-1;
	//printf("b: %d\n", b);
	int c = myPow(10,n-1);
	//printf("c: %d\n", c);
	int max = data.largest;
	//printf("max: %d\n", max);
	int min = data.smallest;
	//printf("min: %d\n", min);
	if (max < b){b = max;}
	if (min > c){c = min;}
	int e = (b-c+1);
	//printf("e: %d\n", e);
	int output;
	srand(time(NULL));
	while(1){
		while (chunksOutput < data.bucketSize){
			output = (rand() % e)+(c);
			//printf("output: %d\n", output);
			write(fd, &output, sizeof(int));
			chunksOutput++;
		}
		if (chunksOutput == data.bucketSize){
			pred = 1;
			pthread_mutex_unlock(&mtx);
			pthread_cond_broadcast(&cond);
			pthread_exit(NULL);
		}
		nanosleep(&ts, NULL);
	}
}
//the main thread: gets input and computes bucket's measurements
void *threadFuncA(){
	char buf[64];
	int a, b, c;
	myPrint("How many integers do you want to create? ");
	read(STDIN_FILENO, buf, 64);
	if (buf == NULL){exit(EXIT_FAILURE);}
	a = atoi(buf);
	if (a == 0){myPrint("invalid input\n"); exit(EXIT_FAILURE);}
	myPrint("What is the smallest integer? ");
	read(STDIN_FILENO, buf, 64);
	if (buf[0] == '\n'){myPrint("invalid input\n");exit(EXIT_FAILURE);}
	b = atoi(buf);
	data.smallest = b;
	myPrint("What is the largest integer? ");
	read(STDIN_FILENO, buf, 64);
	if (buf[0] == '\n'){myPrint("invalid input\n");exit(EXIT_FAILURE);}
	c = atoi(buf);
	data.largest = c;
	
	if (c < b){myPrint("large number can't be smaller than small number\n"); exit(EXIT_FAILURE);}
	data.smallCount = numDigits(b);
	data.largeCount = numDigits(c);
	data.numBuckets = (data.largeCount - data.smallCount)+1;
	data.bucketSize = a/data.numBuckets;
	int remainder = a%data.numBuckets;
	if (remainder != 0){
		myPrint("numbers can not be split equally among buckets because (numberofintegers / numBuckets) is not a whole number.  Amount of numbers generated will be decreased to ");
		printf("%d\n", a-remainder);
	}
	//printf("numBuckets: %d\n", data.numBuckets);
	//printf("%d, %d, %d\n", a, b, c);
	int count = atoi(buf);
	int flags = O_CREAT | O_WRONLY | O_RDONLY | O_TRUNC;
	mode_t perms = S_IRUSR | S_IWUSR;
	fd = open("data.dat", flags, perms);
	if (fd == -1)
	{
		perror("open for write");
		exit(EXIT_FAILURE);
	}
	//printf("fd: %d\n", fd);
	//pred = 1;
	//pthread_cond_broadcast(&cond);
	pthread_t tid[data.numBuckets];
	int s;
	int offset;
	int cnD[data.numBuckets];
	//s = pthread_create(&tid[0], NULL, &threadFuncB, &fd);
	//hndlError(s, "create thread main fail\n");
	//pthread_join(tid[0], NULL);
	pred = 1;
	pthread_cond_broadcast(&cond);
	for (int i = data.smallCount; i <= data.largeCount; i++){
		//printf("i: %d\n", i);
		offset = i - data.smallCount;
		//printf("tid: %d\n", offset);
		cnD[offset] = i;
		//printf("cnD[%d] = %d\n", offset, cnD[offset]);
		s = pthread_create(&tid[offset], NULL, &threadFuncB, &cnD[offset]);
		//printf("thread %d: %ld\n", offset, tid[offset]);
		//sleep(1);
		hndlError(s, "create output thread fail\n");
	}
	for (int j = data.smallCount; j <= data.largeCount; j++){
		offset = j - data.smallCount;
		pthread_join(tid[offset], NULL);
	}
	close(fd);
	pthread_exit(NULL);
}
//the main function---------------------------------------------
//creates main thread
int main(int argc, char *argv[])
{
	pthread_t mtid;
	int s;
	s = pthread_create(&mtid, NULL, &threadFuncA, NULL);
	hndlError(s, "create main thread fail\n");
	pthread_join(mtid, NULL);
	myPrint("finished generating numbers and writing to data.dat.  Please wait a moment to see the unsorted data\n");
	sleep(2);
	//prints contents of file
	//can be used to compare with P4B's output file
	int t;
	int i = 0;
	int fd = open("data.dat", O_RDONLY, S_IRUSR | S_IWUSR);
	while (read(fd, &t, sizeof(t)) != 0){
		printf("%d: %d\n", i, t);
		i++;
	}
	//printf("i: %d\n", i);
	exit(EXIT_SUCCESS);
}
