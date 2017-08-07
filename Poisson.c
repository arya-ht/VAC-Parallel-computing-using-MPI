#include <stdio.h>
#include <math.h>
#include "mpi.h"
//created by Arya HajiTaheri
//works on 8 nodes only
//this is 24x24 rectangular matrix
#define N 24

int main(int argc, char *argv[])
{
	// init
	int        rank, value, size, i, j, i0, i_n;
	double     tol, tol_o, xu[(24 / 4) + 2][12], x_u1[(24 / 3) + 2][12],
	start_time, end_time, elapsed_time, pisum;

	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	//only use 8 nodes
	if (size != 8) { MPI_Abort(MPI_COMM_WORLD, 1); }
	i0 = 1;
	i_n = N / size;
	if (rank == 0) { i0++; } 
	if (rank == size - 1) { i_n--; }

	for (i = 1; i <= N / size; i++) {
		for (j = 0; j < N; j++) {
			xu[i][j] = rank;
		}
	}
	for (j = 0; j < N; j++) {
		xu[i0 - 1][j] = -1;
		xu[i_n + 1][j] = -1;
	}
	start_time = MPI_Wtime();

	do {
		if (rank < size - 1)
			MPI_Send(xu[N / size], N, MPI_DOUBLE, rank + 1, 0,
				MPI_COMM_WORLD);
		if (rank > 0)
			MPI_Recv(xu[0], N, MPI_DOUBLE, rank - 1, 0,
				MPI_COMM_WORLD, &status);
		if (rank > 0)
			MPI_Send(xu[1], N, MPI_DOUBLE, rank - 1, 1,
				MPI_COMM_WORLD);
		if (rank < size - 1)
			MPI_Recv(xu[N / size + 1], N, MPI_DOUBLE, rank + 1, 1,
				MPI_COMM_WORLD, &status);
		tol = 0.0;
		for (i = i0; i <= i_n; i++)
			for (j = 1; j < N - 1; j++) {
				x_u1[i][j] = (xu[i][j + 1] + xu[i][j - 1] +
					xu[i + 1][j] + xu[i - 1][j]) / 4.0;
				tol += (x_u1[i][j] - xu[i][j]) *
					(x_u1[i][j] - xu[i][j]);
			}
		/* Only transfer the interior points */
		for (i = i0; i <= i_n; i++) {
			for (j = 1; j < N - 1; j++) {
				xu[i][j] = x_u1[i][j];
			}
		}

		MPI_Allreduce(&tol, &tol_o, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
		tol_o = sqrt(tol_o);
		if (rank == 0){ printf("%e\n",tol_o);
	}
	} while (tol_o > 1.0e-3);
	end_time = MPI_Wtime();
	elapsed_time = end_time - start_time;//timestamp
	printf("\nIt took %lf seconds\n", elapsed_time);
	MPI_Finalize();
	return 0;
}