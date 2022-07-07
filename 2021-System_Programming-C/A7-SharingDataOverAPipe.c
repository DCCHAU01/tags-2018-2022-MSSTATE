/* A7-SharingDataOverAPipe.c */
/************************************************************************
Thoughts: While learning about makefiles, I thought back to all the times
	    I dumped myPrint and every other self-made reusable function 
	    into each and every file.  Ah....
	    I rethought my conclusion from A6 and arrived at another 
	    conclusion.  No.  Why?  These comments are also for myself.
Date due: 11/7/2021
OS: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Desc: shares data over a pipe.  
Parent prompts data and sends data.  
Child receives data and outputs data.
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>
#include <string.h>
#include <errno.h>

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
//signal handler---------------------------------------------------------
void signalHandler(int sig){
	if (sig == SIGCHLD)
	{
		pid_t p;
		while ((p = waitpid(-1, NULL, WNOHANG)))
		{
			if (errno == ECHILD)
			{
				exit(EXIT_SUCCESS);
			}
			if (p == -1)
			{
				perror("waitpid");
				exit(EXIT_FAILURE);
			}
		}
	}
}
//main code--------------------------------------------------------------
int main(int argc, void *argv[])
{
	//initialize variables
	char buf[1024], buf2[1024];
	int fd[2];
	pid_t pid;
	//make and define sigaction sa
	struct sigaction sa;
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	//assign signal handler to parent for child's exit
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
	{
		perror("sigaction SIGCHLD");
		exit(EXIT_FAILURE);
	}
	//open pipe
	if (pipe(fd) == -1)
	{
		perror("open the pipe");
		exit(EXIT_FAILURE);
	}
	//make child
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
//child's behavior-------------------------------------------------------
	if (pid == 0)
	{
		close(fd[1]);//close write side
		int s;
		s = read(fd[0], buf2, 1024);
		if (s == -1)
		{
		 	perror("read from pipe");
			exit(EXIT_FAILURE);
		}
		myPrint("child: ");
		myPrint(buf2);
		close(fd[0]);//close read side
		exit(EXIT_SUCCESS);
	}
//parent's behavior------------------------------------------------------
	else
	{
		close(fd[0]);//close read side
		int s;
		myPrint("Please enter text at the parent: prompt\n");
		myPrint("parent: ");
		s = read(STDIN_FILENO, buf, 1024);
		if (s == -1)
		{
			perror("read from STDIN_FILENO");
			exit(EXIT_FAILURE);
		}
		if (write(fd[1], buf, strlen(buf)) == -1)
		{
			perror("write to pipe");
			exit(EXIT_FAILURE);
		}		
		close(fd[1]);//close write side
		while(1){pause();}
	}
}
