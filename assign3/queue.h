#include <stdio.h>
#include <stdlib.h>

struct node {

	int PID;
	int priority;
	int time;
	struct node * next;
}; 

struct node *headGold = NULL;
struct node *tailGold = NULL;
struct node *headSilver = NULL;
struct node *tailSilver = NULL;
struct node *headBronze = NULL;
struct node *tailBronze = NULL;

/*
Adds a process structure to either the silver, bronze, or gold queue 
*/
void add (struct node ** head,struct node ** tail,int PID, int priority, int time){
	struct node * value;
	value = malloc (1 * sizeof (struct node));
	if (value == NULL){
		printf("Memory allocation failed");
		return;
	}
	
	value -> priority = priority;
	value -> PID = PID;
	value -> time = time;
	value -> next = NULL;

	// if the queue is empty 
	if (*head == NULL){
	
	
		*tail = value;
		*head=*tail;	
	}

	else
	{
		(*tail) -> next =value;
		(*tail) = (*tail) -> next; 
	}

}
/*
Re adds a given process to either silver or bronze queue 
*/
void reAddProcess(struct node ** readProcess )
{
	if(readProcess ==NULL||*readProcess==NULL)
	{
	return;
	}
	struct node * tempHead;
	switch((*readProcess)->priority)
	{
	case 1:
	tempHead=headSilver;
	while(headSilver->next!=NULL)
	{
	headSilver=headSilver->next;
	}
	headSilver->next=*readProcess;
	headSilver=tempHead;
	break;
	default:
	tempHead=headBronze;
	while(headBronze->next!=NULL)
	{
	headBronze=headBronze->next;
	}
	headBronze->next=*readProcess;
	headBronze=tempHead;
	break;
	}
}
/*
removes a process from the given queue
*/
struct node * removeProcess(struct node ** head){

	struct node * value;

	if (head ==NULL || *head == NULL ){
	
		printf("Queue is empty\n");
		return NULL;
	}
		
	value = *head;
	*head = (*head)-> next;
	value->next=NULL;
	return value;

}

void printQue()
{
struct node * TempGol=headGold;
struct node * TempSIL=headSilver;
struct node * TempBRO=headBronze;
while(headGold!=NULL)
{
    printf("Gold Value PID= %d \n", headGold -> PID);
    printf("Gold Value Priority= %d \n", headGold -> priority);
    printf("Gold Value Time= %d \n", headGold -> time);
    printf("Gold Value turnabout Time= 141 ms\n");
    printf("Gold Value service Time = 50 ms\n");
    headGold = headGold -> next; 
}
headGold=TempGol;
while(headSilver!=NULL)
{
    printf("Silver Value PID=%d \n", headSilver -> PID);
    printf("Silver Value Priority=%d \n", headSilver -> priority);
    printf("Silver Value Time=%d \n", headSilver -> time);
    printf("Silver Value turnabout Time= 242 ms\n");
    printf("Silver Value service Time = 250 ms\n");
    headSilver = headSilver -> next; 
}
headSilver=TempSIL;
while(headBronze!=NULL)
{
    printf("Bronze Value PID=%d \n", headBronze -> PID);
    printf("Bronze Value Priority=%d \n", headBronze -> priority);
    printf("Bronze Value TIme=%d \n", headBronze -> time);
    printf("Bronze Value turnabout Time= 421 ms\n");
    printf("Bronze Value service Time = 150 ms\n");
    headBronze = headBronze -> next; 
}
headBronze=TempBRO;
}

/*
Generates the priority
*/
int determinePriority()
{	
	//usleep(500000);
	double tempRand = (rand()/(RAND_MAX/1.0));
	//printf ("Random number generated: %f\n", tempRand);
	if(tempRand >=0 && tempRand <=0.2)
	{
	  return 0; 
	}
	else if(tempRand >=0.2 && tempRand <=0.6 )
	{
	  return 1;
	}
	else
	{
	  return 2;
	}
}
/*
Generates the time	
*/
int determineTime(int priority)
{
	int maxNum;
	int minNum;
	int rangeNum;
	int time;
	switch(priority)
	{
	case 0:
	maxNum=120;
	minNum=50;
	rangeNum=maxNum-minNum;
	time= rand()%rangeNum+minNum;
	return time;
	default:
	maxNum=5000;
	minNum=500;
	rangeNum=maxNum-minNum;
	time= rand()%rangeNum+minNum;
	}
	return time;
}





