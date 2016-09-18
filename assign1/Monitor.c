//Group Members: Tejas Patil  Student ID: 100823713, Fady Ibrahim   Student ID: 100876906


#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "Monitor.h"
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <sys/msg.h>
#include <time.h>

// Global variables for the message Queue Id and the clientname

int msgQid;
char clientname[200];

// the child monitor processes are defined in this function
void start_childmonitorprocess()
{

// local variables for the child function
struct my_msg_st some_data;
int r = 0;
long int msg_to_receive = 0;

    // Error checking to ensure the message queue ID is valid
    if (msgQid == -1){

	    fprintf(stderr, "msgget failed with error: %d\n", errno);
	    exit(EXIT_FAILURE);
    }

    int a = 0, b= 0;

    // infinite while loop to provide the feature of monitoring the heartbeat periodically
    while(1==1){

	    // buffer  to store the size of the bytes received
	    int size = sizeof(struct  my_msg_st) - sizeof(long int);  
	 	
	    // random number generator between 60 and 100
	    srand ( time(NULL) );
	    r = rand() % (101 - 60) + 60;
	  
	    // setting the values for the struct to be pointed to 
	    some_data.my_msg_type = 1;
	    some_data.some_text = r;
	    some_data.client_name[0]=(char)0;
	    strcpy(some_data.client_name, clientname);
	    some_data.client_pid = getpid();

	    // monitoring interval of three seconds 
	    sleep(3);

	    // Sends the message through the message queue 
	    a = msgsnd(msgQid, (void *)&some_data, size, 0);	

	    // Error checking to ensure the send is successful
 	    if (a == -1) {
		    fprintf(stderr, "msgsnd failed\n");
		    exit(EXIT_FAILURE);
            }

	    // Receives the message through the message queue
	    b =  msgrcv(msgQid, (void *) &some_data, size, msg_to_receive, 0);

	    // Error checking to ensure the receive is successful
	    if (b == -1) {
		    fprintf(stderr, "msgrcv failed with error: %d\n", errno);
		    exit(EXIT_FAILURE);
            }

	    printf("Confirmation from controller: %s\n", some_data.ACK);
    }
}

/*terminates the child process cleaning*/ 
void kill_childprocess(int sig)
{
  exit(0);
}
int main(int argc, char *argv[] )
{
    struct sigaction  terminatemonitorchild;
    int server_fifo_fd, client_fifo_fd;
    struct data_to_pass_st my_data;
    int times_to_send;
    char client_fifo[256];
    pid_t pid;
//copy the command line argument into the clientname string
    strcpy(clientname,argv[1]);
//create a message queue to send patient data to the controller
    msgQid = msgget(ftok("/home/LABS", 1), 0666 | IPC_CREAT); 
    if (msgQid == -1) {
       	fprintf(stderr, "msgget failed with error: %d\n", errno);
       	exit(EXIT_FAILURE);
    }
//check the command line arugments passed to makesure not more than one are entered
    if(argc!= 2){ 
	printf("Error: Only one command line argument parameter specified. Please run the program again with two command line argument parameters specified. Ensure the second parameter is a patient name.\n"); 
        exit(0);
    }
//attach to the server fifo as write only
    server_fifo_fd = open(SERVER_FIFO_NAME, O_WRONLY);
    if (server_fifo_fd == -1) {
        fprintf(stderr, "Sorry, no server\n");
        exit(EXIT_FAILURE);
    }

    my_data.client_pid = getpid();
    sprintf(client_fifo, CLIENT_FIFO_NAME, my_data.client_pid);
//make client fifo so that the server cans send informattion back to the monitor
    if (mkfifo(client_fifo, 0777) == -1) {
        fprintf(stderr, "Sorry, can't make %s\n", client_fifo);
        exit(EXIT_FAILURE);
    }
     sprintf(my_data.connection_status, "connect");
     printf("Attempting a connection to Controller\n");
//write a sting with a value of connect and send it to the server fifo
     write(server_fifo_fd, &my_data, sizeof(my_data));
     client_fifo_fd = open(client_fifo, O_RDONLY);
     if (client_fifo_fd != -1) {
        while(1==1){

         if (read(client_fifo_fd, &my_data, sizeof(my_data)) > 0) {
           if((strcmp(my_data.connection_status,"start")==0))
	    {
//creating child that will send patient information via message queue
	    pid = fork();
	    switch(pid) {
	    case -1:
	      /* Failure */
	      perror("fork failed");
	      exit(1);
	    case 0:
	      /* child */
//set up the sigaction interface to listen to SIGTERM interrupts
	      terminatemonitorchild.sa_handler=kill_childprocess;
	      sigemptyset(&terminatemonitorchild.sa_mask);
	      terminatemonitorchild.sa_flags=0;
	      sigaction(SIGTERM,&terminatemonitorchild,0);
	      start_childmonitorprocess();
		}
            }
/*compare the string stop to what is currently held. If stop is the string held proceed to terminate process.Else continue reading from the fifo.*/
           if((strcmp(my_data.connection_status,"stop")==0))
	    {
	      printf("Stop command received terminating monitor\n.");
	      kill(pid, SIGTERM); 
	      break;
            }
	 }}
//If the monitor is not reading from the client_fifo_fd fifo close the connection
            close(client_fifo_fd);
       
     }
//cleaning resources by removing all fifo connections and existing the monitor process
    close(server_fifo_fd);
    unlink(client_fifo);
    exit(EXIT_SUCCESS);
    
}
