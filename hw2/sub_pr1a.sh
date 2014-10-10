#!/bin/sh -login
#PBS -l walltime=00:05:00,nodes=64:ppn=4
#PBS -N cse891_pr1

NS=10
export OMP_NUM_THREADS=256

cd $PBS_O_WORKDIR
make clean; make

rm -f pr1a_results.txt

for np in 1 4 16 64 256;
do
    for N in 10 100 5000;
    do
	if [ "$np" -lt "$N" ]
	then
	    #echo PARAMS $np $N
	    i=0
	    while [ "$i" -lt "$NS" ]
	    do
		mpirun -np $np ./pr1a $np $N >> pr1a_results.txt
		i=$[$i+1]
	    done
	fi
    done
done
