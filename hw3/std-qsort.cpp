#include <iostream>
#include <cstdlib>
#include <ctime>

#ifndef MIC
#define MIC 0
#endif

using namespace std;

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

int __attribute__((target(mic))) comp(const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}

int main(int argc, char ** argv) {
    
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); 

    int N = atoi(argv[1]);
    int * A = new int[N];

    srand(time(NULL));
    for (int i=0; i<N; ++i) {
        A[i] = (int) rand();
    }
    #pragma offload target(mic) if (MIC==1) inout(A:length(N))
    qsort(A, N, sizeof(int), comp);

    for (int i=1; i<N; ++i) {
        if (A[i] < A[i-1]) {
            cout << "ERROR: Not properly sorted" << endl;
            exit(1);
        }
    }

    timespec endts;
    clock_gettime(CLOCK_MONOTONIC, &endts);
    cout << N << "\t" << diff(ts,endts).tv_sec + (float) diff(ts,endts).tv_nsec / 1000000000.0 << endl;

}
