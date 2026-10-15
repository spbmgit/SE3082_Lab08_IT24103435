#include <mpi.h>
#include <cstdio>
#include <cstdlib>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const long long N = 10000000;
    long long mine = N / size + (rank < N % size ? 1 : 0);
    unsigned int seed = 12345 + rank;
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();
    long long inside = 0, total = 0;
    for (long long i = 0; i < mine; i++) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x * x + y * y <= 1.0) inside++;
    }
    MPI_Reduce(&inside, &total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    double el = MPI_Wtime() - t0, slowest = 0;
    MPI_Reduce(&el, &slowest, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
        printf("Processes: %d  Pi: %.6f  Time: %.6f s\n", size, 4.0 * total / N, slowest);
    MPI_Finalize();
    return 0;
}
