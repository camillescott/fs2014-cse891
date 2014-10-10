#include <cstdlib>
#include <iostream>
#include <time.h>
#include "hw2_common.hpp"

using namespace std;

int main(int argc, char ** argv) {
    
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int N = atoi(argv[1]);
    int block_size = atoi(argv[2]);
    int pad = N % block_size;
   
    double ** A = ones_matrix<double>(N, N, pad, pad);
    double ** B = ones_matrix<double>(N, N, pad, pad);
    double ** C = naive_blocked_mat_mult(A, B, N+pad, block_size);
    
    timespec endts;
    clock_gettime(CLOCK_MONOTONIC, &endts);
    cout << N << "\t" << diff(ts,endts).tv_sec + (float) diff(ts,endts).tv_nsec / 1000000000.0 << endl;
    return 0;
}
