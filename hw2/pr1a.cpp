#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

#include "mpi.h"
#include "hw2_common.hpp"

using namespace std;

int main(int argc, char* argv[]) {

    int id;
    int num_cpus;

    MPI::Init(argc, argv);
    num_cpus = MPI::COMM_WORLD.Get_size();
    id = MPI::COMM_WORLD.Get_rank();
    double start_t = MPI_Wtime();
    if (num_cpus < atoi(argv[1])) {
        cout << "FATAL: Did not find number of requested CPUs" << endl;
        cout << "Found " << num_cpus << " processors, " << atoi(argv[1]) << " requested." << endl;
        exit(1);
    }

    int N = atoi(argv[2]);
    int pad = N % num_cpus;
    int buffer_size = (N+pad) / num_cpus;
    double * buffer = new double[buffer_size];
    
    MPI_Status recv_stat, send_stat;
    MPI_Request send_req[num_cpus-1];

    if (id == 0) {
        double* V = ones_vector<double>(N, pad);
        for (int pid=1; pid<num_cpus; ++pid) {
            MPI_Isend(V+pid*buffer_size, buffer_size, MPI_DOUBLE, pid, 0, MPI_COMM_WORLD, &send_req[pid-1]);
        }

        double local_sum = 0.0;
        for(int i=0; i<buffer_size; i++) {
            local_sum += V[i];
        }

        double remote_sum = 0;
        for (int pid=1; pid<num_cpus; ++pid) {
	    MPI_Wait(&send_req[pid-1], &send_stat);
            MPI_Recv(&remote_sum, 1, MPI_DOUBLE, pid, 0, MPI_COMM_WORLD, &recv_stat);
            local_sum += remote_sum;
        }
        double end_t = MPI_Wtime() - start_t;
        if (local_sum == (double) N) {
            printf("%d\t%d\t%f\n", N, num_cpus, end_t);
        }
        else {
            printf("ERROR exp=%d act=%d\n", double(N), local_sum);
        }
    }
    else {
        MPI_Recv(buffer, buffer_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &recv_stat);
	double local_sum = 0;
        for (int i=0; i<buffer_size; ++i) {
            local_sum += buffer[i];
        }
        MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
