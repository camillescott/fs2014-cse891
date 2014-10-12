#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

#include "hw2_common.hpp"
#include "mpi.h"

#define BLOCK 16

using namespace std;

int main(int argc, char* argv[]) {

    int pid;
    int P;

    MPI::Init(argc, argv);
    P = MPI::COMM_WORLD.Get_size();
    pid = MPI::COMM_WORLD.Get_rank();
    double start_t = MPI_Wtime();
    MPI_Comm row_comm, col_comm;

    int N = atoi(argv[1]);
    int block_size = N / sqrt(P);
    int N_block = N / block_size;

    int i_C = pid / (int) sqrt(P);
    int j_C = pid % (int) sqrt(P);

    double ** A_ij;
    double ** B_ij;
    double ** C_ij;

    A_ij = alloc_matrix<double>(block_size, block_size, (double) 1.0);
    B_ij = alloc_matrix<double>(block_size, block_size, (double) 1.0);
    C_ij = alloc_matrix<double>(block_size, block_size, 0.0);
    
    MPI_Comm_split(MPI_COMM_WORLD, i_C, pid, &row_comm);
    MPI_Comm_split(MPI_COMM_WORLD, j_C, pid, &col_comm);

    double ** X_i = alloc_matrix<double>(block_size, N, 0.0);
    double ** Y_j = alloc_matrix<double>(block_size, N, 0.0);

    // Distribute locally produced blocks out to all processors
    for (int i=0; i<block_size; ++i) {
	MPI_Allgather(A_ij[i], block_size, MPI_DOUBLE, X_i[i], block_size, MPI_DOUBLE, row_comm);
	MPI_Allgather(B_ij[i], block_size, MPI_DOUBLE, Y_j[i], block_size, MPI_DOUBLE, col_comm);
    }

    // Do local matrix mults to fill C_ij
    for (int b=0; b<N_block; ++b) {
	// Instead of copying data, just redirect some pointers and use the local
	// A and B block matrices, seeing as we've already distributed their data
	for (int i=0; i<block_size; ++i) {
	    A_ij[i] = &(X_i[i][b*block_size]);
	    B_ij[i] = &(Y_j[i][b*block_size]);
	}
	double ** tmp = naive_blocked_mat_mult(A_ij, B_ij, block_size, BLOCK);
	for (int i=0; i<block_size; ++i) {
	    for (int j=0; j<block_size; ++j) {
		C_ij[i][j] += tmp[i][j];
	    }
	}
    }

    double ** C;
    //cout << "FINAL RESULT =" << endl;
    //print_matrix<double>(C_ij, block_size, block_size);

    //delete[][] A_ij;
    //delete[][] B_ij;
    //delete[][] X_i;
    //delete[][] Y_j;

    if (pid == 0) {
	C = alloc_matrix(N, N, 0.0);

	for (int i=0; i<block_size; ++i) {
	    for (int j=0; j<block_size; ++j) {
		C[i][j] = C_ij[i][j];
	    }
	}
	MPI_Status stat;
	double ** recv_buffer = alloc_matrix<double>(block_size, block_size, 0.0);
	for (int rpid=1; rpid<P; ++rpid) {
	    //printf("RECV from CPU %d\n", rpid);
	    MPI_Recv(&(recv_buffer[0][0]), block_size*block_size, MPI_DOUBLE, rpid, 0, MPI_COMM_WORLD, &stat);
	    int drow = rpid / (int) sqrt(P);
	    int dcol = rpid % (int) sqrt(P);
	    //print_matrix<double>(recv_buffer, block_size, block_size);
	    for (int i=0; i<block_size; ++i) {
		for (int j=0; j<block_size; ++j) {
		    C[drow*block_size+i][dcol*block_size+j] = recv_buffer[i][j];
		}
	    }
	}
    } else {
	//printf("CPU %d SEND\n", pid);
	//print_matrix<double>(C_ij, block_size, block_size);
	MPI_Send(&(C_ij[0][0]), block_size*block_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    
    if (pid == 0) {
	double end_t = MPI_Wtime() - start_t;
	printf("%d\t%d\t%f\n", N, P, end_t);
	//print_matrix<double>(C, N, N);
    }

    MPI_Finalize();

    return 0;
}
