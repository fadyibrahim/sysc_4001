//Group Members: Tejas Patil  Student ID: 100823713, Fady Ibrahim   Student ID: 100876906


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#define SERVER_FIFO_NAME "/tmp/serv_fifo"
#define CLIENT_FIFO_NAME "/tmp/cli_%d_fifo"

#define BUFFER_SIZE 20

// The structure declaration to be followed for the FIFO
struct data_to_pass_st {
    pid_t  client_pid;
    char   connection_status[BUFFER_SIZE - 1];
    char   client_name[BUFFER_SIZE];
};

// The structure declaration to be followed for the Message Queue 
 struct my_msg_st { 

// The message type for the message sent and received
     long int my_msg_type; 

// The integer value for the heartbeat
     int  some_text; 

// The child process ID
     pid_t client_pid;

// The patient name 
     char client_name[200];

// The  acknowledge string for the monitor
     char ACK[50];
 }; 
