#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <omp.h>

#ifndef LOOP
#define LOOP 0
#endif

using namespace std;

void omp_count_sort(int a[], int n, int P);

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

int main(int argc, char ** argv) {

    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);    

    int N = atoi(argv[1]);
    int * A = new int[N];
    int P = atoi(argv[2]);

    srand(time(NULL));
    for (int i=0; i<N; ++i) {
        A[i] = (int) rand();
    }
    omp_count_sort(A, N, P);
    for (int i=1; i<N; ++i) {
        if (A[i] < A[i-1]) {
            cout << "ERROR: Sort failed" << endl;
            cout << A[i-1] << " " << A[i] << endl;
            exit(1);
        }
    }

    timespec endts;
    clock_gettime(CLOCK_MONOTONIC, &endts);
    cout << N << "\t" << P << "\t" << diff(ts,endts).tv_sec + (float) diff(ts,endts).tv_nsec / 1000000000.0 << endl;

}

void omp_count_sort(int a[], int n, int P) {

    int i, j;
    int count = 0;
    int *temp = new int[n];
    omp_set_num_threads(P);

    #pragma offload target(mic)
    #pragma omp parallel for if (LOOP==0) private(count)
    for (i = 0; i < n; i++) { 
        count = 0;
        #pragma omp parallel for if (LOOP==1) shared(count)
        for (j = 0; j < n; j++)
            if (a[j] < a[i])
                __sync_add_and_fetch(&count, 1);
            else if (a[j] == a[i] && j < i)
                __sync_add_and_fetch(&count, 1);
        temp[count] = a[i];
    }
    #pragma omp parallel for
    for (int k = 0; k < n; ++k)
        a[k] = temp[k];
    //memcpy(&a[k], &temp[k], sizeof(int));
    free(temp);
} /* Count sort */
