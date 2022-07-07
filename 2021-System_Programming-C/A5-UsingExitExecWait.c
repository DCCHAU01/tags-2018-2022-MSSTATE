/* A5-UsingExitExecWait.c */
/************************************************************************
Notes: If you somehow found this repository for a school project, 
	 do not just .COPY.    !UNDERSTAND!
Date due: 10/6/2021
OS: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Desc: uses fork to execute myRand program.  Retrieves X from exit 
status using wait, WIFEXITED, and WEXITSTATUS.  Opens dataX.dat where X 
is from the retrieved X exit status.  Reads 60 values in dataX.dat, 
computes their average, and displays both.  Ends program with unlinking 
the dataX.dat file
************************************************************************/
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
//the main code----------------------------------------------------------
int main(int argc, char *argv[]) 
{
	pid_t pid;
	pid = fork();
//pid determines whether it is child or parent and acts as programmed----
	if (pid == -1){//if fork fails
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0){//child executes myRand
		char *av[] = {NULL};
		char *ep[] = {NULL};
		execve("./myRand", av, ep);
	} else {//parent
		char buf[1024] = { 0 };
		int status, i=0, b, fd, flags, avg=0;
		while (wait(&status) != -1){//waits for child to terminate
			if (WIFEXITED(status))
			{//gets exit status X from child
				sprintf(buf, "%d", WEXITSTATUS(status));
			}
		}//piecing together file name
		char p3[] = ".dat";
		char datbuf[20] = "data";
		strncat(datbuf, buf,strlen(buf));
		strncat(datbuf, p3, strlen(p3));
		
		flags =  O_RDONLY;
		mode_t perms = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP |S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH;
//opening file for reading and operating---------------------------------
		fd = open(datbuf, flags, perms);
		if (fd == -1)//if open fails
		{
			perror("open for write");
			exit(EXIT_FAILURE);
		}
		while (i < 60)//printing read values
		{
			read(fd, &b, sizeof(b));
			myPrintInt(b);
			myPrint("\n");
			avg += b;//accumulating average
			i++;
		}
		avg /= 60;//compute average
		myPrint("Average: ");
		myPrintInt(avg);
		myPrint("\n");
//cleaning up------------------------------------------------------------
		close(fd);
		if (unlink(datbuf) == -1)//if unlink fails
		{
			perror("unlink");
			exit(EXIT_FAILURE);
		} else {//if unlink success
			myPrint(datbuf);
			myPrint(" unlinked\n");
		}
		exit(EXIT_SUCCESS);
	}
//Afterword: at this point in time, I'm still adding comments to my 
//university assignments then uploading to github.  
//Imagine my surprise when I found out that past me already added comments.
}
