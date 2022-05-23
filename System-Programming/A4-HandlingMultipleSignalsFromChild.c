/* A4-HandlingMultipleSignalsFromChild.c */
/************************************************************************
Date due: 9/29/2021
OS: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Desc: an application of fork that generates a parent and a child.  Child runs on a timer.  When the timer expires, a value is randomly generated and a signal is sent to parent.  Parent and child acts depending on value.  Parent will wait for child to terminate before terminating itself.  When signalling an interrupt, type Y to terminate all processes.  type n to continue processes.
************************************************************************/
//If you're OCD or oriented efficiently, you may wonder why I slapped a 
//bunch of libraries with no explanations added.
//...When I found a function that required a certain library, I simply
//slapped it into the "include" section.
/***********************************************************************/
//A little late, but I looked up how to prevent the crawler from linking 
//to this particular repository, so if you are here without my reference, 
//good luck.
/****"include" section**************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
/***********************************************************************/
#define BUF_SIZE 1024
/***********************************************************************/
//declaring global variables for global access
int ranNum = 0;    //random number
int X = 0;         //counter for signal SIGUSR1
int Y = 0;         //counter for signal SIGUSR2
pid_t pPid;        //parent process id - process id is not an int
pid_t cPid;        //child process id
/***********************************************************************/
//prints a string to STDOUT
int myPrint(const char *str)
{
 	int i = 0;
 	int numBytes;
 	if ((numBytes = write(STDOUT_FILENO, str, strlen(str))) > 0){ 
 		return 1;
	}
	if (numBytes == -1) {return 0;}
}
//convert int to string and call myPrint(str)
int myPrintInt(const int val)
{
	int b = val;
	int d = 0;
	int i;
	//catch value of 0
	if (val == 0){
		char str[1];
		str[0] = 0 + '0';
		return myPrint(str);
	}
	//catch value greater than 0
	while (b > 0){
		b /= 10;
		//count length of value, think strlen()
		//num: 100 :would have a length of 3
		d++;
	}
	//initialize a string buffer of length d
	char str[d];
	//append terminating character to the 
	str[d] = '\0';
	b = val;
	for (i = d-1; i >= 0; i--){
		//ASCII trick to convert from int to string
		//Some math to correctly move the correct digit into str
		str[i] = (b % 10) + '0';
		b = b / 10;	
	}
	return myPrint(str);
}
/***********************************************************************/
//executes when certain processes exit
void exitHandlerC(void) {
	myPrint("Child exiting\n"); 
	//sends signal SIGCHLD to parent process id
	kill(pPid, SIGCHLD); 
	myPrint("child exits\n");
}
void exitHandlerP(void) {
	myPrint("Parent exiting\n"); 
	//sends signal SIGTERM to child process id
	kill(cPid, SIGTERM);
	myPrint("parent exits\n");
}
/***********************************************************************/
//determines what action to take when the process receives a signal
void signalHandler(int sig)
{
 	char buf[BUF_SIZE];
 	char msg[BUF_SIZE] = "\nExit: Are you sure (Y/n)? ";
 	int RD;
 	int i;
 	int stat;
	//if I remember correctly, child is set to ignore SIGINT
	//=> only parent will process SIGINT
	//if signal is SIGINT, then prompt user with msg and continue
	if (sig == SIGINT){
		myPrint(msg);
		//reads 4 bytes from STDIN and stores it in buf
 		while ((RD=read(STDIN_FILENO, buf, 4)) >= 0 ){
			//catch error
 			if (RD == -1){
 				perror("Read from STDIN");
 				exit(EXIT_FAILURE);
 			}
			//catch single character.  
			//'\n' may take up buffer space, 
			//hence the loop
 			for (i = 0 ; i < 4 ; i++){
 				if (buf[i] == 'Y'){
 				kill(cPid, SIGTERM);
 				}
 			}  
 			for (i = 0 ; i < 4 ; i++){
 				if (buf[i] == 'n'){
 				return;} else {continue;};
 			}
 			myPrint(msg);
 		}
	}
	//when child exits, it sends SIGCHLD to parent
	if (sig == SIGCHLD){
		//the parent then waits until all children 
		//have finished cleaning up and leaving
		while (waitpid(cPid, NULL, WNOHANG) > 0){
			sleep(1);
			continue;
		}
			sleep(1);
			myPrint("No more children remain\n");
			exit(EXIT_SUCCESS);
	}
	//when child reads a value that meets certain
	//conditions, it sends a signal to the parent
	if (sig == SIGUSR1){
		X++;	//increment count of values < 25
		myPrint("The child has generated ");
		myPrintInt(X);
		myPrint(" values less than 25\n");
		return;
	}
	if (sig == SIGUSR2){
		Y++;	//increment count of values > 25
		myPrint("The child has generated ");
		myPrintInt(Y);
		myPrint(" values greater than 75\n");
		return;
	}
	//child generates a random number and signals parent
	//when the number meets certain conditions
	if (sig == SIGALRM){
		ranNum = rand() % 100;
		myPrint("The child has generated value: ");
		myPrintInt(ranNum);
		myPrint("\n");
		if (ranNum < 25){
			kill(pPid, SIGUSR1);
			return;
		}
		if (ranNum > 75){
			kill(pPid, SIGUSR2);
			return;
		}
		//exit when victory condition is met
		if ((ranNum > 48) && (ranNum < 51)){
			exit(EXIT_SUCCESS);
		}
		return;	//I suspect that this may cause an error, but I never ran into it.
	}
	//SIGTERM means "signalling termination"
	if (sig == SIGTERM){
		exit(EXIT_SUCCESS);
	}
}
/***********************************************************************/
int main(int argc, char *argv[])
{
	//initializing variables
	int i;			//loop variable
	int sec = 15;		//wait interval in seconds
	pid_t pid;			//holds process id
	
	struct sigaction sa;	//set of variables for signal actions
	struct itimerval tv;	//set of variables for timer values
/***********************************************************************/
	//setting values
	sa.sa_handler = signalHandler;	//sets the signal handler
	sigemptyset(&sa.sa_mask);		//empties the signal mask
	sa.sa_flags = 0;				//no flags
	//I know I looked up how the timer works; clearly, past me knew 
	//more than present me.  All I know now is that a number is generated every 15 seconds.
	tv.it_value.tv_sec = sec;		
	tv.it_value.tv_usec = 0;
	tv.it_interval.tv_sec = 0;
	tv.it_interval.tv_usec = 0;
/***********************************************************************/
	pPid = getpid();		//parent stores its own process id
	pid = fork();		//fork processes.  Each process will store 
					//their own process id into their own copy of pid
					//parent stores child process id into pid
					//child stores 0 into pid
	//exit if fork failed
	if (pid == -1){
		perror("fork");
		exit(EXIT_FAILURE);
	}
/***********execute if process is child*********************************/
	if (pid == 0){
		//at exit, do exitHandlerC()
		atexit(exitHandlerC);
		//child adds SIGINT to the mask
		sigaddset(&sa.sa_mask, SIGINT);
		//I forgot what this does, let me look it up.
		//child will now block signals in the mask
		sigprocmask(SIG_BLOCK, &sa.sa_mask, NULL);
/***********************************************************************/
	//when a certain signal is received, child will do signalHandler
 		if (sigaction(SIGALRM, &sa, NULL) == -1){
 			perror("sigaction child SIGALRM");
 			exit(EXIT_FAILURE);
 		}
 		if (sigaction(SIGTERM, &sa, NULL) == -1){
 			perror("sigaction child SIGTERM");
 			exit(EXIT_FAILURE);
 		}
/***********************************************************************/
 		myPrint("Generating Random Numbers . . . \n");
 		while (1){
			//sets timer to 15 seconds
			//every time the timer expires, SIGALRM is sent to child
			//look at signalHandler for expected behavior
 			if (setitimer(ITIMER_REAL, &tv, NULL) < 0){
 				perror("setitimer");
 				exit(EXIT_FAILURE);
 			}
 			sleep(sec);
 		}
	} 
/***********execute when process is parent******************************/
	else {
		//store child process id
		cPid = pid;
		//at exit, do exitHandlerP()
		atexit(exitHandlerP);
/***********************************************************************/
		//when a certain signal is received, do signalHandler
		if (sigaction(SIGCHLD, &sa, NULL) == -1){
			perror("sigaction parent SIGCHLD");
			exit(EXIT_FAILURE);
		}
		if (sigaction(SIGINT, &sa, NULL) == -1){
			perror("sigaction parent SIGINT");
			exit(EXIT_FAILURE);
		}
		if (sigaction(SIGUSR1, &sa, NULL) == -1){
			perror("sigaction parent SIGUSR1");
			exit(EXIT_FAILURE);
		}
		if (sigaction(SIGUSR2, &sa, NULL) == -1){
			perror("sigaction parent SIGUSR2");
			exit(EXIT_FAILURE);
		}
		//parent waits for signals
		while(1){pause();}
/***********************************************************************/
	}
}
