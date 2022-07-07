/* A9-SharedMemoryAndSemaphores-Worker.c */
/************************************************************************
//Date: 11/23/2021
//os: ubuntu 20.04
//Made in: Text Editor
//Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
//Language: C
//Role: Worker
//Desc: uses shared memory and semaphores to communicate a sequence of 
//request and responses between two programs.
************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
int bsUseSemUndo = 0;
int bsRetryOnEintr = 1;
union semun 
{
	int val;   		  /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};
struct request
{
	int msg[3];
};

int reserveSem(int semId, int semNum)

{
  struct sembuf sops;

  sops.sem_num = semNum;
  sops.sem_op = -1;
  sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

  while (semop(semId, &sops, 1) == -1)
    {
      if (errno != EINTR || !bsRetryOnEintr)
	return -1;
    }

  return 0;
}

int releaseSem(int semId, int semNum)
{
  struct sembuf sops;

  sops.sem_num = semNum;
  sops.sem_op = 1;
  sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

  return semop(semId, &sops, 1);
}
int myPow(int a, int b)
{
	int c = a;
	for (int i = 1; i < b; i++)
	{
		c = c * a;
	}
	printf("myPow: %d\n", c);
	if (b == 1){return a;}
	if (b == 0){return 1;}
	return c;
}
int findMax(int *arr, int size){
	int max, temp;
	max = arr[0];
	for (int i = 1; i < size; i++){
		temp = arr[i];
		if (temp > max){
			max = temp;
		}
	}
	printf("findMax: %d\n", max);
	return max;
}
int findMin(int *arr, int size){
	int min, temp;
	min = arr[0];
	for (int i = 1; i < size; i++){
		temp = arr[i];
		if (temp < min){
			min = temp;
		}
	}
	printf("findMin: %d\n", min);
	return min;
}
int mySum(int *arr, int size){
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		j += arr[i];
	}
	printf("mySum: %d\n", j);
	return j;
}
int myProduct(int *arr, int size){
	int j = 0;
	for (int i = 0; i < size; i++)
	{
		j *= arr[i];
	}
	printf("myProduct: %d\n", j);
	return j;
}
int myDiff(int *arr, int size){
	int A = findMax(arr, size);
	int B = findMin(arr, size);
	printf("myDiff: %d\n", A-B);
	return A-B;
	
}
int myDiffLS(int *arr, int size){
	int A = myPow(findMax(arr, size), 3);
	int B = myPow(findMin(arr, size), 2);
	printf("myDiffLS: %d\n", A-B);
	return A-B;	
}
int myDiffSL(int *arr, int size){
	int A = myPow(findMin(arr,size), 3);
	int B = myPow(findMax(arr,size), 2);
	printf("myDiffSL: %d\n", A-B);
	return A-B;
}
void myprintArray(int *arr, int len){
	for (int i = 0; i < len; i++){
		printf("%d: %d\n", i, arr[i]);
	}
	return;
}
int main(int argc, char *argv[])
{
	struct request *smap;
	int semid, shmid;
	union semun dummy;
	
	key_t semK, shmK;
	
	semK = ftok(".", IPC_PRIVATE);	//generate keys
	shmK = ftok(".", IPC_PRIVATE);
	
	semid = semget(semK, 0, 0);
	shmid = shmget(shmK, 0, 0);
	
	smap = shmat(shmid, NULL, 0);	//attach shared memory
	if (smap == (void *) -1)
	{
		perror("shmat");
		exit(EXIT_FAILURE);
	}
	
	reserveSem(semid, 1);
	//wait for request from director
	//read action value and do stuff
	//stuff below
	int C = 0;
	int min = 0;
	int max = 0;
	int size = 1;
	int *arr = (int *) malloc(sizeof(int));
	//printf("malloc\n");
	int loop = 0;
	printf("Time to start work!\n");
	while (1){
	arr[size-1] = smap->msg[1];
	printf("loop: %d\n", loop);
	printf("Request: %d\n", smap->msg[0]);
	myprintArray(arr, size);
	loop++;
	
	if (smap->msg[0] == 1){C=mySum(arr,size);}//compute sum of data
	if (smap->msg[0] == 2){C=myProduct(arr,size);}//compute product of data
	if (smap->msg[0] == 3){C=myDiff(arr,size);}//compute difference of largest and smallest value
	if (smap->msg[0] == 4){C=myDiffLS(arr,size);}//largest3 - smallest2
	if (smap->msg[0] == 5){C=myDiffSL(arr,size);}//smallest3 - largest2
	if (smap->msg[0] == 6){C=findMax(arr,size);}//compute the largest value
	if ((smap->msg[0] > 6) || (smap->msg[0] < 1)) {
		//detach shared memory, signal director to continue, and exit
		printf("Time to leave work!\n");
		free(arr);//too lazy to zero out arr
		shmdt(smap);
		releaseSem(semid, 0);
		exit(EXIT_SUCCESS);
	}
	//store the value and signal director to continue
	smap->msg[2] = C;
	printf("Result: %d\n", C);
	size++;
	//printf("smap->msg[2]\n");
	arr = (int *) realloc(arr, size * (sizeof(int)));
	//printf("realloc\n");
	releaseSem(semid, 0);	//release producer
	reserveSem(semid, 1);	//pause worker until released
	}
}
