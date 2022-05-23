/* P2-ReportInvalidFlow-main.c */
/************************************************************************
Notes: Initially, I was confused about the different ways to open a file.
	 That was until I got a big, fat 80/100 along with the comment:
	 "This is a systems programming class.. don't use FILE*, fopen, 
	 fread, fwrite, fscanf, feof, etc.  It defeats the purpose of this 
	 class."
	 So here is the corrected version!
Date due: 9/12/2021
OS: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Desc: the legit version of P2, without using FILE*
reads from dataFlow.dat
prints low, medium, high messages based on the value read
prints alert if value is not medium
prints number of entries since last alert occurred
************************************************************************/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <linux/limits.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[])
{//------------assuming this program is run in terminal----------------
//		it gets current directory and prints it
	char cwd[PATH_MAX];                       
	char *current = getcwd(cwd, sizeof(cwd));
	printf("%s \n", cwd);
//-------assuming dataFlow.dat is in same directory as P2.c------------
//       it opens it to read, if open fails then error
	int fd;
	int flags =  O_RDONLY;
  	mode_t perms = S_IRUSR | S_IRGRP | S_IROTH;

	char filename[] = "dataFlow.dat";
	fd = open(filename, flags, perms);
	if (fd == -1) 
	{
		perror("open file");
		exit(EXIT_FAILURE);
	} else 
	{
		printf("%s opened\n", filename);
	}
//-------------initializing variables----------------------------------
	int a;//tracks state of read
	int c;//will store individual entries
	int count = 0;//counts number of entries
	int alertCount = 0;//counts number of entries since last alert occur
	int firstAlert = 0;//alert won't begin until first alert
//-------------reads entries and makes distinctions by-----------------
//             printing messages separating individual entries
//             by low, medium, and high pressure
//             any entry that is not medium prints an alert
	while ((a=read(fd, &c, sizeof(c))) != 0)
	{
		if (a == -1){perror("read error"); exit(EXIT_FAILURE);}
		count++;//track current number of entries
		alertCount++;//track count since last alert occur
		if ((c <= 950) || (c >= 1050)) {//prints alert
			if (firstAlert != 0){
				printf("Number of data entries read before since the last Alert: %d\n" , alertCount);
				alertCount = 0;
			} else {
				printf("This is the first Alert!\n");
				alertCount = 0;
				firstAlert = 1;
			}
			printf("Alert: Value read isn't between 950 and 1050: ");
			printf("%d\n", c);
		} else if ((c >= 950) || (c <= 1050)) {//prints medium
			printf("Pressure in the lines is medium: ");
			printf("%d\n", c);
		}
		if ((c >= 450) && (c <= 550)){//prints low
			printf("Pressure in the lines is low: ");
			printf("%d\n", c);
		}
		if ((c >= 1450) && (c <= 1550)){//prints high
			printf("Pressure in the lines is high: ");
			printf("%d\n", c);
		}
	}
	printf("Number of entries: %d\n", count);
//------cleaning up---------------------------------------------------------
	close(fd);
	return 0;
}
