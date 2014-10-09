#include <cstdlib>

using namespace std;

long long* range_vector(int N) {
    long long* V = new long long[N];
    for (int i=0; i<N; ++i) {
        V[i] = i;
    }
    return V;
}

long long* ones_vector(int N) {
    long long* V = new long long[N];
    for (int i=0; i<N; ++i) {
	V[i] = 1;
    }
    return V;
}

long long* random_vector(int N) {
    long long* V = new long long[N];
    srand(time(NULL));
    for (int i=0; i<N; ++i) {
	V[i] = rand() % 10;
    }
    return V;
}

void print_vector(long long* V, int N) {
    cout << "Vector [ ";
    for (int i=0; i<N; ++i) {
        cout << V[i] << " " << flush;
    }
    cout << "]" << endl << flush;
}

int get_seg_size(int N, int pid, int num_cpus) {
    int buffer_size = N / num_cpus;
    if (pid == num_cpus-1) {
        return buffer_size + (N % num_cpus);
    }
    return buffer_size;
}
