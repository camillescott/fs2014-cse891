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

template<typename T>
void print_matrix(T ** A, int N, int M) {
    for (int i=0; i<N; ++i) {
	for (int j=0; j<M; ++j) {
	    cout << A[i][j] << " ";
	}
	cout << endl;
    }   
}

// N: rows
// M: columns
template<typename T>
T ** ones_matrix(int N, int M, int padN, int padM) {
    T ** A = new T* [N+padN];
    for (int i=0; i<N+padN; ++i) {
	if (i < N) A[i] = ones_vector<T>(M, padM);
	else {
	    A[i] = new T[M+padM];
	    for (int j=0; j<M+padM; ++j) {
		A[i][j] = (T) 0;
	    }
	}
    }
    return A;
}

/*
 * Alloc Matrix in contiguous memory for easy sends
 * Code modified from 
 * http://stackoverflow.com/questions/5901476/sending-and-receiving-2d-array-over-mpi
 */
template<typename T>
T ** alloc_matrix(int N, int M, T fill) {
    T * data = (T *) malloc(N * M * sizeof(T));
    T ** A = (T **) malloc(N * sizeof(T));
    for (int i=0; i<N; ++i) {
	A[i] = &(data[M*i]);
	for (int j=0; j<M; ++j) {
	    A[i][j] = fill;
	}
    }
    return A;
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

template<typename T>
T ** naive_mat_mult(T** A, T** B, int N) {
    // Allocate result matrix
    T ** C = new T*[N];
    for (int i=0; i<N; ++i) C[i] = new T[N];

    for (int i=0; i<N; ++i) {
	for (int j=0; j<N; ++j) {
	    for (int k=0; k<N; ++k) {
		C[i][j] += A[i][k] * B[k][j];
	    }
	}
    }
    return C;
}

template<typename T>
T ** naive_blocked_mat_mult(T** A, T** B, int N, int block_size) {
    // Allocate result matrix
    T ** C = alloc_matrix<double>(N, N, 0.0);

    for (int bi=0; bi<N/block_size; ++bi) {
	int block_row = bi * block_size;
	for (int bj=0; bj<N/block_size; ++bj) {
	    int block_column = bj * block_size;
	    //for (int bk=0; bk<N/block_size; ++bk) {
		for (int i=block_row; i<block_row+block_size; ++i) {
		    for (int j=block_column; j<block_column+block_size; ++j) {
			for (int bk=0; bk<N/block_size; ++bk) {
			int k_start = bk * block_size;
			//cout << "Fill [" << i << "," << j << "]\n" << endl;
			for (int k=k_start; k<k_start+block_size; ++k) {
			    C[i][j] += A[i][k] * B[k][j];
			}
		    }
		}
	    }
	}
    }
    return C;
}

template<typename T>
void mat_inc_sum(T** A, T** B, int N) {
    for (int i=0; i<N; ++i) {
        for (int j=0; j<N, ++j) {
            C[i][j] += B[i][j];
        }
    }
}

timespec diff(timespec start, timespec end)
{
    timespec temp;
    if ((end.tv_nsec-start.tv_nsec)<0) {
	temp.tv_sec = end.tv_sec-start.tv_sec-1;
	temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
    } else {
	temp.tv_sec = end.tv_sec-start.tv_sec;
	temp.tv_nsec = end.tv_nsec-start.tv_nsec;
    }
    return temp;
}









