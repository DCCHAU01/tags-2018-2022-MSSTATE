/* myRand.c */
//Date due: 10/6/2021
//OS: ubuntu 20.04
//Made in: Text Editor that comes with Ubuntu
//Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
//Language: C
//Desc: generates file dataX.dat where X is a random number between 0 and 255.  Generates 60 values between 0 and 100 and stores it in dataX.dat.  Exits with exit status of X
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
//system call print functions--------------------------------------------
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
//gets length of a number. 10 would be length 2--------------------------
int intlen(const int val)
{
	int b = val;
	int d = 0;
	if (val == 0){
		return 0;
	}
	while (b > 0){
		b /= 10;
		d++;
	}
	return d;
}
//calculates random number with limiter imposed--------------------------
int calcRan(int limit)
{
	return (rand() % limit);
}
//assumes file is open and writes random numbers to file-----------------
void wVal(int fd)
{
	int i = 0, a , len, b;
	while (i < 60)
	{
		a = calcRan(100);
		write(fd, &a, sizeof(a));
    		i++;
	}
	return;
}
//the main code----------------------------------------------------------
int main(int argc, char *argv[]) 
{
	int fd;
	int flags = O_CREAT | O_WRONLY | O_APPEND;
	mode_t perms = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP |S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH;
	
	srand(time(NULL));//uses current time as seed for rand
//generating data file with random X-------------------------------------
	int X = calcRan(255);
	int len = intlen(X);
	char buf[len];
	sprintf(buf, "%d", X);
	char p3[] = ".dat";
	char datbuf[20] = "data";
	strncat(datbuf, buf,strlen(buf));
	strncat(datbuf, p3, strlen(p3));
	myPrint("Generating data file: ");
	myPrint(datbuf);
	myPrint("\n");
//opening file, fails if error is returned-------------------------------
	fd = open(datbuf, flags, perms);
	if (fd == -1)
	{
		perror("open for write");
		exit(EXIT_FAILURE);
	}
//writing numbers to file and cleaning up--------------------------------
	wVal(fd);
	close(fd);
	exit(X);
}
