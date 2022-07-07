/* P3-ParentAndChild-main.c */
/************************************************************************
Date: 10/10/2021
os: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Desc: a flow monitoring program that consists of a parent process and a 
child process.  
The parent receives and outputs the input, reaps children, handles 
interruptions, and prints warnings when child tells it to.  
Child opens flowData.dat, ignores interruptions, has a REAL interval 
timer.  When parent dies, child kills itself.  When certain values are 
read from flowData.dat, it tells parent to print a warning.  Exits on EOF.
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

#define BUF_SIZE 2014
//initializing global values---------------------------------------------
time_t X, Y;
int fd;
pid_t pid;
pid_t pPid;
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
//exit handlers----------------------------------------------------------
void exitHandlerC(void) {
	myPrint("exitHandlerC executing.\n");
}
void exitHandlerP(void) {//if Parent exits first, tell children to die
	myPrint("exitHandlerP executing.\n");
	kill(0, SIGTERM);
}
//signal handlers--------------------------------------------------------
void signalHandlerP(int sig)
{
 	char buf[BUF_SIZE];
 	char msg[BUF_SIZE] = "\nExit: Are you sure (Y/n)? ";
 	int RD;
 	pid_t p;
 	int i = 0, status;
 	if (sig == SIGCHLD){//polls for zombie children
		while ((p = waitpid(-1, &status, WNOHANG)) > 0)
		{
			continue;
		}
		if (p == -1)//parent exits when there are no more children
		{
			if (errno == ECHILD){
				myPrint("Warning!  Pressure control monitoring has been terminated, exiting the system.\n");
				exit(EXIT_SUCCESS);
			}
			perror("wait");
			exit(EXIT_FAILURE);
		}
 	}
 	if (sig == SIGINT){
 		myPrint(msg);
 		while ((RD=read(STDIN_FILENO, buf, 4)) >= 0 ){
 			if (RD == -1){
 				perror("Read from STDIN");
 				exit(EXIT_FAILURE);
 			}
 			for (i = 0 ; i < 4 ; i++){
 				if (buf[i] == 'Y'){
 				printf("Y");
 				exit(EXIT_SUCCESS);}
 			}  
 			for (i = 0 ; i < 4 ; i++){
 				if (buf[i] == 'n'){
 				printf("n");
 				return;} else {continue;};
 			}
 			myPrint(msg);
 		}
 	}
 //handlers for SIGUSR 1 & 2---------------------------------------------
 	if (sig == SIGUSR1){
 		myPrint("Warning! Line pressure is dangerously low.  It has been ");
 		Y = time(NULL);
 		myPrintInt(Y-X);
 		myPrint(" seconds since the previous warning.\n");
 		X = time(NULL);
 		return;
 	}
 	if (sig == SIGUSR2){
		myPrint("Warning! Line pressure is dangerously high.  It has been ");
		Y = time(NULL);
		myPrintInt(Y-X);
 		myPrint(" seconds since the previous warning.\n");
 		X = time(NULL);
 		return;
 	}
}
void signalHandlerC(int sig)
{
	if (sig == SIGTERM){close(fd); exit(EXIT_SUCCESS);}//death to child
	if (sig == SIGALRM){//read when timer rings
		int c;
		while (read(fd, &c, sizeof(c)) != 0)
		{
			if ((c>=450) && (c<=550))
			{kill(pPid, SIGUSR1); return;}
			if ((c>=1450) && (c<=1550))
			{kill(pPid, SIGUSR2); return;}
			return;
		}
		close(fd);
		exit(EXIT_SUCCESS);
	}
}
//the main code----------------------------------------------------------
int main(int argc, char *argv[])
{
	int sec = 5;//timer rings every sec seconds
	//intializing structs
	struct sigaction saP, saC;
	struct itimerval tv;
	//setting structs
	saP.sa_handler = signalHandlerP;
	sigemptyset(&saP.sa_mask);
	saP.sa_flags = 0;
	saC.sa_handler = signalHandlerC;
	sigemptyset(&saC.sa_mask);
	saC.sa_flags = 0;
	
	tv.it_value.tv_sec = sec;
	tv.it_value.tv_usec = 0;
	tv.it_interval.tv_sec = sec;
	tv.it_interval.tv_usec = 0;
	//acquiring parent pid
	pPid = getpid();
	pid = pPid;
	
	if (pid == -1){perror("fork error");exit(EXIT_FAILURE);}
	//parent forks a child and handles accordingly
	if (pid > 0){
		pid = fork();
		if (pid == 0){//child's behavior
			atexit(exitHandlerC);
			int flags = O_RDONLY;
			mode_t perms = S_IRUSR;
			fd = open("flowData.dat", flags, perms);
			if (fd == -1)
			{
				perror("open file");
				exit(EXIT_FAILURE);
			}
			sigaddset(&saC.sa_mask, SIGINT);//adds SIGINT to mask
			sigprocmask(SIG_BLOCK, &saC.sa_mask, NULL);//tells program to BLOCK signals in sig set
			if (sigaction(SIGTERM, &saC, NULL) < 0){
				perror("Sigaction: SIGTERM");
				exit(EXIT_FAILURE);
			}
			if (sigaction(SIGALRM, &saC, NULL) < 0){
				perror("Sigaction: SIGALRM");
				exit(EXIT_FAILURE);
			}
			if (setitimer(ITIMER_REAL, &tv, NULL) < 0){
 				perror("setitimer");
 				exit(EXIT_FAILURE);
 			}
 			while(1) {pause();}
		}
		else
		{//parent's behavior
			atexit(exitHandlerP);
			X = time(NULL);
			if (sigaction(SIGCHLD, &saP, NULL) < 0){
				perror("Sigaction: SIGCHLD");
				exit(EXIT_FAILURE);
			}
			if (sigaction(SIGINT, &saP, NULL) < 0){
				perror("Sigaction: SIGINT");
				exit(EXIT_FAILURE);
			}
			if (sigaction(SIGUSR1, &saP, NULL) < 0){
				perror("Sigaction: SIGUSR1");
				exit(EXIT_FAILURE);
			}
			if (sigaction(SIGUSR2, &saP, NULL) < 0){
				perror("Sigaction: SIGUSR2");
				exit(EXIT_FAILURE);
			}
			char buf[1024]; 
			int bR, bW;
			while(1) 
			{
				bR = read(STDIN_FILENO, buf, 1024);
				bW = write(STDOUT_FILENO, buf, bR);
			}
		}
	}
}
