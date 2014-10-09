#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

template<typename T>
T * range_vector(int N, int pad) {
    T * V = new T[N];
    for (int i=0; i<N; ++i) {
        V[i] = (T) i;
    }
    for (int i=0; i<pad; ++i) {
        V[N+i] = (T) i;
    }
    return V;
}

template<typename T>
T * ones_vector(int N, int pad) {
    T * V = new T[N + pad];
    for (int i=0; i<N; ++i) {
	V[i] = (T) 1;
    }
    for (int i=0; i<pad; ++i) {
	    V[N+i] = (T) 0;
    }
    return V;
}

template<typename T>
T * random_vector(int N, int pad) {
    T * V = new T[N + pad];
    srand(time(NULL));
    for (int i=0; i<N; ++i) {
	    V[i] = (T) rand() % 10;
    }
    for (int i=0; i<pad; ++i) {
	    V[N+i] = (T) 0;
    }
    return V;
}

template<typename T>
void print_vector(T * V, int N) {
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

template<typename T>
T vector_sum(T * V, int N) {
    T sum = 0;
    for (int i=0; i<N; ++i) {
	sum += V[i];
    }
    return sum;
}
