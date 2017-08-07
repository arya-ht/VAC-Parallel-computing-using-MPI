#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//created by Arya HajiTaheri

#define NMAX  10000000    //Maximum number to scan.
int isprime(int n) {
	int i, nsqrt;
	if (n > 10) {
		nsqrt = (int)sqrt(n);
		for (i = 3; i <= nsqrt; i += 2)
			if ((n%i) == 0) {
				return 0; // return 0 if not prime
			}
		return 1; //return "true" if is prime
	}

	else
		return 0;
}


int main(int argc, char *argv[])
{
	int size,rank,n,prime,maxprime,n0,nstep;

	double start_time, end_time,elapsed_time;
	//init
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//only take even node
	if ((size % 2) != 0) {
		printf("Sorry - the number of nodes must be even\n");
		MPI_Finalize();
		exit(0);
	}

	n0 = (rank * 2) + 1;
	nstep = size * 2;
	prime = 0;
	start_time = MPI_Wtime();
	if (rank == 0) { //first node
		printf("Using %d nodes:", size);
		for (n = n0; n <= NMAX; n = n + nstep) {
			if (isprime(n)) {
				prime = n;
			}
		}   
		MPI_Reduce(&prime, &maxprime, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
		end_time = MPI_Wtime();
		elapsed_time = end_time - start_time;
		printf("It took %lf seconds to calculate prime %d numbers", elapsed_time, NMAX);
	}
	if (rank > 0) { //rest of the tasks
		for (n = n0; n <= NMAX; n = n + nstep) {
			if (isprime(n)) {
				prime = n;
				printf("%d is a prime number.", prime); //print primes
			}
		}
		MPI_Reduce(&prime, &maxprime, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}

