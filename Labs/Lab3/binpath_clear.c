
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/utsname.h>
#include <math.h>
#include <ctype.h>
// #include<omp.h>

#define BIN_HEAP
// #define TRACK_OPTIMAL
// #define TRACK_MEMORY
// #define TRACK_RESOURCES
// #define DEBUG

// #ifdef TRACK_RESOURCES
// #define TRACK_MEMORY
// #define TRACK_BANDWIDTH

// typedef int int;

#define MAX_DISTANCE ((int)0x3FFFFFFF)
#define MATH_INF ((int)0x3FFFFFFF)
#define UNDEFINED -2

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

#define pnlinknext(to, el)       \
    {                            \
        (el)->next = (to)->next; \
        (el)->prev = (to);       \
        (to)->next->prev = (el); \
        (to)->next = (el);       \
    }
#define pnlinkprev(to, el)       \
    {                            \
        (el)->prev = (to)->prev; \
        (el)->next = (to);       \
        (to)->prev->next = (el); \
        (to)->prev = (el);       \
    }
#define pnunlink(el)                   \
    {                                  \
        (el)->next->prev = (el)->prev; \
        (el)->prev->next = (el)->next; \
    }
#define pnrelink(el)             \
    {                            \
        (el)->next->prev = (el); \
        (el)->prev->next = (el); \
    }

#define ERROR(...) error(__FILE__, __LINE__, __func__, __VA_ARGS__);

static void error(const char *fn, int line, const char *func,
                  const char *format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(stderr,
            "ERROR [file = %s, line = %d] "
            "%s: ",
            fn,
            (int)line,
            func);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}
#define MALLOC(x) malloc((x))
#define CALLOC(x, y) calloc((x), (y))
#define FREE(x) free((x))

char *strlower(char *s)
{
    char *t;

    for (t = s; *s != '\0'; s++)
        *s = (char)tolower(*s);

    return (t);
}

char *itoa(int val)
{
    int base = 10;
    static char buf[32] = {0};
    int i = 30;
    for (; val && i; --i, val /= base)
        buf[i] = "0123456789abcdef"[val % base];
    return &buf[i + 1];
}

/**************************************************************  prefix sum. */
int prefixsum(int n, int *a, int k)
{
    int run = 0;
    for (int u = 0; u < n; u++)
    {
        int tv = a[u];
        a[u] = run;
        run = run + tv + k;
    }
    return run;
}

typedef struct graph
{
    int n;
    int m;
    int num_edges;
    int edge_capacity;
    int *edges;
} graph_t;

static int *enlarge(int m, int m_was, int *was)
{
    assert(m >= 0 && m_was >= 0);

    int *a = (int *)MALLOC(sizeof(int) * m);
    int i;
    if (was != (void *)0)
    {
        for (i = 0; i < m_was; i++)
        {
            a[i] = was[i];
        }
        FREE(was);
    }
    return a;
}

graph_t *graph_alloc()
{
    graph_t *g = (graph_t *)MALLOC(sizeof(graph_t));
    g->n = 0;
    g->m = 0;
    g->num_edges = 0;
    g->edge_capacity = 100;
    g->edges = enlarge(3 * g->edge_capacity, 0, (void *)0);

    return g;
}

void graph_free(graph_t *g)
{
    if (g->edges != NULL)
        FREE(g->edges);
    FREE(g);
}

void graph_add_edge(graph_t *g, int u, int v, int w)
{
    // assert(u >= 0 && v >= 0 && u < g->n && v < g->n);

    if (g->num_edges == g->edge_capacity)
    {
        g->edges = enlarge(6 * g->edge_capacity, 3 * g->edge_capacity, g->edges);
        g->edge_capacity *= 2;
    }

    assert(g->num_edges < g->edge_capacity);

    int *e = g->edges + 3 * g->num_edges;
    g->num_edges++;
    e[0] = u;
    e[1] = v;
    e[2] = w;
    int *f = g->edges + 3 * g->num_edges;
    g->num_edges++;
    f[0] = v;
    f[1] = u;
    f[2] = w;
}

#define MAX_LINE_SIZE 1024

graph_t *graph_load(FILE *in)
{
    char in_line[MAX_LINE_SIZE];
    int u, v, w;
    graph_t *g = graph_alloc();
    g->n = (int)23947347;
    g->m = (int)58333344;
    while (fgets(in_line, MAX_LINE_SIZE, in) != NULL)
    {
        char *line = strlower(in_line);
        sscanf(line, "%d %d %d", &u, &v, &w);
        graph_add_edge(g, u - 1, v - 1, w);
    }
    return g;
}

typedef struct dijkstraq
{
    int n;
    int m;
    int src;
    int dst;
    int *pos;
    int *adj;
} dijkstra_t;

dijkstra_t *root_build(graph_t *g, int src, int dst)
{

    int n = g->n;
    int m = g->m;
    int *pos = (int *)MALLOC((n + 1) * sizeof(int));
    int *adj = (int *)MALLOC(((n + 1) + (4 * m) + (2 * n)) * sizeof(int));

    assert(src >= 0 && src < n);
    assert(dst >= -1 && dst < n);

    dijkstra_t *root = (dijkstra_t *)MALLOC(sizeof(dijkstra_t));
    root->n = n;
    root->m = m;
    root->src = src;
    root->dst = dst;
    root->pos = pos; //
    root->adj = adj;

    for (int u = 0; u < n; u++)
        pos[u] = 0;

    int *e = g->edges;
    for (int j = 0; j < 3 * m; j += 3)
    {
        pos[e[j]] += 2;
        pos[e[j + 1]] += 2;
    }

    pos[n] = (2 * n);

    int run = prefixsum(n + 1, pos, 1);

    for (int u = 0; u < n; u++)
        adj[pos[u]] = 0;

    for (int j = 0; j < 3 * m; j += 3)
    {
        int u = e[j + 0];
        int v = e[j + 1];
        int w = e[j + 2];
        int pu = pos[u];
        int pv = pos[v];
        int i_pu = pu + 1 + (2 * adj[pu]);
        int i_pv = pv + 1 + (2 * adj[pv]);

        adj[i_pv] = u;
        adj[i_pv + 1] = w;
        adj[pv]++;
        adj[i_pu] = v;
        adj[i_pu + 1] = w;
        adj[pu]++;
    }

    int u = n;
    adj[pos[u]] = 0;
    int pu = pos[u];
    for (int v = 0; v < n; v++)
    {
        int i_pu = pu + 1 + (2 * adj[pu]);
        adj[i_pu] = v;
        adj[i_pu + 1] = MATH_INF;
        adj[pu]++;
    }

    return root;
}

void dijkstra_free(dijkstra_t *root)
{
    if (root->pos != NULL)
        FREE(root->pos);
    if (root->adj != NULL)
        FREE(root->adj);
    FREE(root);
}

typedef struct bheap_item
{
    int item;
    int key;
} bheap_item_t;

typedef struct bheap
{
    int max_n;
    int n;       // size of binary heap
    bheap_item_t *a; // stores (distance, vertex) pairs of the binary heap
    int *p;      // stores the positions of vertices in the binary heap

} bheap_t;

bheap_t *bh_alloc(int n)
{
    bheap_t *h = (bheap_t *)malloc(sizeof(bheap_t));
    h->max_n = n;
    h->n = 0;
    h->a = (bheap_item_t *)malloc((n + 1) * sizeof(bheap_item_t));
    h->p = (int *)malloc(n * sizeof(int));
    return h;
}

void bh_free(bheap_t *h)
{
    free(h->a);
    free(h->p);
    free(h);
}

static void bh_siftup(bheap_t *h, int p, int q)
{
    int j = p;
    int k = 2 * p;
    bheap_item_t y = h->a[p];

    while (k <= q)
    {
        bheap_item_t z = h->a[k];
        if (k < q)
        {
            if (z.key > h->a[k + 1].key)
                z = h->a[++k];
        }

        if (y.key <= z.key)
            break;
        h->a[j] = z;
        h->p[z.item] = j;
        j = k;
        k = 2 * j;
    }

    h->a[j] = y;
    h->p[y.item] = j;
}

bheap_item_t bh_min(bheap_t *h)
{
    return (bheap_item_t)h->a[1];
}

static void bh_insert(bheap_t *h, int item, int key)
{
    int i = ++(h->n);

    while (i >= 2)
    {
        int j = i / 2;
        bheap_item_t y = h->a[j];

        if (key >= y.key)
            break;

        h->a[i] = y;
        h->p[y.item] = i;
        i = j;
    }

    h->a[i].item = item;
    h->a[i].key = key;
    h->p[item] = i;
}

static void bh_delete(bheap_t *h, int item)
{
    int n = --(h->n);
    int p = h->p[item];

    if (p <= n)
    {
        if (h->a[p].key <= h->a[n + 1].key)
        {
            h->a[p] = h->a[n + 1];
            h->p[h->a[p].item] = p;
            bh_siftup(h, p, n);
        }
        else
        {
            h->n = p - 1;
            bh_insert(h, h->a[n + 1].item, h->a[n + 1].key);
            h->n = n;
        }
    }
}

static void bh_decrease_key(bheap_t *h, int item, int new_key)
{

    int i = h->p[item];
    while (i >= 2)
    {
        int j = i / 2;
        bheap_item_t y = h->a[j];

        if (new_key >= y.key)
            break;

        h->a[i] = y;
        h->p[y.item] = i;
        i = j;
    }

    h->a[i].item = item;
    h->a[i].key = new_key;
    h->p[item] = i;
}

static int bh_delete_min(bheap_t *h)
{
    bheap_item_t min = (bheap_item_t)h->a[1];
    int u = min.item;
    bh_delete((bheap_t *)h, u);
    return u;
}

// allocation
#define heap_alloc(n) bh_alloc((n))
#define heap_free(h) bh_free((bheap_t *)(h));
// heap operations
#define heap_insert(h, v, k) bh_insert((h), (v), (k))
#define heap_delete_min(h) bh_delete_min((h));
#define heap_decrease_key(h, v, k) bh_decrease_key((h), (v), (k));
// fetch structure elements
#define heap_n(h) ((bheap_t *)h)->n;
#define heap_key_comps(h) ((bheap_t *)h)->key_comps;
#define heap_mem(h) (h)->mem;
// heap nodes
#define heap_node_t bheap_item_t
#define heap_t bheap_t

/************************************************************ traceback path. */

void tracepath(int n, int s, int cost,
               int v, int *p)
{
    fprintf(stdout, "[source: %d] [destination: %d] [cost: %d]\n",
            s + 1, v + 1, cost);
    fprintf(stdout, "[source: %d] [destination: %d] [cost: %s]\n",
            s + 1, v + 1, cost == MAX_DISTANCE ? "INFINITY" : itoa(cost));

    if (s == v)
    {
        fprintf(stdout, "\n\n");
        return;
    }

    if (cost == MAX_DISTANCE)
    {
        fprintf(stdout, "No path from %d to %d\n\n", s + 1, v + 1);
        return;
    }

    int u = p[v];
    while (u != s)
    {
        if (u == UNDEFINED)
            return;
        fprintf(stdout, "%d->", v + 1);
        v = u;
        u = p[v];
    }
    fprintf(stdout, "%d->%d\n\n", v + 1, u + 1);
}

void dijkstra(int n,
              int m,
              int *pos,
              int *adj,
              int s,
              int *d,
              int *visit,
              int *p,
              int dst)
{

    heap_t *h = heap_alloc(n);
    //initialise
    for (int v = 0; v < n; v++)
    {
        d[v] = MAX_DISTANCE; //mem: n
        visit[v] = 0;        //mem: n
        p[v] = UNDEFINED;    //mem: n
    }
    d[s] = 0;
    p[s] = UNDEFINED;

    for (int v = 0; v < n; v++)
        heap_insert(h, v, d[v]);

    //visit and label
    while (h->n > 0)
    {
        int u = heap_delete_min(h); // pick out the min one
        visit[u] = 1;   //  visited
        if(u == dst)
        {
            break;
        }
        int pos_u = pos[u]; // 
        int *adj_u = adj + pos_u;
        int n_u = adj_u[0];
        for (int i = 1; i <= 2 * n_u; i += 2)
        {
            int v = adj_u[i];   // v = adj_u[i];
            int d_v = d[u] + adj_u[i + 1];
            if (!visit[v] && d[v] > d_v)
            {
                d[v] = d_v;
                p[v] = u;
                heap_decrease_key(h, v, d_v);
            }

        }
        //mem: 2n+6m
    }
}

int dijkstra_query(dijkstra_t *root)
{

    int n = root->n;
    int m = root->m;
    int src = root->src;
    int dst = root->dst;
    int *d = (int *)MALLOC(n * sizeof(int));
    int *visit = (int *)MALLOC(n * sizeof(int));
    int *p = (int *)MALLOC(n * sizeof(int));
    dijkstra(n, m, root->pos, root->adj, src, d, visit, p, dst);
    //trace path
    int min_cost = 0;

    if (dst != -1)
    {
        min_cost = d[dst];

        // printf("min_cost1 = %d\n",min_cost);
        tracepath(n, src, min_cost, dst, p);
    }
    else
    {
        for (int v = 0; v < n; v++)
        {
            if (v != src)
            {
                min_cost = d[v];
                // printf("min_cost2 = %d\n",min_cost);
                tracepath(n, src, min_cost, v, p);
            }
        }
    }

    FREE(d);
    FREE(visit);
    FREE(p);
    return min_cost;
}

/******************************************************* Program entry point. */

int main(int argc, char **argv)
{
    if (argc > 1 && !strcmp(argv[1], "-h"))
    {
        fprintf(stdout, "Usage: %s -in <in-file> -src <source> -dst <destination>\n\n", argv[0]);
        return 0;
    }

    int has_input = 0;
    int has_source = 0;
    int has_destination = 0;
    char *filename = NULL;
    ;
    int src = -1;
    int dst = -1;

    for (int f = 1; f < argc; f++)
    {
        if (argv[f][0] == '-')
        {
            if (!strcmp(argv[f], "-in"))
            {
                has_input = 1;
                filename = argv[++f];
            }
            if (!strcmp(argv[f], "-src"))
            {
                has_source = 1;
                src = atol(argv[++f]) - 1;
            }
            if (!strcmp(argv[f], "-dst"))
            {
                has_destination = 1;
                dst = atol(argv[++f]);
            }
        }
    }

    FILE *in = NULL;
    if (!has_input)
    {
        //read graph from standard input

        in = stdin;
    }
    else
    {
        in = fopen(filename, "r");
        if (in == NULL)
            ERROR("unable to open file '%s'", filename);
    }

    if (!has_source)
    {

        src = 0;
    }

    if (!has_destination)
    {
    }
    else
    {
        if (dst != -1)
            dst--;
    }

    //read input graph
    graph_t *g = graph_load(in);
    //build root query
    dijkstra_t *root = root_build(g, src, dst);
    //release graph memory
    graph_free(g);
    //execute the algorithm
    dijkstra_query(root);
    //release query memory
    dijkstra_free(root);
    return 0;
}