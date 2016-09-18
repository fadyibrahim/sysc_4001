/* A common header file to describe the shared memory we wish to pass about. */

#define buff_size 10

struct shared_buffer {
    int isInit;
    int k;
    int in;
    int out;
    int buffer[buff_size];
};

// prototype for the shared methods of producer and consumer.
static void initializeSharedMemory();
static void detach();
static int initializeBuffer();

//global static variables for both the consumer and the producer
static int running;
static void *shared_memory;
static struct shared_buffer *shared_stuff;
static char buffer[BUFSIZ];
static int shmid;

/*Creates and attaches the shared memory to the program.*/
static void initializeSharedMemory()
{
    shmid = shmget((key_t)1234, sizeof(struct shared_buffer), 0666 | IPC_CREAT);

    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n", (int)shared_memory);
}


/* Lastly, the shared memory is detached*/
static void detach()
{
    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    printf("detaching the producer from the shared memory.");
}

/*This function initializes the buffer and all the members of the struct (in shared memory)*/
static int initializeBuffer()
{
    shared_stuff = (struct shared_buffer *)shared_memory;
    if(shared_stuff->isInit==1)
    {
	return 0;
    }
    shared_stuff->k=buff_size;
    shared_stuff->in=0;
    shared_stuff->out=0;
    shared_stuff->isInit=1;
    return 1;
}

