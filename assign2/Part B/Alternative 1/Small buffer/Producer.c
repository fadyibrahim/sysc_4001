/* After the #includes, the function prototypes and the global variable, we come to the
 main function. There the semaphore is created with a call to semget, which returns the
 semaphore ID. If the program is the first to be called (i.e. it's called with a parameter
 and argc > 1), a call is made to set_semvalue to initialize the semaphore and op_char is
 set to X. */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sem.h>
#include "Header.h"
#include <string.h>
#include <sys/shm.h>
#include "shm_com.h"


static void productInformation(int product);
static int producer(int argNum);
static void append(int v);
static int argval;
static int product;
static int tempIn;


int main(int argc, char *argv[])
{
    int pause_time;
    int i;
    argval= atoi (argv[1]);

    shared_memory = (void *)0;

    initialization_Of_SEM_SNE(argc);

    initializeSharedMemory();
    initializeBuffer();
    for(i=0;i<10;i++)
    {
    product=producer(argval);
    semaphore_p(sem_idP);
    semaphore_p(sem_idE);
    append(product);
    productInformation(product);
    semaphore_v(sem_idN);
    semaphore_v(sem_idP);
    sleep(3);
    }
    semaphore_p(sem_idP);
    append(-1);
    semaphore_v(sem_idN);
    semaphore_v(sem_idP);
    detach();    
    exit(EXIT_SUCCESS);
}


/*Produce will generate a number according to the commandline given*/
static int producer(int argNum)
{
    int minNum;
    int range=10000;
    
    argNum=argNum*range;
    minNum=argNum-(range-1);
    return argNum=rand()%range+minNum;
}

/*Places a value in critical section of shared memory */
static void append(int v)
{
	shared_stuff->buffer[shared_stuff->in]=v;
	tempIn=shared_stuff->in;
	if(shared_stuff->in==buff_size-1)
	{
	shared_stuff->in=0;
        shared_stuff->k=shared_stuff->k-1;
	}
	else
	{
	shared_stuff->in=shared_stuff->in+1;
        shared_stuff->k=shared_stuff->k-1;
	}

}

static void productInformation(int product)
{
	printf("SM %d: pid %d puts number %d at position %d\n", shmid,  getpid(), product, tempIn);

}
