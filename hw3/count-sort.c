#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#ifndef LOOP
#define LOOP 0
#endif

#ifndef MIC
#define MIC 0
#endif

//using namespace std;

void omp_count_sort(int a[], int n, int P);

struct timespec diff(struct timespec start, struct timespec end)
{
    struct timespec temp;
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

    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);    

    int N = atoi(argv[1]);
    int * A = (int *) malloc (N * sizeof(int));
    int P = atoi(argv[2]);

    srand(time(NULL));
    int i;
    for (i=0; i<N; ++i) {
        A[i] = (int) rand();
    }
    omp_count_sort(A, N, P);
    for (i=1; i<N; ++i) {
        if (A[i] < A[i-1]) {
	    printf("ERROR: Sort failed\n");
            //cout << "ERROR: Sort failed" << endl;
            //cout << A[i-1] << " " << A[i] << endl;
            exit(1);
        }
    }

    struct timespec endts;
    clock_gettime(CLOCK_MONOTONIC, &endts);
    printf("%d\t%d\t%f\n", N, P, diff(ts,endts).tv_sec + (float) diff(ts,endts).tv_nsec / 1000000000.0);

}

void omp_count_sort(int a[], int n, int P) {

    int * temp;
    #pragma offload target(mic) if (MIC==1) inout(a:length(n)) nocopy(temp:length(n))
    {
    int i, j;
    int count = 0;
    temp = (int *) malloc(n * sizeof(int));
    omp_set_num_threads(P);


    #pragma omp parallel for if (LOOP==0) private(count)
    for (i=0; i < n; i++) { 
        count = 0;
        #pragma omp parallel for if (LOOP==1) shared(count)
        for (j=0; j < n; j++)
            if (a[j] < a[i])
                __sync_add_and_fetch(&count, 1);
            else if (a[j] == a[i] && j < i)
                __sync_add_and_fetch(&count, 1);
        temp[count] = a[i];
    }
    int k = 0;
    #pragma omp parallel for
    for (k=0; k < n; ++k)
        memcpy(&a[k], &temp[k], sizeof(int));
    free(temp);
    }
} /* Count sort */
