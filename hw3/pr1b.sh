#!/bin/sh -login
#PBS -l walltime=00:10:00,nodes=1:ppn=20,feature=intel14
#PBS -N cse891_pr1b

NS=10
export OMP_NUM_THREADS=20

cd $PBS_O_WORKDIR
make

for np in 1 2 4 8 12 16 20;
do
    for N in 1000 5000 10000;
    do
	echo "count-sort N=${N} P=${np}"
	i=0
	while [ "$i" -lt "$NS" ]
	do
	    ./count-sort-inner $N ${np} >> count-inner.results.tsv
	    ./count-sort-outer $N ${np} >> count-outer-results.tsv
	    i=$(( $i + 1 ))
	done
    done
done

for N in 1000 5000 10000;
do
    i=0
    while [ "$i" -lt "$NS" ]
    do
	./std-qsort $N >> std-qsort.results.tsv
	i=$(( $i + 1 ))
    done
done

