#include <cstdlib>
#include <iostream>
#include "hw2_common.hpp"

using namespace std;

int main(int argc, char ** argv) {
    
    int N = atoi(argv[1]);
   
    double ** A = ones_matrix<double>(N, N, 0, 0);
    double ** B = ones_matrix<double>(N, N, 0, 0);
    double ** C = naive_mat_mult(A, B, N);
    print_matrix<double>(C, N, N);
}
