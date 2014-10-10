all: pr1a pr1b pr1c naive_mult naive_blocked_mult parallel_mult

pr1a: pr1a.cpp
	mpic++ $< -o $@

pr1b: pr1b.cpp
	mpic++ $< -o $@

pr1c: pr1c.cpp
	mpic++ $< -o $@

naive_mult: naive_mult.cpp hw2_common.hpp
	g++ $< -o $@ -lrt

naive_blocked_mult: naive_blocked_mult.cpp hw2_common.hpp
	g++ $< -o $@ -lrt

parallel_mult: parallel_mult.cpp
	mpic++ parallel_mult.cpp -o parallel_mult

clean:
	rm -f pr1a pr1b pr1c naive_mult naive_blocked_mult parallel_mult
