#ifndef __DINIC_H_
#define __DINIC_H_

#define WHITE 0
#define GRAY 1
#define BLACK 2

class Dinic {
	int s, t;
	int n, m;
//	int S[N], T[N];
//	int cnts, cntt;
	Graph *wg;

//	int a[N], b[N], aa[M], bb[M], n, m;

	int head, tail;
	int *q;
	int *color;
	int *layer;
public:
	Dinic(int n, int m);
	~Dinic();
	int min(int a, int b);
	void enqueue(int x);
	int dequeue();
//	void add_edge(int u, int v, int w);
	void cst_graph();
	int bfs();
	void dfs(int u);
	void show();
	int max_flow(int *S, int *T, int &cnts, int &cntt);
};

#endif
