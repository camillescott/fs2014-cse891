---
title: Final Project Abstract
subtitle: for CSE891, FS2014
author: 
- name: Camille Welcher
  affiliation: Michigan State University
  email: welcherc@msu.edu
date: October 20, 2014
...

The overall goal of my final project will be to add parallel graph construction and querying to khmer[^1], a k-mer counting package built and maintained in my home lab, the Lab for Genomics, Evolution, and Development [^2]. This software is used to count fixed-length subsequences of a longer DNA or RNA sequencing read (k-mers), with cardinalities reaching into the tens of billions. To handle this amount of information in-memory, we use a bloom filter to store the existence of a hashed k-mer in fixed memory with a known false positive rate. Performance analysis has shown that the most significant bottleneck is in the hashing of k-mers; however, at this time, we have not implemented a workable multiprocessing solution. Given that the vast majority of use-cases for this software are on multicore and HPC systems, parallel hashing would greatly improve performance of analyses for many thousands of users.

Currently, our k-mer counting implementation is unique in that it operates completely in-memory, and streaming reading and writing is a primary design feature; this is a constraint that will be maintained within the parallelization effort. My first-order goal is to simply enable parallel read-write on a single node with OpenMP. Challenges here include developing an appropriate locking scheme (is it even necessary to acquire locks when our data structure is inherently error-prone?) and balancing hashing performance with I/O limitations. My second-order goal is to implement distributed-memory storage along with parallel hashing. Questions to address here include how best to subdivide the data structure while maintaining hash consistency and maximising k-mer locality within reads. We would prefer to subdivide the datastructure across nodes in such a way that all the k-mers for a given read are on a single node, but avoid overloading any particular node with k-mers and thus increasing its number of false-positives.

Evaluation of single-node hashing performance is somewhat trivial; I would compare the seqential performance to parallel performance on a variety of systems (single-user, Amazon EC2, and MSU HPC) and aim to achieve near-linear scaling. Performance evaluation of the distributed-memory implementation would be more difficult, and best done on extremely large datasets. Currently, my lab maintains a number of large metagenomics datasets that routinely require multiple terabytes of memory to process, and I would make use of these for evaluation. This would include looking at scaling with number of nodes and comparison to single-node performance, which can be done by utilizing some of MSU's big-memory systems.

I plan on tackling this project on my own, and would be responsible for all parts of it. As I have worked with the software in question for multiple years and have considerable familiarity with its internals, I feel confident in my ability to tackle the project on my own.

[^1]: https://github.com/ged-lab/khmer
[^2]: http://ged.msu.edu/
