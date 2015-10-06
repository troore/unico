#ifndef __DISJOINTSET_H_
#define __DISJOINTSET_H_

class DisjointSet {
	int num_atomic;
	int *p;
	double *size;
public:
	DisjointSet();
	~DisjointSet();
	void make_set(int x, double lop);
	int find_set(int x);
	void union_set(int x, int y);
	void link(int x, int y);
	double get_min_set_size();
	int get_min_set_size_id();
	double get_max_set_size();
	int get_max_set_size_id();
	int get_neb_set_id(int id)
};

#endif
