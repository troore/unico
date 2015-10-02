

#include <stdio.h>
#include <stdlib.h>

#define N 30001
//#define M 501

int p[N];
//int rank[N];
int size[N]; // set size

void make_set(int x)
{
	p[x] = x;
//	rank[x] = 1;
	size[x] = 1;
}

int find_set(int x)
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

void link(int x, int y)
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

void union_set(int x, int y)
{
	link(find_set(x), find_set(y));
}

int main(int argc, char *argv[])
{
	int i, j;
	int n, m, s;
	int x, y;

	while (scanf("%d %d", &n, &m)) {
		if (n == 0 && m == 0)
			break;
		for (i = 0; i < n; i++)
			make_set(i);
		for (i = 0; i < m; i++) {
			scanf("%d", &s);
			scanf("%d", &x);
			for (j = 1; j < s; j++) {
				scanf("%d", &y);
				union_set(x, y);
			}
		}
		printf("%d\n", size[0]);
	}

	return 0;
}
