#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 201000
#define M  2001000
#define INF 100000000

#define WHITE 0
#define GRAY 1
#define BLACK 2

typedef struct {
	int next;
	int to;
	int len;
} Edge;

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

static int min(int a, int b)
{
	return (a < b) ? a : b;
}

static void enqueue(int x)
{
    q[tail] = x;
    tail++;
    color[x] = GRAY;
}

static int dequeue()
{
    int x = q[head];
    head++;
    color[x] = BLACK;
	
    return x;
}

static void add_edge(int u,int v,int w)
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

void read()
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

int bfs()
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

int find(int u, int cur_residual_flow)
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

void dfs(int u)
{
	int i;

	color[edge[i].to] = GRAY;
	for (i = Head[u]; i != -1; i = edge[i].next)
		if(edge[i].len > 0 && color[edge[i].to] == WHITE) {
			dfs(edge[i].to);
		}

	color[u] = BLACK;
}

void show()  
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

int dinic()
{
    int ans = 0;
	
    while (bfs())
		ans += find(s, INF);
	
    return ans;
}

int main(int argc, char *argv[])
{
    while (scanf("%d %d", &n, &m) != EOF) {
        read();
        printf("%d\n", dinic());
		show();
    }
	
    return 0;
}
