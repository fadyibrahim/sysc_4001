
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "semun.h"
#include <sys/sem.h>
#define buff_size 50

static void del_semvalue(int sem_id);
static int semaphore_p(int sem_id);
static int semaphore_v(int sem_id);
static int set_semvalue(int sem_id, int val);

static int sem_idP;
static int sem_idC;
static int sem_idN;
static int sem_idE;

struct buffer {
	int k;
	int buff[buff_size];
};

static void initialization_Of_SEM_SNE(int argc)
{
    sem_idP = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    sem_idC = semget((key_t)1232, 1, 0666 | IPC_CREAT);
    sem_idN = semget((key_t)1235, 1, 0666 | IPC_CREAT);
    sem_idE = semget((key_t)1233, 1, 0666 | IPC_CREAT);
    if (argc > 1) {

        if (!set_semvalue(sem_idN, 0)) {
            fprintf(stderr, "Failed to initialize semaphore N\n");
            exit(EXIT_FAILURE);
        }
        if (!set_semvalue(sem_idE, buff_size)) {
            fprintf(stderr, "Failed to initialize semaphore E\n");
            exit(EXIT_FAILURE);
        }
        if (!set_semvalue(sem_idP, 1)) {
            fprintf(stderr, "Failed to initialize semaphore N\n");
            exit(EXIT_FAILURE);
        }
        if (!set_semvalue(sem_idC, 1)) {
            fprintf(stderr, "Failed to initialize semaphore E\n");
            exit(EXIT_FAILURE);
        }
      }
}



/* semaphore_v is similar except for setting the sem_op part of the sembuf structure to 1,
 so that the semaphore becomes available. */

static int semaphore_v(int sem_id)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}



/* semaphore_p changes the semaphore by -1 (waiting). */

static int semaphore_p(int sem_id)
{
    struct sembuf sem_b;
    
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}



/* The del_semvalue function has almost the same form, except the call to semctl uses
 the command IPC_RMID to remove the semaphore's ID. */

static void del_semvalue(int sem_id)
{
    union semun sem_union;
    
    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        fprintf(stderr, "Failed to delete semaphore\n");
}

/* The function set_semvalue initializes the semaphore using the SETVAL command in a
 semctl call. We need to do this before we can use the semaphore. */

static int set_semvalue(int sem_id, int val)
{
    union semun sem_union;

    sem_union.val = val;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1) return(0);
    return(1);
}

