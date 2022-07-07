/* A1-WritingTheDataFile.c */
/*
Notes: If you are reading this after you read A3, you must be 
	 wondering why A1 is here.  Well, er, uhmm, it is because
	 I dug through my files and found it in the Project2 folder
*/
/************************************************************************
Date due: 8/29/2021
OS: ubuntu 20.04
Made in: Text Editor
Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
Language: C
Desc: generates a set of random numbers from three ranges and stores
them in a binary file by using system calls
************************************************************************/
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define BUF_SIZE 4

union dtob {
  	int value;
  	char bytes[4];
};

int outBytes(const char *out, int cnt, int low, int mid, int high) {
	int outputFd, openFlags, i;
	mode_t filePerms;
	int range;
	double offst;
  	int value;
	union dtob outBytes;

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

	outputFd = open(out, openFlags, filePerms);

	if (outputFd == -1) {
		perror("open for write");
      	return -1;
	}

	for (i = 0; i < cnt; i++) {
	      range = rand() % 100;//generate num b/w 0&100
     	 	offst = (rand() % 1000 - 500)/1000.0;//offset by 0.5

     	 	if (range < 15) { value = (int) ((low + offst) * 100);}
       else if (range > 85) { value = (int) ((high + offst) * 100);}
                       else { value = (int) ((mid + offst) * 100); }

      	outBytes.value = value;
      	printf("%d\n", value);
      	if (write(outputFd, outBytes.bytes, 4) != 4) {
      		perror("write");
      		return -1;
    		}
	}
 
  	close(outputFd);
	return 0;
}

char *dupString(const char *val) {
  	int len = strlen(val);
  	char *outStr = (char *) calloc(len + 1, sizeof(char));
  	if (outStr == NULL) {
	      perror("calloc");
      	exit(EXIT_FAILURE);
    	}
  	strcpy(outStr, val);
  	return outStr;
}

// if we call createFile moreData.dat 50 8 11 15
// then argc = 6
// argv[0] = "createFile"
// argv[1] = "moreData.dat"
// argv[2] = "50"
// argv[3] = "8"
// argv[4] = "11"
// argv[5] = "15"
// to make argv[2] into 50 .. atoi(argv[2]) ... 
int main(int argc, char *argv[]) 
{
  	char *outFile = NULL;
  	int count =  60;
  	int low = 5;
  	int mid = 10;
  	int high = 15;
  
  	srand(time(NULL));

  	if (argc != 1 && argc != 3 && argc != 6) {
	      perror("malformed call to createFile");
	      exit(EXIT_FAILURE);
	}

  	if (argc == 1) {
      	outFile = dupString("dataFlow.dat");
    	}
  	if (argc > 2) {
	      outFile = dupString(argv[1]);
      	count = atoi(argv[2]);
    	}
  	if (argc > 5) {
      	low = atoi(argv[3]);
      	mid = atoi(argv[4]);
      	high = atoi(argv[5]);
    	}

  	if (outBytes(outFile, count, low, mid, high) == -1) {
	      perror("failed to write file");
	      exit(EXIT_FAILURE);
    	}
  	exit(EXIT_SUCCESS);
}
