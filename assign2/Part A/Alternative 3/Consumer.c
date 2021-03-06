/* After the #includes, the function prototypes and the global variable, we come to the
 main function. There the semaphore is created with a call to semget, which returns the
 semaphore ID. If the program is the first to be called (i.e. it's called with a parameter
 and argc > 1), a call is made to set_semvalue to initialize the semaphore and op_char is
 set to X. */

#include <unistd.h>
#include <stdlib.h>
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


static void delete();
static int take();
static void consumeProduct(int product);
static int tempOut;
static int isProdDone;
static int tempHolderProduct;


int main(int argc, char *argv[])
{


    
    int i;
    int consumer;
    int pause_time;
    int running =1;
    shared_memory = (void *)0;

    initialization_Of_SEM_SNE(argc);

    initializeSharedMemory();
    initializeBuffer();
   
    semaphore_p(sem_idC);
    shared_stuff->numConsumers++;
    semaphore_v(sem_idC);
 
     time (&begin);
    while(running){
    semaphore_p(sem_idN);
    semaphore_p(sem_idC);
    tempHolderProduct=take();
    semaphore_v(sem_idC);
    semaphore_v(sem_idE);
    if(tempHolderProduct!=-1)
    {
    consumeProduct(tempHolderProduct);
    }
    else
    {
     running=0;
    }

    sleep(2);
    }
    shared_stuff->numConsumers--;
    semaphore_v(sem_idN);
    if (  shared_stuff->numConsumers == 0){
    del_semvalue(sem_idN);
    del_semvalue(sem_idE);
    delete();    
    }
    detach();
   
  
   
    exit(EXIT_SUCCESS);

  
}



/* The shared memory is deleted. */
static void delete()
{
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
}


/* Take the product from the buffer*/
static int take()
{
        int temp=shared_stuff->buffer[shared_stuff->out];
	tempOut=shared_stuff->out;
	if(temp!=-1)
	{
	if(shared_stuff->out==buff_size-1)
	{
 	shared_stuff->out=0;
        shared_stuff->k=shared_stuff->k+1;
        }
	else
        {
 	shared_stuff->out=shared_stuff->out+1;
        shared_stuff->k=shared_stuff->k+1;
	}
	}
	return temp;
}

/*Consumes the product by printing it out. */
static void consumeProduct(int product)
{
	printf("SM %d: pid %d takes number %d from position %d\n", shmid,  getpid(), product, tempOut);

}
