#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include<math.h>
#include "mpi.h"
//created by Arya HajiTaheri
int main(int argc, char  *argv[]) {
	//init
	srand(time(NULL));
	long long int N = 0, modu;
	double x, y, z, pi;
	long long int i, count = 0;
	int size, rank, n0, nstep;
	printf("Enter the number of iterations used: ");
	scanf("%llu", &N);
	//N = 50000;
	count = 0;
	modu = N / 100;
	double ratio;
	double start_time, end_time, elapsed_time,pisum;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if ((size % 2) != 0) {
		printf("Sorry - the number of nodes must be even\n");
		MPI_Finalize();
		exit(0);
	}

	start_time = MPI_Wtime();
	if (rank == 0) { //first node
		printf("Using %d nodes:", size);
		//randomize
		for (i = rank; i < N; i += size) {
			ratio = (double)i / N;
			/* print what percentage is computed
			if (i % modu == 0) {
				cls();
				printf("Loading...\n %.1lf%% complete!\n", ratio * 100);
			}*/
			x = (double)rand() / RAND_MAX;
			y = (double)rand() / RAND_MAX;
			z = x*x + y*y;
			// if in circle, increase count
			if (z <= 1) count++;
		}
		pi = (double)count / N * 4;
		//printf("After %llu trials, the estimate of pi is %.16lf \n\a", N, pi);
		MPI_Reduce(&pi, &pisum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		end_time = MPI_Wtime();

		printf("Answer : %.15lf,\nPI = %.15lf\n", pisum, M_PI);
		printf("\n%.15lf%%\n", fabs(M_PI - pisum)/M_PI*100);
		elapsed_time = end_time - start_time;
		printf("\nIt took %lf seconds to estimate pi\n", elapsed_time);
	}
	if (rank > 0) {
		for (i = rank; i < N; i += size) {
			ratio = (double)i / N;
			x = (double)rand() / RAND_MAX;
			y = (double)rand() / RAND_MAX;
			z = x*x + y*y;
			if (z <= 1) count++;
		}
		pi = (double)count / N * 4;
		MPI_Reduce(&pi, &pisum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


	}
	MPI_Finalize();
	return 0;

}
