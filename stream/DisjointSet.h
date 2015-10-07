#ifndef __DISJOINTSET_H_
#define __DISJOINTSET_H_

#include "Task.h"

class DisjointSet {
	int num_elems;
	Task **task_elems;
	int *p;
	int *set_size;
	double *set_latency;
	double *set_power;
	double *set_lop;
public:
	DisjointSet(int n);
	~DisjointSet();
	void make_set(int x, double lop);
	int find_set(int x);
	void union_set(int x, int y);
	void link(int x, int y);
	double min_set_size();
	int min_set_size_id();
	double max_set_size();
	int max_set_size_id();
	int neb_set_id(int id);
	double min_set_latency(int x);
	void update_elem(int x, Task *t);
};

#endif
