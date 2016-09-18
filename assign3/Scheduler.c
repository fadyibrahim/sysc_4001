#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "queue.h"
#include <semaphore.h>

void *thread_function(void *arg);
pthread_mutex_t work_mutex; /* protects both work_area and time_to_exit */

#define WORK_SIZE 1024
char work_area[WORK_SIZE];
int time_to_exit = 0;
struct node * memGold;
struct node * memSilver;
struct node * memBronze;
void process(int priority, int time, int PID);
void execute(struct node ** head);
int quantumTimeBronze = 400;
int quantumTimeSilver = 300;
sem_t goldQueue_sem;
sem_t silverQueue_sem;
sem_t bronzeQueue_sem;
pthread_t firstProcessor;
pthread_t secondProcessor;
pthread_t thirdProcessor;
pthread_t fourthProcessor;

int main() {
    int res;
    int i;

    void *thread_result;
    srand((unsigned) time (NULL));
    res = sem_init(&goldQueue_sem, 0, 1);
    if (res != 0) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    res = sem_init(&silverQueue_sem, 0, 1);
    if (res != 0) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    res = sem_init(&bronzeQueue_sem, 0, 1);
    if (res != 0) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
    for(i=0;i<=30;i++)
    {
	int temp =determinePriority();
	int time =determineTime(temp);
	process(temp,time,i);
    }
    memGold=headGold;
    memSilver=headSilver;
    memBronze=headBronze;
    printQue();	
			
    res = pthread_create(&firstProcessor, NULL, thread_function, NULL);
    res = pthread_create(&secondProcessor, NULL, thread_function,NULL);
    res = pthread_create(&thirdProcessor, NULL, thread_function, NULL);
    res = pthread_create(&fourthProcessor, NULL, thread_function,NULL);

    res = pthread_join(firstProcessor, &thread_result);
    res = pthread_join(secondProcessor, &thread_result);
    res = pthread_join(thirdProcessor, &thread_result);
    res = pthread_join(fourthProcessor, &thread_result);
    printQue();
    sem_destroy(&goldQueue_sem);
    sem_destroy(&silverQueue_sem);
    sem_destroy(&bronzeQueue_sem);
    free(memGold);
    free(memSilver);
    free(memBronze);  
    
    exit(EXIT_SUCCESS);
}

/*
This method excutes the first process by passing in the pointer to the ready Queue
*/
void execute(struct node ** head)
{
	if(head==NULL ||*head==NULL)
	{
	  printf("Queue is empty\n");
	  return;
	}
	struct node *tempProcess;
	tempProcess=removeProcess(&(*head));
	if (tempProcess==NULL)
	{
	printf("tempProcess =NULL\n");
	return;
	}
	//according to the priority(which determines the queue)
	//it will either execute the whole program or take time off
	//the exectution and replace the program at the back of the 		//queue due to the quantum time.
	switch(tempProcess->priority)
	{
	  case 0:
		usleep(tempProcess->time*1000);
	  break;
	  case 1:
		if(tempProcess->time<=quantumTimeSilver)
		{
		usleep(tempProcess->time*1000);
		}
		else
		{
		tempProcess->time=tempProcess->time-quantumTimeSilver;
		usleep(quantumTimeSilver*1000);
		//reAddProcess(&tempProcess);
		}
	  break;
	  default:
		if(tempProcess->time<=quantumTimeBronze)
		{
		usleep(tempProcess->time*1000);
		}
		else
		{
		usleep(quantumTimeBronze*1000);
		tempProcess->time=tempProcess->time-quantumTimeBronze;
	    //reAddProcess(&tempProcess);		
		}
	  break;
	}
	
	
}
/*
This function creates the process by adding, to the appropriate ready Queue, a structure that will represent the process
*/
void process(int priority, int time, int PID)
{
	switch(priority)
	{
	  case 0:
	  	add(&headGold,&tailGold,PID,priority,time);
	  break;
	  case 1:
	  	add(&headSilver,&tailSilver,PID,priority,time);
	  break;
	  default:
		add(&headBronze,&tailBronze,PID,priority,time);
	  break;
	}

}
	
void *thread_function(void *arg) {
    int tempRatio;
    int max;
    int min;
    int range;
    while(headGold!=NULL ||headBronze!=NULL ||headSilver!=NULL)
    {
    if(headGold!=NULL)
    {
    sem_wait(&goldQueue_sem);
	execute(&headGold);
	sem_post(&goldQueue_sem);
    }
    else
    {
	max=30;
	min=0;
	range=max-min;
	tempRatio= rand()%range+min;
	if(tempRatio>=0 && tempRatio<=10)
	{
	if(headBronze!=NULL)
	{
	sem_wait(&bronzeQueue_sem);
	execute(&headBronze);
    sem_post(&bronzeQueue_sem);
	}
	}
	if(tempRatio>10 && tempRatio<=30)
	{
	if(headSilver!=NULL)
	{
	sem_wait(&silverQueue_sem);
	execute(&headSilver);
    sem_post(&silverQueue_sem);
	}
	}	
     }
    }

    printf("thread finished \n");
    pthread_exit(0);
}
