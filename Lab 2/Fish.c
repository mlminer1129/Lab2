#include <math.h>
#include <stdio.h>
#include "include.h"
//give size of shared memory
#define SHM_SIZE 1000

void exeptionHandler(int);
int findClosestPellet(int*);
void moveLeft(int, int*);
void moveRight(int, int*);

int main(int argc, char* argv[]) {
    //signal for our exit condition
    signal(SIGINT, exeptionHandler);
    //from .h
    attachSharedMemory();
    //Starting middle position and storing to first memory location
    int fish = 94; 
    sharedMemory[0] = fish;

  while(1) {
    int closestPellet = sharedMemory[findClosestPellet(sharedMemory)];
    if ((closestPellet % 10) > (fish % 10) ) {
      moveRight(fish, sharedMemory);
    }
    else if ((closestPellet % 10) < (fish % 10)) {
      moveLeft(fish, sharedMemory);
    }
    else{

    }
    // moveRight(fish,sharedMemory);
    sleep(1);
  }
  shmdt(sharedMemory);
  return 0;
}

int findClosestPellet(int* sharedMemory) {
  int closestPellet = 0;
  int distance[20] = {0}; 
  int minimumDistance = 0;

  int x2 =  sharedMemory[0] % 10;
  int y2 = sharedMemory[0] / 10;

  for (int i = 1; i < 20; i++) {
    int x1 = sharedMemory[i] % 10;
    int y1 = sharedMemory[i] / 10;
    //maintaining shortest distance utilizing distance formule from Algebra
    distance[i] = pow((x2 - x1), 2) + pow((y2 - y1), 2);
    //distance[i] = ((x2-x1)*(x2-x1)) + ((y2-y1)*(y2-y1));
  }
  minimumDistance = distance[1];

  //Finding smallest distance between fish and pellet
  for (int i = 2; i < 20; i++) {
    if (distance[i] < minimumDistance) {
      closestPellet = i;
    }
  }
  return closestPellet;
}

void moveLeft(int fish, int* sharedMemory) {
  if (sharedMemory[0] <= 90) {
  }
  else{
    fish--;
    //Decrementing column and moving the fish left
    sharedMemory[0]--; 
  }
}

void moveRight(int fish, int* sharedMemory) {
  if (sharedMemory[0] >= 99){
  }
  else{
    fish++;
    //Incrementing column and moving the fish right
    sharedMemory[0]++;
  }
}

void exeptionHandler(int num) {
    //incase something goes wrong
    shmdt(sharedMemory);
	exit(1);
}