/* A8-SharingDataOverAFIFO-Producer.c */
/************************************************************************
//Date due: 11/7/2021
//OS: ubuntu 20.04
//Made in: Text Editor
//Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
//Language: C
//Role: Producer
//Desc: shares data over a FIFO.  
Producer prompts data and sends data.  
Consumer receives data and outputs data.
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

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
//main code--------------------------------------------------------------
int main(int argc, char *argv[])
{
	//initialize variables
	int fd;
	char buf[1024];
	//makes fifo FIFO
	if (mkfifo("FIFO", S_IRWXU) == -1)
	{//-1 means an error occurred
		if (errno != EEXIST)
		{//if error is because FIFO already exists, ignore it and continue; otherwise, perror and exit
			perror("mkfifo producer");
			exit(EXIT_FAILURE);
		}
	}
	//open FIFO for write.  blocks until FIFO is also open for read
	fd = open("FIFO", O_WRONLY);
	if (fd == -1)
	{
		perror("open FIFO for write");
		exit(EXIT_FAILURE);
	}
	//producer prompts data
	myPrint("Please enter text at the producer: prompt\n");
	myPrint("producer: ");
	if (read(STDIN_FILENO, buf, 1024) == -1)
	{
		perror("parent prompt");
		exit(EXIT_FAILURE);
	}
	buf[strlen(buf)-1] = '\0';
	//producer writes data into FIFO
	if (write(fd, buf, strlen(buf)) == -1)
	{
		perror("FIFO send");
		exit(EXIT_FAILURE);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
