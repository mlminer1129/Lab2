#include <time.h>
#include "include.h"
//give size of shared memory
#define SHM_SIZE 1000
//initializing variables
void handler(int);
void eatPellet();
void missPellet();

int main(int argc, char* argv[]) {

  signal(SIGINT, handler);

  attachSharedMemory();
  sharedMemory[20]++;
  int i = sharedMemory[20];
  //randomly generate a pellet
  for (; i < 20; i++) {
    srand(time(NULL));
    sharedMemory[i] = rand() % 9 ;

    break;
  }
 
  sleep(1);

  while(1) {
    sleep(1);
    if (sharedMemory[i] < 90) {
      sharedMemory[i] += 10;
    }
    else if (sharedMemory[i] == sharedMemory[0]) {
        //pellet is eaten and terminated then printed
        eatPellet();
        printf("Position: %d\n", sharedMemory[i] );
        sharedMemory[i] = -1;
        break;
    }
    else {
      missPellet();
      //if missed pellet is terminated and not eaten
      printf("Position: %d\n", sharedMemory[i] );
      sharedMemory[i] = 0;
      break;
    }

    i++;

  }
  shmdt(sharedMemory);
  return 0;
}
//If pellet is eaten say so and return PID
void eatPellet() {
  printf("Pellet was eaten! \n");
  printf("PID: %d \n", getpid());
}
//If pellet is missed say so and return PID
void missPellet() {
  printf("Pellet was missed by the slow, slow fish\n");
  printf("PID: %d \n", getpid());
}
//incase something goes wrong
void handler(int num) {
  shmdt(sharedMemory);
  exit(1);
}