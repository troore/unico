
#include "Dinic.h"
#include "Graph.h"

Dinic::Dinic(int n, int m):n(n), m(m)
{
	q = new int[n];
	color = new int[n];
	layer = new int[n];
}

int Dinic::min(int a, int b)
{
	return (a < b) ? a : b;
}

void Dinic::enqueue(int x)
{
    q[tail] = x;
    tail++;
    color[x] = GRAY;
}

int Dinic::dequeue()
{
    int x = q[head];
    head++;
    color[x] = BLACK;
	
    return x;
}

/*
 * Construct graph for max-flow min-cut
 */
void Dinic::cst_graph(double *a, double *b,
					  double *aa, double *bb,
					  double *ws, double *wt)
{
    wg = new Graph(n, m);
	
    for (int i = 1; i <= m; i++) {
		//    scanf("%d %d %d", &aa[i], &bb[i], &w);
        wg->add_edge(aa[i], bb[i], ws[i]);
		wg->add_edge(bb[i], aa[i], wt[i]);
    }

    s = 0; t = n + 1;
    for (int i = 1; i <= n; i++) {
		wg->add_edge(s, i, b[i]);
		wg->add_edge(i, t, a[i]);
	}
}

int Dinic::bfs()
{
	int u, v;
	int *head_node;
	Edge *edge;

	head_node = wg->head_node;
	edge = wg->edge;

	head = tail = 1;
	
	for (int i = 0; i <= n + 1; i++) {
		layer[i] = -1;
		color[i] = WHITE;
	}
	
    enqueue(s);
	layer[s] = 0;
    while (head != tail) {
        u = dequeue();
        for (i = head_node[u]; i != -1; i = edge[i].next) {
			v = edge[i].to;
            if (edge[i].len > 0 && color[v] == WHITE) {
                layer[v] = layer[u] + 1;
                enqueue(v);
            }
		}
    }
	
    return color[t] == BLACK;
}

int Dinic::find(int u, int cur_residual_flow)
{
    int result = 0, tmp = 0;

	int *head_node;
	Edge *edge;

	head_node = wg->head_node;
	edge = wg->edge;

	if (u == t)
		return cur_residual_flow;
	
    for (int i = head_node[u]; i != -1 && result < cur_residual_flow; i = edge[i].next) {
		int v = edge[i].to;
        if (edge[i].len > 0 && layer[v] == layer[u] + 1) {
            tmp = find(v, min(edge[i].len, cur_residual_flow - result));
            edge[i].len -= tmp;
			edge[i ^ 1].len += tmp;
			result += tmp;
        }
	}
    if (!result)
		layer[u] = -1;
	
    return result; 
}

void Dinic::dfs(int u)
{
	int *head_node;
	Edge *edge;

	head_node = wg->head_node;
	edge = wg->edge;

	color[u] = GRAY;
	for (int i = head_node[u]; i != -1; i = edge[i].next)
		if(edge[i].len > 0 && color[edge[i].to] == WHITE) {
			dfs(edge[i].to);
		}

	color[u] = BLACK;
}

void Dinic::show()
{  
    printf("S %d:\n", cnts);
    for (int i = 0; i < cnts; i++) {  
		printf("%d", S[i]);
		printf(i < cnts - 1 ? " " : "\n");
	}
	printf("T %d:\n", cnts);
	for (int i = 0; i < cntt; i++) { 
		printf("%d ", T[i]);
		printf(i < cntt - 1 ? " " : "\n");
	}
}  

int Dinic::max_flow(int *S, int *T, int &cnts, int &cntt)
{
    int ans = 0;
	
    while (bfs())
		ans += find(s, INF);

	for (int i = 0; i <= n + 1; i++)
		color[i] = WHITE;

	cnts = cntt = 0;
	dfs(0);
	
    for (int i = 0; i <= n + 1; i++)  
    {  
        if (color[i] == BLACK)  
            S[cnts++] = i;  
        else  
            T[cntt++] = i;  
    }
	
    return ans;
}

Dinic::~Dinic()
{
	delete[] q;
	delete[] color;
	delete[] layer;

	if (wg)
		delete wg;
}
