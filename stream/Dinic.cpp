
#include "Dinic.h"

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

void Dinic::add_edge(int u, int v, int w)
{
	edge[cnt].to = v;
	edge[cnt].len = w;
	edge[cnt].next = Head[u];
	Head[u] = cnt++;

	edge[cnt].to = u;
	edge[cnt].len = 0;
	edge[cnt].next = Head[v];
	Head[v] = cnt++;
}

void Dinic::cst_graph()
{
	int i, w;
	
    memset(Head, -1, sizeof(Head));
    for (i = 1; i <= n; i++)
		scanf("%d %d", &a[i], &b[i]);
    for (i = 1; i <= m; i++) {
        scanf("%d %d %d", &aa[i], &bb[i], &w);
        add_edge(aa[i], bb[i], w);
		add_edge(bb[i], aa[i], w);
    }
    s = 0; t = n + 1;
    for (i = 1; i <= n; i++) {
		add_edge(s, i, a[i]);
		add_edge(i, t, b[i]);
	}
}

int Dinic::bfs()
{
	int i, u, v;

	head = tail = 1;
	
    memset(layer, -1, sizeof layer);
	for (i = 0; i <= n + 1; i++)
		color[i] = WHITE;
	
    enqueue(s);
	layer[s] = 0;
    while (head != tail) {
        u = dequeue();
        for (i = Head[u]; i != -1; i = edge[i].next) {
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
	int i;
    int result = 0, tmp = 0;
	
	if (u == t)
		return cur_residual_flow;
	
    for (i = Head[u]; i != -1 && result < cur_residual_flow; i = edge[i].next) {
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
	int i;

	color[edge[i].to] = GRAY;
	for (i = Head[u]; i != -1; i = edge[i].next)
		if(edge[i].len > 0 && color[edge[i].to] == WHITE) {
			dfs(edge[i].to);
		}

	color[u] = BLACK;
}

void Dinic::show()  
{  
    int i;

	for (i = 0; i <= n + 1; i++)
		color[i] = WHITE;

	cnts = cntt = 0;
	dfs(0);
	
    for (i = 0; i <= n + 1; i++)  
    {  
        if (color[i] == BLACK)  
            S[cnts++] = i;  
        else  
            T[cntt++] = i;  
    }  
    printf("S %d:\n", cnts);
    for (i = 0; i < cnts; i++) {  
		printf("%d", S[i]);
		printf(i < cnts - 1 ? " " : "\n");
	}
	printf("T %d:\n", cnts);
	for (i = 0; i < cntt; i++) { 
		printf("%d ", T[i]);
		printf(i < cntt - 1 ? " " : "\n");
	}
}  

int Dinic::max_flow()
{
    int ans = 0;
	
    while (bfs())
		ans += find(s, INF);
	
    return ans;
}
