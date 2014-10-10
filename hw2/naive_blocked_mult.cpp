#include <cstdlib>
#include <iostream>
#include "hw2_common.hpp"

using namespace std;

int main(int argc, char ** argv) {
    
    int N = atoi(argv[1]);
    int block_size = atoi(argv[2]);
   
    double ** A = ones_matrix<double>(N, N, 0, 0);
    double ** B = ones_matrix<double>(N, N, 0, 0);
    double ** C = naive_blocked_mat_mult(A, B, N, block_size);
    print_matrix<double>(C, N, N);
}
