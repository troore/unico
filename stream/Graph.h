#ifndef __GRAPH_H_
#define __GRAPH_H_

class Edge {
	int next;
	int to;
	int len;
public:
	int get_next();
	int get_to();
	int get_len();

	void set_next();
	void set_to();
	void set_len();
};

class Graph {
	int *head_node;
	Edge *edge;
	int n; // number of vertices
	int m; // upper bound of number of edges
	int cnt; // counter of current number of edges
public:
	Graph(int n, int m);
	~Graph();
	void add_edge(int u, int v, int w);
};

#endif
