all: count-sort-inner count-sort-outer std-qsort

count-sort-inner: count-sort.cpp
	g++ -DLOOP=1 -o $@ -fopenmp $<

count-sort-outer: count-sort.cpp
	g++ -o $@ -fopenmp $<

std-qsort: std-qsort.cpp
	g++ -o $@ $<

