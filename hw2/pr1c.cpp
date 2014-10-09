#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>

#include "hw2_common.hpp"
#include "mpi.h"

using namespace std;

int main(int argc, char* argv[]) {

    int id;
    int num_cpus;

    MPI::Init(argc, argv);
    num_cpus = MPI::COMM_WORLD.Get_size();
    id = MPI::COMM_WORLD.Get_rank();

    if (num_cpus < atoi(argv[1])) {
        cout << "FATAL: Did not find number of requested CPUs" << endl;
        cout << "Found " << num_cpus << " processors, " << atoi(argv[1]) << " requested." << endl;
        exit(1);
    }

    int N = atoi(argv[2]);
    int pad = N % num_cpus;
    int buffer_size = N / num_cpus;

    double * V;

    if (id == 0) {
        printf("Num processors: %d\n", num_cpus);
	    // Generate padded vector
        V = ones_vector<double>(N, pad);
    }
    
    // Local buffer for scatter
    double * buffer = new long long [buffer_size];
    MPI_Scatter(V, buffer_size, MPI_DOUBLE, 
		buffer, buffer_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    double local_sum = vector_sum(buffer, buffer_size);
    double global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 

    double end_t = MPI_Wtime() - start_t;

    if (id == 0) {
        if (global_sum == (double) N) {
            printf("%d\t%d\t%f\n", N, num_cpus, end_t);
        }
        else {
            printf("ERROR exp=%d act=%d\n", exp_sum, my_sum);
        }
    }

    MPI_Finalize();

    return 0;
}
