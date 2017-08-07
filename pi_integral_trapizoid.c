#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
// created by Arya HajiTaheri
double f(double x){
        return 4/(1+x*x); // our exact function to find pi
}

int main(int argc, char  *argv[]) {
	int size, rank, n0, nstep;
	double start_time, end_time, elapsed_time;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if ((size % 2) != 0) {
		printf("Sorry - the number of nodes must be even\n");
		MPI_Finalize();
		exit(0);
	}
	double a = 1, b = 0, n = 50000, i,
		h, sum1, sum, summ, yn, y0;


	n0 = (rank * 2) + 1;
	nstep = size * 2;
	start_time = MPI_Wtime();
	if (rank == 0) { //first node
		printf("Using %d nodes:", size);
		h = (b - a) / n;;
		y0 = f(a + 0 * h);
		yn = f(a + n*h);
		// trapezoidal method
		for (i = 1; i < n; i++) {
			sum1 = sum1 + f(a + i*h);
		}
		sum = (h / 2)*(y0 + yn + 2 * sum1);
		MPI_Reduce(&sum, &summ, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		end_time = MPI_Wtime();

		printf("Answer : %.15lf,\nPI = %.15lf", sum, M_PI);
		printf("\n%.15lf", fabs(M_PI + sum));
		elapsed_time = end_time - start_time;
		printf("It took %lf seconds to estimate pi", elapsed_time);
	}
	if (rank > 0) { //rest of the tasks
		h = (b - a) / n;;
		y0 = f(a + 0 * h);
		yn = f(a + n*h);

		for (i = 1; i < n; i++) {
			sum1 = sum1 + f(a + i*h);
		}
		sum = (h / 2)*(y0 + yn + 2 * sum1);
		MPI_Reduce(&sum, &summ, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	}

	MPI_Finalize();
	return 0;
}
