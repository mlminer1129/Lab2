
all: SwimMill.o Fish.o Pellets.o

SwimMill.o: SwimMill.c
	gcc -o SwimMill SwimMill.c -I -lm -pthread

Fish.o: Fish.c
	gcc -o Fish Fish.c -lm -I.

Pellets.o: Pellets.c
	gcc -o Pellets Pellets.c -I -lm.

exec:
	./SwimMill
