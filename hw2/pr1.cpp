# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <cmath>
# include <ctime>

# include "mpi.h"

using namespace std;

long long* generate_vector(int N) {
    long long* V = new long long[N];
    for (int i=0; i<N; ++i) {
        V[i] = i;
    }
    return V;
}

void print_vector(long long* V, int N) {
    cout << "Vector [ ";
    for (int i=0; i<N; ++i) {
        cout << V[i] << " ";
    }
    cout << "]" << endl;
}

int get_buffer_size(int N, int pid, int num_cpus) {
    int buffer_size = N / num_cpus;
    if (pid == num_cpus-1) {
        return buffer_size + (N % buffer_size);
    }
    return buffer_size;
}

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
    int buffer_size = get_buffer_size(N, id, num_cpus);
    long long buffer[buffer_size];
    //printf("CPU %d buffer size: %d\n", id, buffer_size);
    //printf("CPU %d N: %d\n", id, N);
    MPI_Status stat;

    if (id == 0) {
        //printf("Num processors: %d\n", num_cpus);

        long long* V = generate_vector(N);
        long long exp_sum = 0;
        for (int i=0; i<N; ++i) {
            exp_sum += V[i];
        }
        /*
        printf("Vector N: %d\n", N);
        printf("Expected result: %lld\n", exp_sum);
        print_vector(V+N-10, 10);
        */
        double start_t = MPI_Wtime();
        for (int pid=1; pid<num_cpus; ++pid) {
            int this_bsize = get_buffer_size(N, pid, num_cpus);
            //print_vector(V+pid*buffer_size, this_bsize);
            MPI_Send(V+pid*buffer_size, this_bsize, MPI_LONG_LONG_INT, pid, 0, MPI_COMM_WORLD);
        }

        long long local_sum = 0;
        for(int i=0; i<buffer_size; i++) {
            local_sum += V[i];
        }

        long long remote_sum = 0;
        for (int pid=1; pid<num_cpus; ++pid) {
            MPI_Recv(&remote_sum, 1, MPI_LONG_LONG_INT, pid, 0, MPI_COMM_WORLD, &stat);
            //printf("Recv %lld from CPU %d\n", remote_sum, pid);
            local_sum += remote_sum;
        }
        double end_t = MPI_Wtime() - start_t;
        if (local_sum == exp_sum) {
            printf("%d\t%d\t%f\n", N, num_cpus, end_t);
        }
        else {
            printf("-1\t-1\t-1\n");
        }
        //cout << "Finished computation! Result: " << local_sum << endl;
    }
    else {
        MPI_Recv(buffer, buffer_size, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD, &stat);
        //cout << "CPU " << id << " got buffer starting at " << buffer[0] 
        //    << " and ending at " << buffer[buffer_size-1] << endl;
        //print_vector(buffer, 10);
        //print_vector(buffer+buffer_size-10, 10);
        long long local_sum = 0;
        for (int i=0; i<buffer_size; ++i) {
            local_sum += buffer[i];
        }
        //printf("Send %lld from CPU %d\n", local_sum, id);
        MPI_Send(&local_sum, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
