all: count-sort-inner count-sort-outer std-qsort

count-sort-inner: count-sort.cpp
	icpc -lrt -O3 -openmp -DLOOP=1 -o $@ $<

count-sort-outer: count-sort.cpp
	icpc -lrt -O3 -openmp -o $@ $<

std-qsort: std-qsort.cpp
	icpc -lrt -O3 -o $@ $<

clean:
	rm -f count-sort-mic count-sort-inner count-sort-outer std-qsort

