/* A9-SharedMemoryAndSemaphores-Director.c */
/************************************************************************
//Date due: 11/23/2021
//OS: ubuntu 20.04
//Made in: Text Editor
//Compiler: gcc (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0
//Language: C
//Role: Director
//Desc: uses shared memory and semaphores to communicate a sequence of request and responses between two programs.
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
	int val;  		  /* Value for SETVAL */
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

int main(int argc, char *argv[])
{
	struct request *smap;
	union semun arg;
	int semid, shmid;
	
	key_t semK, shmK;
	
	semK = ftok(".", IPC_PRIVATE);	//generate keys
	shmK = ftok(".", IPC_PRIVATE);
	
	srand(time(NULL));
	
	semid = semget(semK, 2, IPC_CREAT | OBJ_PERMS);	//create semaphore
	arg.val = 1;
  	semctl(semid, 0, SETVAL, arg);//set director semaphore to available
  	arg.val = 0;
  	semctl(semid, 1, SETVAL, arg);//set worker semaphore to not available
	//create shared memory
	shmid = shmget(shmK, sizeof(struct request), IPC_CREAT | OBJ_PERMS);
	
	smap = shmat(shmid, NULL, 0);		//attach shared memory
	if (smap == (void *) -1)
	{
		perror("shmat");
		exit(EXIT_FAILURE);
	}
	
	int numRequests = (rand() % 19) + 2;
	printf("numRequests: %d\n", numRequests);
	//printf("reserving director\n");
	reserveSem(semid, 0);//wait for worker to release director
	int A, B, C;
	//first pass assign values
	A = (rand() % 6)+1;
	B = (rand() % 201)-100;
	C = 0;
	smap->msg[0] = A;
	smap->msg[1] = B;
	smap->msg[2] = C;//will be modified by worker not director
	
	//signal worker that it is time to process the shared data
	//begin looping for numRequests times
	//wait for worker to complete assigned task
	//after final loop "numRequests", director tells worker that there is no more to be done and outputs the last response.
	int i = 0;
	printf("Time to hire workers!\n");
	while (i < numRequests)
	{
		printf("sending: %d\n", smap->msg[0]);
		releaseSem(semid, 1);//release worker
		reserveSem(semid, 0);//wait for worker to release director
		A = (rand() % 6)+1;
		B = (rand() % 201)-100;
		smap->msg[0] = A;
		smap->msg[1] = B;
		//printf("%d\n", smap->msg[2]);
		i++;
		printf("Response: %d\n", smap->msg[2]);
	}
	if (i == numRequests)
	{
		smap->msg[0] = (rand() % 999)+10;
		printf("sending: %d\n", smap->msg[0]);
		printf("Time to fire everyone!\n");
	}
	releaseSem(semid, 1);//release worker
	reserveSem(semid, 0);//pause producer until released
	
	shmdt(smap);	//detach shared memory
	semctl(semid, 0, IPC_RMID);	//delete semaphore
	shmctl(shmid, 0, IPC_RMID);	//delete shared memory
	exit(EXIT_SUCCESS);
}
