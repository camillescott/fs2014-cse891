all: count-sort-inner count-sort-outer std-qsort std-qsort-phi count-sort-phi

html: Homework_3.ipynb
	nbconvert Homework_3.ipynb --to slides

count-sort-inner: count-sort.c
	icc -lrt -O3 -openmp -DLOOP=1 -o $@ $<

count-sort-outer: count-sort.c
	icc -lrt -O3 -openmp -o $@ $<

count-sort-phi: count-sort.c
	icc -lrt -O3 -openmp -DMIC=1 -o $@ $<

std-qsort: std-qsort.cpp
	icpc -lrt -O3 -o $@ $<

std-qsort-phi: std-qsort.cpp
	icpc -lrt -O3 -DMIC=1 -o $@ $<	

clean:
	rm -f count-sort-mic count-sort-inner count-sort-outer std-qsort

