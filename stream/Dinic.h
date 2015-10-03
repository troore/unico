#ifndef __DINIC_H_
#define __DINIC_H_

#define N 201000
#define M  2001000
#define INF 100000000

#define WHITE 0
#define GRAY 1
#define BLACK 2

class Edge {
	int next;
	int to;
	int len;
public:
	int get_next() { return next; }
	int get_to() { return to; }
	int get_len() { return len; }
};

class Dinic {
	Edge edge[M];
	int Head[N];
	int s, t, cnt;
	int S[N], T[N];
	int cnts, cntt;

	int a[N], b[N], aa[M], bb[M], n, m;

	int head, tail;
	int q[N + 2];
	int color[N];
	int layer[N];
public:
	int min(int a, int b);
	void enqueue(int x);
	int dequeue();
	void add_edge(int u, int v, int w);
	void read();
	int bfs();
	void dfs(int u);
	void show();
	int max_flow();
};

#endif
