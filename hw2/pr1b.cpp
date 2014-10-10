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
    double * buffer;
    MPI_Status stat;
    if (id == 0) {

        double * V = ones_vector<double>(N, pad);
	for (int pid=1; pid<num_cpus; ++pid) {
	    MPI_Send(V+pid*buffer_size, buffer_size, MPI_DOUBLE, pid, 0, MPI_COMM_WORLD);
	}
	buffer = V;
    } else {
	buffer = new double[buffer_size];	
	MPI_Recv(buffer, buffer_size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &stat);
    }

    double local_sum = vector_sum<double>(buffer, buffer_size);
    //printf("CPU %d SUM %f\n", id, local_sum);
    int m = (int) log2(num_cpus);
    for (int i=0; i<m; ++i) {
	if (id % int(exp2(i+1)) == 0) {
	    double other_sum;
	    MPI_Recv(&other_sum, 1, MPI_DOUBLE, id+int(exp2(i)), 0, MPI_COMM_WORLD, &stat);
	    local_sum += other_sum;
	} else if (id % int(exp2(i+1)) == int(exp2(i))) {
	    MPI_Send(&local_sum, 1, MPI_DOUBLE, id-int(exp2(i)), 0, MPI_COMM_WORLD);
	}
    }
    
    if (id == 0) {
	double end_t = MPI_Wtime() - start_t;
        if (local_sum == (double) N) {
            printf("%d\t%d\t%f\n", N, num_cpus, end_t);
        }
        else {
            printf("ERROR exp=%f act=%f\n", (double) N, local_sum);
        }
 	
    }

    MPI_Finalize();

    return 0;
}
