#include <cstdlib>
#include <iostream>
#include <time.h>
#include "hw2_common.hpp"
#include <cblas.h>

using namespace std;

int main(int argc, char ** argv) {
    
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int N = atoi(argv[1]);
   
    double ** A = alloc_matrix<double>(N, N, 1.0);
    double ** B = alloc_matrix<double>(N, N, 1.0);
    double ** C = alloc_matrix<double>(N, N, 0.0);

    double alpha = 1.0;
    double beta = 1.0;

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, alpha, &(A[0][0]), N, &(B[0][0]), N, beta, &(C[0][0]), N);
 
    timespec endts;
    clock_gettime(CLOCK_MONOTONIC, &endts);
    cout << N << "\t" << diff(ts,endts).tv_sec + (float) diff(ts,endts).tv_nsec / 1000000000.0 << endl;
    return 0;
}
