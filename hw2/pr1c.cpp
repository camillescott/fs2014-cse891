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

    long long * V;

    if (id == 0) {
        printf("Num processors: %d\n", num_cpus);
	// Generate padded vector
        V = ones_vector(N, pad);
    }
    
    // Local buffer for scatter
    long long * buffer = new long long [buffer_size];
    MPI_Scatter(V, buffer_size, MPI_LONG_LONG_INT, 
		buffer, buffer_size, MPI_LONG_LONG_INT, 0, MPI_COMM_WORLD);

    long long local_sum = vector_sum(buffer, buffer_size);
    long long 


	printf("Start Reduce on CPU %d\n", id);
	MPI_Reduce(V, &my_sum, num_cpus, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        double end_t = MPI_Wtime() - start_t;

        long long exp_sum = 0;
        for (int i=0; i<N; ++i) {
            exp_sum += V[i];
        }
        if (my_sum == exp_sum) {
            printf("%d\t%d\t%f\n", N, num_cpus, end_t);
        }
        else {
            printf("ERROR exp=%d act=%d\n", exp_sum, my_sum);
        }
 	
    }

    MPI_Finalize();

    return 0;
}
