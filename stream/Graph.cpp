
#include "Graph.h"

Edge::Edge()
{
}

int Edge::get_next()
{
	return next;
}

int Edge::get_to()
{
	return to;
}

int Edge::get_len()
{
	return len;
}

void Edge::set_next(int n)
{
	next = n;
}

void Edge::set_to(int n)
{
	to = n;
}

void Edge::set_len(int n)
{
	len = n;
}

Edge::~Edge()
{
}


Graph::Graph(int n, int m):n(n), m(m)
{
	cnt = 0;
	head_node = new int[n];
	edge = new int[m];
}

void Graph::add_edge(int u, int v, int w)
{
	edge[cnt].to = v;
	edge[cnt].len = w;
	edge[cnt].next = head_node[u];
	head_node[u] = cnt++;

	edge[cnt].to = u;
	edge[cnt].len = 0;
	edge[cnt].next = head_node[v];
	head_node[v] = cnt++;
}

Graph::~Graph()
{
	delete[] head;
	delete[] edge;
}
