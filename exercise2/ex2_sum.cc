// ex2_sum.cc - sum 1..N in parallel with MPI
#include <mpi.h>
#include <cstdio>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long N = 10000000;
    long long start = rank * N / size + 1;        // this process's slice
    long long end   = (rank + 1) * N / size;

    MPI_Barrier(MPI_COMM_WORLD);                   // start all together
    double t0 = MPI_Wtime();

    long long local = 0;
    for (long long i = start; i <= end; i++) local += i;

    long long total = 0;
    MPI_Reduce(&local, &total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - t0, slowest = 0;
    MPI_Reduce(&elapsed, &slowest, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0)
        printf("Processes: %d  Sum: %lld  Time: %.6f s\n", size, total, slowest);

    MPI_Finalize();
    return 0;
}
