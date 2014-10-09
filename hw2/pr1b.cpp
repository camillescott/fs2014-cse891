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
    long long * V;
    int seg_size = get_seg_size(N, id, num_cpus);
    long long * buffer = new long long[seg_size];
    MPI_Status stat;
    if (id == 0) {
        //printf("Num processors: %d\n", num_cpus);

        V = ones_vector(N);
	for (int pid=1; pid<num_cpus; ++pid) {
	    //MPI_Request req;
	    MPI_Send(V+pid*seg_size, get_seg_size(N, pid, num_cpus), MPI_LONG_LONG_INT, pid, 0, MPI_COMM_WORLD);
	    //MPI_Request_free(&req);
	}
	buffer = V;
    } else {
	MPI_Recv(buffer, seg_size, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD, &stat);
	//printf("Recv on $d\n", id);
    }

    long long my_sum = 0;
    for (int i=0; i<seg_size; ++i) {
	my_sum += buffer[i];
    }
    //printf("CPU %d sum %lld\n", id, my_sum);
    //printf("CPU %d stat %d\n", id, my_sum==seg_size);

    int m = (int) log2(num_cpus);
    for (int i=0; i<m; ++i) {
	if (id % int(exp2(i+1)) == 0) {
	    long long other_sum;
	    MPI_Recv(&other_sum, 1, MPI_LONG_LONG_INT, id+int(exp2(i)), 0, MPI_COMM_WORLD, &stat);
	    my_sum += other_sum;
	    //printf("Recv %lld on %d [L %d]\n", other_sum, id, i);
	} else if (id % int(exp2(i+1)) == int(exp2(i))) {
	    //printf("Send %lld on %d to %d [L %d]\n", my_sum, id, id-int(exp2(i)), i);
	    MPI_Send(&my_sum, 1, MPI_LONG_LONG_INT, id-int(exp2(i)), 0, MPI_COMM_WORLD);
	}
    }
    
    if (id == 0) {
        long long exp_sum = 0;
        for (int i=0; i<N; ++i) {
            exp_sum += V[i];
        }
        double end_t = MPI_Wtime() - start_t;

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
