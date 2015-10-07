
#include "DisjointSet.h"

DisjointSet::DisjointSet(int n, Task *pt)
	: num_elems(n), task_chain(pt)
{
	p = new int[n];
	set_latency = new double[n];
	set_power = new double[n];
	set_lop = new double[n];
}

void DisjointSet::build_disjset()
{
	for (int i = 0; i < num_elems; i++) {
		make_set(i);
	}
}

void DisjointSet::make_set(int x)
{
	p[x] = x;
	set_size[x] = 1;
	set_latency[x] = task_chain[x]->latency;
	set_power[x] = task_chain[x]->power;
	set_lop[x] = task_chain[x]->lop;
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
		set_size[x] = set_size[x] + set_size[y];
		set_size[y] = 0;
		set_latency[x] = set_latency[x] + set_latency[y];
		set_latency[y] = 0.0;
		set_power[x] = (set_latency[x] * set_power[x] + set_latency[y] * set_power[y]) / set_size[x];
		set_power[y] = 0.0;
		set_lop[x] = set_latency[x] / set_power[x];
		set_lop[y] = 0.0;
	}
	else if (y < x){
		p[x] = y;
		set_size[y] = set_size[y] + set_size[x];
		set_size[y] = 0;
		set_latency[y] = set_latency[y] + set_latency[x];
		set_latency[x] = 0.0;
		set_power[y] = (set_latency[y] * set_power[y] + set_latency[x] * set_power[x]) / set_size[y];
		set_power[x] = 0.0;
		set_lop[y] = set_latency[y] / set_power[y];
		set_lop[x] = 0.0;
		/*
		if (rank[x] == rank[y])
			rank[y] = rank[y] + 1;
		*/
	}
	else {
		// We never link a set with itself.
		;
	}
}

void DisjointSet::union_set(int x, int y)
{
	link(find_set(x), find_set(y));
}

double DisjointSet::get_min_set_latency(int x)
{
	return set_latency[find_set(x)];
}

double DisjointSet::min_set_size()
{
	double tmp = -1.0;
	
	for (int i = 0; i < num_elems; i++) {
		if (tmp == -1.0 || (tmp - size[i]) > 0.00001)
			tmp = size[i];
	}

	return res;
}

int DisjointSet::min_set_size_id()
{
	double tmp = -1.0;
	int id;
	
	for (int i = 0; i < num_elems; i++) {
		if (res == -1.0 || (res - size[i]) > 0.00001) {
			res = size[i];
			id = i;
		}
	}

	return id;
}

double DisjointSet::max_set_size()
{
	double tmp = 0.0;
	
	for (int i = 0; i < num_elems; i++) {
		if ((size[i] - tmp) > 0.00001)
			tmp = size[i];
	}

	return res;
}

int DisjointSet::max_set_size_id()
{
	double tmp = 0.0;
	int id;
	
	for (int i = 0; i < num_elems; i++) {
		if ((size[i] - res) > 0.00001) {
			res = size[i];
			id = i;
		}
	}

	return id;
}

int DisjointSet::neb_set_id(int id)
{
	int i, j;
	double down, up;

	if (id >= (num_elems - 1)) {
		for (i = 0; i < id; i++) {
			if (size[i] != 0) {
				down = size[i]
					break;
			}
		}

		return i;
	}
	else if {
		for (j = id + 1; j < num_elems; j++) {
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
		for (j = id + 1; j < num_elems; j++) {
			if (size[j] != 0) {
				up = size[j];
				break;
			}
		}

		return (down - up < 0.00001) ? i : j;
	}
}

double DisjointSet::system_power_consumption()
{
	double res = 0.0;

	for (int i = 0; i < num_elems; i++) {
		res = res + set_power[i];
	}

	return res;
}

DisjointSet::~DisjointSet()
{
	delete[] p;
	delete[] set_size;
	delete[] set_latency;
	delete[] set_power;
	delete[] set_lop;
}
