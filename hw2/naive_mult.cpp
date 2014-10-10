#include <cstdlib>
#include <iostream>
#include "hw2_common.hpp"

using namespace std;

int main(int argc, char ** argv) {
    
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    int N = atoi(argv[1]);
   
    double ** A = ones_matrix<double>(N, N, 0, 0);
    double ** B = ones_matrix<double>(N, N, 0, 0);
    double ** C = naive_mat_mult(A, B, N);
    
    timespec endts;
    clock_gettime(CLOCK_MONOTONIC, &endts);
    cout << N << "\t" << diff(ts,endts).tv_sec + (float) diff(ts,endts).tv_nsec / 1000000000.0 << endl;
    return 0;
    
}
