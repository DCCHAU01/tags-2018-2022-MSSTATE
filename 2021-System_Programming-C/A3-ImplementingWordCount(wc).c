/* A3-ImplementingWordCount(wc).c */
/***********************************************************************
Date: 9/15/2021
os: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Desc: a I/O system call implementation of wc
wc is used to count the number of words, lines, and characters in a file
By default, wc outputs the number of words in its input and reads from 
standard input until the end-of-file. The wc command supports the following command line arguments:
-c -- display the number of characters
-l -- display the number of new-lines
-w -- displays the number of words
<file> -- read from the specified file instead of standard input. 
Issue: clean up EWC
***********************************************************************/
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <linux/limits.h>
#include <string.h>
#include <ctype.h>

#define BUF_SIZE 1024

int bytesWritten;

//My implemented version of wc
void EWC(int fd, char buf[], int BUFF_SIZE, int bytesRead, int totalbytesRead, int linesRead, int wordsRead, char mode[])
{
	int i;
	while ((bytesRead = read(fd, buf, BUFF_SIZE)) > 0)
		{
			buf[bytesRead] = '\0';
			//count characters, newlines, and words
			for (i = 0; i<bytesRead; i++)
			{       //count words
				if ((isspace(buf[i])) || (buf[i] == '\0'))
				{       
					if ((i>0) && (!isspace(buf[i-1])))
					{
						wordsRead++;
					}
				}
				//count bytes 
				if (!isspace(buf[i])) //I do not know if this is the right way...
				{
					totalbytesRead++;
				}
				//count number of lines
				if (buf[i] == '\n')
				{ 
					linesRead++;
				}
			}
			if (bytesRead == -1) //Will this ever run?
			{
				perror("read from standard input");
				exit(EXIT_FAILURE);
			}
		}
		//handles the displaying of num chars, newlines, & words
	        //word count mode
		if ((strcmp(mode, "default") == 0) || (strcmp(mode, "-w") == 0))
		{
			printf("total words read: %d\n", wordsRead);
		}
	        //line count mode
		if (strcmp(mode, "-l") == 0)
		{
			printf("total lines read: %d\n", linesRead);
		}
	        //character count mode
		if (strcmp(mode, "-c") == 0)
		{
			printf("total chars read: %d\n", totalbytesRead);
		}
}


//Assumes txt file is in the same directory as this program when compiling and running this program.
int main(int argc, char *argv[])
{	//initializing variables
	int fd;
	int flags = O_RDONLY;
	int perms = S_IRUSR | S_IWUSR;
	char buf[BUF_SIZE];
	int bytesRead;
	int totalbytesRead = 0;
	int linesRead = 0;
	int wordsRead = 0;
//the help screen----------------------------------------------------------
	printf("Valid arguments are: \n -c \n -l \n -w \n <file> \n");
	printf("Anything else is invalid!\n");
	printf("Ex call: ./executablefilename -c or ./executablefilename test.txt or ./executablefilename -w test.txt\n\n");
//------the default call of wc---------------------------------------------
	//counts words from STDIN
	if (argc == 1)//with no arguments
	{
		printf("This is default mode!\n");
		EWC(STDIN_FILENO, buf, BUF_SIZE, bytesRead, totalbytesRead, linesRead, wordsRead, "default");
		
	}
//-------handles command line arguments of size 2--------------------------
//example call: ./executablefilename test.txt
//example call 2: ./executablefilename -c
	if (argc == 2) {
		if (strcmp(argv[1], "-c") == 0) {
			printf("This is chars mode!\n");
			EWC(STDIN_FILENO, buf, BUF_SIZE, bytesRead, totalbytesRead, linesRead, wordsRead, "-c");
			exit(EXIT_SUCCESS);
		} else if (strcmp(argv[1], "-l") == 0) {
			printf("This is newlines mode!\n");
			EWC(STDIN_FILENO, buf, BUF_SIZE, bytesRead, totalbytesRead, linesRead, wordsRead, "-l");
			exit(EXIT_SUCCESS);
		} else if (strcmp(argv[1], "-w") == 0) {
			printf("This is words mode!\n");
			EWC(STDIN_FILENO, buf, BUF_SIZE, bytesRead, totalbytesRead, linesRead, wordsRead, "-w");
			exit(EXIT_SUCCESS);
		} else if ((argv[1] != NULL)) {
			//opens file and counts words in it
			fd = open(argv[1], flags, perms);
			if (fd == -1) //if file doesn't exist {
				perror("error\n");
				exit(EXIT_FAILURE);
			} else {//if file exists
				printf("file opened for read!\n");
			}
			printf("This is default mode!\n");
			EWC(fd, buf, BUF_SIZE, bytesRead, totalbytesRead, linesRead, wordsRead, "default");
		} //Should have added an "else {"
	}
	
//-------handles command line arguments of size 3--------------------------
//example call: ./E3 -c test.txt
//example call 2: ./E3 -l test.txt
//a 3rd argument allows for displaying the chars, newlines, or words of a file
	if (argc == 3) {
		fd = open(argv[2], flags, perms);
		if (fd == -1) {
			perror("error\n");
			exit(EXIT_FAILURE);
		} else {
			printf("file opened for read!\n");
		}
		if (strcmp(argv[1], "-c") == 0) {
			printf("This is chars mode!\n");
			EWC(fd, buf, BUF_SIZE, bytesRead, totalbytesRead, linesRead, wordsRead, "-c");
		} else if (strcmp(argv[1], "-l") == 0) {
			printf("This is newlines mode!\n");
			EWC(fd, buf, BUF_SIZE, bytesRead, totalbytesRead, linesRead, wordsRead, "-l");
		} else if (strcmp(argv[1], "-w") == 0) {
			printf("This is words mode!\n");
			EWC(fd, buf, BUF_SIZE, bytesRead, totalbytesRead, linesRead, wordsRead, "-w");
		}
	}	
	
//-------the end------------------------------------------------------------
	close(fd);
	exit(EXIT_SUCCESS);
	
}
