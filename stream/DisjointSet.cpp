
#include "DisjointSet.h"

DisjointSet::DisjointSet(int n)
	: num_atomic(n)
{
	p = new int[n];
	size = new double[n];
}

void DisjointSet::make_set(int x, double lop)
{
	p[x] = x;
//	rank[x] = 1;
	size[x] = lop;
}

int DisjointSet::find_set(int x)
{
	/*
	if (p[x] != x)
		p[x] = find_set(p[x]);

	return p[x];
	*/

	int root = x;
	int tmp;
	
	while (p[root] != root) {
		root = p[root];
	}
	while (p[x] != x) {
		tmp = p[x];
		p[x] = root;
		x = tmp;
	}

	return p[x];
}

void DisjointSet::link(int x, int y)
{
	if (x < y) {
		p[y] = x;
		size[x] = size[x] + size[y];
		size[y] = 0;
	}
	else if (y < x){
		p[x] = y;
		size[y] = size[y] + size[x];
		size[x] = 0;
		/*
		if (rank[x] == rank[y])
			rank[y] = rank[y] + 1;
		*/
	}
	else
		;
}

void DisjointSet::union_set(int x, int y)
{
	link(find_set(x), find_set(y));
}

double DisjointSet::get_min_set_size()
{
	double tmp = -1.0;
	
	for (int i = 0; i < num_atomic; i++) {
		if (tmp == -1.0 || (tmp - size[i]) > 0.00001)
			tmp = size[i];
	}

	return res;
}

int DisjointSet::get_min_set_size_id()
{
	double tmp = -1.0;
	int id;
	
	for (int i = 0; i < num_atomic; i++) {
		if (res == -1.0 || (res - size[i]) > 0.00001) {
			res = size[i];
			id = i;
		}
	}

	return id;
}

double DisjointSet::get_max_set_size()
{
	double tmp = 0.0;
	
	for (int i = 0; i < num_atomic; i++) {
		if ((size[i] - tmp) > 0.00001)
			tmp = size[i];
	}

	return res;
}

int DisjointSet::get_max_set_size_id()
{
	double tmp = 0.0;
	int id;
	
	for (int i = 0; i < num_atomic; i++) {
		if ((size[i] - res) > 0.00001) {
			res = size[i];
			id = i;
		}
	}

	return id;
}

int DisjointSet::get_neb_set_id(int id)
{
	int i, j;
	double down, up;

	if (id >= (num_atomic - 1)) {
		for (i = 0; i < id; i++) {
			if (size[i] != 0) {
				down = size[i]
					break;
			}
		}

		return i;
	}
	else if {
		for (j = id + 1; j < num_atomic; j++) {
			if (size[j] != 0) {
				up = size[j];
				break;
			}
		}

		return j;
	}
	else {
		for (i = 0; i < id; i++) {
			if (size[i] != 0) {
				down = size[i]
					break;
			}
		}
		for (j = id + 1; j < num_atomic; j++) {
			if (size[j] != 0) {
				up = size[j];
				break;
			}
		}

		return (down - up < 0.00001) ? i : j;
	}
}

DisjointSet::~DisjointSet()
{
	if (p)
		delete[] p;
	if (size)
		delete[] size;
}
