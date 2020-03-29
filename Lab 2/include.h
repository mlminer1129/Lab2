#include <stdio.h>
//used for messages, semaphores, and shared memory
#include <sys/ipc.h>
//used for pid_t and can be used for rand time or round time
#include <sys/shm.h>
//provides access to POSIX OS API
#include <unistd.h>
#include <stdlib.h>
//will be used for signal names such as pid_t, SIGKILL, SIGINT, SIGTOP
#include <signal.h>
//give size of shared memory
#define SHM_SIZE 1000

int shmid;
int *sharedMemory;
//to declare a variable that is going to be used to deal with process ids use pid_t
pid_t fish;
pid_t pellet;
//keep track of number of pellets that have been put into the swimmill
static int pelletCount;
//File folder with pointer
FILE *fp;


void attachSharedMemory() {
    //generate random key
    key_t key = ftok("SwimMill.c", 'b'); 
    //need to make the shared memory space and grant permission using 0666
    shmid = shmget(key, SHM_SIZE, IPC_CREAT|0666);
    sharedMemory = shmat(shmid, NULL, 0);

}