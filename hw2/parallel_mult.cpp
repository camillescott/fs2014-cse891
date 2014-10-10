#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

#include "hw2_common.hpp"
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {

    int pid;
    int P;

    printf("INIT\n");
    MPI::Init(argc, argv);
    P = MPI::COMM_WORLD.Get_size();
    pid = MPI::COMM_WORLD.Get_rank();

    MPI_Comm row_comm, col_comm;

    int N = atoi(argv[1]);
    int block_size = N / sqrt(P);
    int N_block = N / block_size;

    int i_C = pid / (int) sqrt(P);
    int j_C = pid % (int) sqrt(P);

    double ** A_ij;
    double ** B_ij;
    double ** C_ij;

    printf("CPU %d alloc A, B, C\n", pid);

    A_ij = alloc_matrix<double>(block_size, block_size, (double) pid);
    B_ij = alloc_matrix<double>(block_size, block_size, (double) pid);
    C_ij = alloc_matrix<double>(block_size, block_size, 0.0);
    
    printf("CPU %d COMM_Split %d\n", pid, i_C);

    MPI_Comm_split(MPI_COMM_WORLD, i_C, pid, &row_comm);
    MPI_Comm_split(MPI_COMM_WORLD, j_C, pid, &col_comm);

    if (pid == 0) {
	cout << endl;
	print_matrix<double>(A_ij, block_size, block_size);
    }

    double ** X_i = alloc_matrix<double>(block_size, N, 0.0);
    double ** Y_j = alloc_matrix<double>(block_size, N, 0.0);

    for (int i=0; i<block_size; ++i) {
	MPI_Allgather(A_ij[i], block_size, MPI_DOUBLE, X_i[i], block_size, MPI_DOUBLE, row_comm);
	MPI_Allgather(B_ij[i], block_size, MPI_DOUBLE, Y_j[i], block_size, MPI_DOUBLE, col_comm);
    }

    if (pid == 0) {
	cout << endl;
	print_matrix<double>(X_i, block_size, N);
	cout << endl;
	print_matrix<double>(Y_j, block_size, N);
    }

    MPI_Finalize();

    return 0;
}
