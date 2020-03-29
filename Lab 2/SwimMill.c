#include <semaphore.h>
#include <time.h>
#include "include.h"
//give size of shared memory
#define SHM_SIZE 1000

//initializing variables

void printMill(int*);
void exitCondition(int);
void killProgram(pid_t, pid_t, int*, int);
pid_t fish;
pid_t pellet;

int main(int argc, char* argv[]) {
	fp = fopen("attempts.txt", "w");
    //initiate the timer
	int timer = 0;
    // from include.h
	attachSharedMemory(); 
    //explained at top of .h file
	signal(SIGINT, exitCondition);

	// Declaring semaphore variables sem_t will return 0 on success else -1
	sem_t semaphoreVariable;
	int pshared = 1;
    unsigned int value = 1;
	if((sem_init(&semaphoreVariable, pshared, value)) == 1)
	 {
         //perror used to include our own error message
		 perror("error encountered when firing up the semaphore");
		 exit(1);
	 }


	// Initializing the shared memory to prevent segmentation fault
	for (int i = 0; i < SHM_SIZE; i++){
		sharedMemory[i] = -1;
	}
	sharedMemory[20] = 0;
    //Not a dinner fork, but used to create a child process witch runs concurrently with the parent process
	fish = fork();
    //error conditions as mentioned before 0 good -1 bad
	if (fish == -1) {
        //The C library function perror prints a descriptive error message to stderr
		perror("fork failed");
        //self explanitory, terminates the calling process immediately
		exit(1);
	}	
        else if (fish == 0) {
        //replaces the current process image with a new process image specified by path
		execl("Fish", "Fish", NULL);
        //The C library function perror prints a descriptive error message to stderr
		perror("Fish exec failed");
        //self explanitory, terminates the calling process immediately
		exit(1);
	}

	while(timer <= 30){ // Run for 30 seconds
		pellet = fork();
		sem_wait(&semaphoreVariable);
		if (pellet == -1) {
			perror("Pellet Fork failed1");
			exit(1);
		}	else if (pellet == 0) { // Child pellet returns 0
			execl("./Pellets", "Pellets", NULL);
			perror("Pellets Fork failed2");
            //self explanitory, terminates the calling process immediately
			exit(1);
		}
		printMill(sharedMemory);
		sleep(1);
		printf("Timer: %d\n", timer);
		fprintf(fp, "Timer: %d\n", timer);
		timer++;
		//unlock the semaphore used in the ()  
		sem_post(&semaphoreVariable);
	}
	//terminates the programs
	killProgram(fish, pellet, sharedMemory, shmid);
	fclose(fp);
	getchar();
	return 0;
}


void printMill(int* sharedMemory) {
	int row = 10;
	int column = 10;
	char stream[row][column]; // Dont use pointer! Causes bad address!

	//showing the swimmill
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
            // '~' are the pellets, don't use pointer on stream because will cause problems
            // Utilizing example formula in lab PDF
			stream[i][j] = '~';
		}
	}
	printf("Fish Position: %d \n", sharedMemory[0]);
	fprintf(fp,"Fish Position: %d \n", sharedMemory[0]);
	printf("Pellet Position: %d \n", sharedMemory[1] );
	fprintf(fp,"Pellet Position: %d \n", sharedMemory[1] );
	for (int k = 1; k < 20; k++) {
        // 'O' are the pellets, don't use pointer on stream because will cause problems
        // Utilizing example formula in lab PDF
		// less than 20 pellets at a time
		stream[sharedMemory[k]/10][sharedMemory[k]%10] = 'O'; 
	}
    // 'Y' is the fist, don't use pointer on stream bad things will happen
    // Utilizing example formula in lab PDF
	stream[sharedMemory[0]/10][sharedMemory[0]%10] = 'Y';
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < column; j++) {
			printf("%c ", stream[i][j]);
		}
		printf("\n");
	}

	if (sharedMemory[1] == sharedMemory[0]) {
	fprintf(fp, "Pellet has been eaten! \n");
	    // return the PID that was eaten
	    fprintf(fp,"PID: %d \n", getpid());
		// eat the pellet then kill it with the -1
		sharedMemory[1] = -1;		
	}
	else{
	    //pellet was miss, but still must be taken out of the memory
	    fprintf(fp,"Pellet was missed! \n");
	    //return the PID that was missed
	    fprintf(fp,"PID: %d \n", getpid());
		
		sharedMemory[1] = 0;

	}

}

void killProgram(pid_t fish, pid_t pellet, int *sharedMemory, int shmid) {
    //sends a signal to the processes specified by the pid SIGUSR1 will be the useful signal here
	kill(fish,SIGUSR1);
	kill(pellet, SIGUSR1);
    //sleep for 3 seconds
	sleep(3);
    //detach shared memory
	shmdt(sharedMemory);
    //from the System V PDF, performs the control operation specified in the next command
	shmctl(shmid, IPC_RMID, NULL);
    //let user know to exit the program
	printf("Program has completed, press something to continue \n");
}

void exitCondition(int num ) { // Ctrl C exits program
	kill(fish,SIGUSR1);
	kill(pellet, SIGUSR1);
    //detach shared memory
	shmdt(sharedMemory);
    //from the System V PDF, performs the control operation specified in the next command
	shmctl(shmid, IPC_RMID, NULL);
	perror("Interrupt signal is pressed!! \n");
    //self explanitory, terminates the calling process immediately
	exit(1);
}