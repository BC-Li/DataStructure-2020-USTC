#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<sys/utsname.h>
#include<math.h>
#include<ctype.h>
#include<stdint.h>
#include<omp.h>

#define TRACK_RESOURCES
#define MAX_DISTANCE ((int)0x3FFFFFFF)
#define MATH_INF ((int)0x3FFFFFFF)
#define UNDEFINED -2


#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

//  get CPU model name

struct cpuid_result {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
};
static inline struct cpuid_result cpuid(unsigned int op)
{
    struct cpuid_result result;
    __asm volatile(
        "mov %%ebx, %%edi;"
        "cpuid;"
        "mov %%ebx, %%esi;"
        "mov %%edi, %%ebx;"
        : "=a" (result.eax),
        "=S" (result.ebx),
        "=c" (result.ecx),
        "=d" (result.edx)
        : "0" (op)
        : "edi");
    return result;
}
void get_cpu_name(char* processor_name)
{
    unsigned int cpuid_op = 0x80000002;
    struct cpuid_result regs;
    char temp_processor_name[50];
    char* processor_name_start;
    unsigned int *name_as_ints = (unsigned int *)temp_processor_name;
    unsigned int i;
    for (i = 0; i < 3; i++) {
        regs = cpuid(cpuid_op + i);
        name_as_ints[i * 4 + 0] = regs.eax;
        name_as_ints[i * 4 + 1] = regs.ebx;
        name_as_ints[i * 4 + 2] = regs.ecx;
        name_as_ints[i * 4 + 3] = regs.edx;
    }

    temp_processor_name[49] = '\0';
    processor_name_start = temp_processor_name;
    while (*processor_name_start == ' ')
        processor_name_start++;

    memset(processor_name, 0, 49);
    strcpy(processor_name, processor_name_start);
}

void get_hw_cpu(void)
{
    char buffer[49] = { '\0' };
    memset((void *)buffer, '\0', sizeof(buffer));
    get_cpu_name(buffer);
    fprintf(stdout, "CPU: %s\n", buffer);

}

#define MALLOC(x) malloc((x))
#define CALLOC(x, y) calloc((x), (y))
#define FREE(x) free((x))


#define TIME_STACK_CAPACITY 256
double start_stack[TIME_STACK_CAPACITY];            // use an double int array to store time points
int start_stack_top = -1;

void push_time(void)
{
    start_stack[++start_stack_top] = omp_get_wtime();  
}   //  The omp_get_wtime routine returns elapsed wall clock time in seconds.
    //  see https://openmp.org/spec-html/5.0/openmpsu160.html

double pop_time(void)
{
    double wstop = omp_get_wtime();                 // time now
    double wstart = start_stack[start_stack_top--]; // get the time (last active point)
    return (double) (1000.0*(wstop-wstart));        // return time between 2 points
}

char* strlower( char *s)    // func turning into small letter
{
   char* t;

   for(t = s; *s != '\0'; s++)
      *s = (char)tolower(*s);

   return(t);
}

char* itoa(int val) //  func itoa,turning int into char type
{
    int base = 10;
	static char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];
	return &buf[i+1];
}

void prefixsum(int n, int *a, int k)    // return cumulative sum V[i] = V[i-1] + V[i];
{
    int run = 0;
    for(int u = 0; u < n; u++) {
        int tv = a[u];
        a[u] = run;
        run = run + tv + k;
    }
    
}


typedef struct graph
{
    int n;
    int m;
    int num_edges;
    int edge_capacity;
    int *edges;
} graph;

static int *enlarge(int m, int m_was, int *was)
{

    int *a = (int *) MALLOC(sizeof(int)*m);
    int i;
    if(was != (void *) 0) { 
        for(i = 0; i < m_was; i++) {
            a[i] = was[i];
        }
        FREE(was);
    }    
    return a;
}

graph *graph_alloc()
{
    graph *g = (graph *) MALLOC(sizeof(graph));
    g->n = 0; 
    g->m = 0; 
    g->num_edges      = 0;
    g->edge_capacity  = 100;
    g->edges          = enlarge(3*g->edge_capacity, 0, (void *) 0);
    
    return g;
}

void graph_free(graph *g)
{
    if(g->edges != NULL)
        FREE(g->edges);
    FREE(g);
}

void graph_add_edge(graph *g, int u, int v, int w)
{

    if(g->num_edges == g->edge_capacity) {
        g->edges = enlarge(6*g->edge_capacity, 3*g->edge_capacity, g->edges);   // 3 is for source, destination and weight. 6 doubled its capacity.
        g->edge_capacity *= 2;
    }


    int *e = g->edges + 3*g->num_edges; // fetch the new location of the new edge
    g->num_edges++;
    e[0] = u;
    e[1] = v;
    e[2] = w;
    int *f = g->edges + 3*g->num_edges; // non-directed graph, adding another edge
    g->num_edges++;
    f[0] = v;
    f[1] = u;
    f[2] = w;
}

#define MAX_SIZE 1024

graph * graph_load(FILE *in)
{
    push_time();

    int u, v, w;
    graph *g = graph_alloc();

    //  hard encode the number of vertices and edges. always remember to change it if replace original dataset !!!

    g->n = (int)23947347;   // USA dataset
    g->m = (int)58333344;
    
    //  reading the lines via fread. read 3 int at a time.

    int s[3];
    while(fread(s, sizeof(int), 3, in)) {
        u = s[0];
        v = s[1];
        w = s[2];
        graph_add_edge(g, u-1, v-1, w); //  add a edge
    }

    double time = pop_time();   //  get current time
    fprintf(stdout, "vertices (n) = %d, edges (m) = %d. I/O time: %.2lf ms ",g->n, g->m, time);   //  print time used in I/O input
    fprintf(stdout, " ");
    fprintf(stdout, "\n");
    fflush(stdout);
    return g;
}


typedef struct dijkstraq
{
    int  n;
    int  m;
    int  src;
    int  dst;
    int *pos;
    int *adj;
}dijkstra_t;

dijkstra_t *root_build(graph *g, int src, int dst)
{
    double time;
    fprintf(stdout, "Time on building dijkstra root: ");
    push_time();    //  record root build starting time

    int n    = g->n;
    int m    = g->m;
    int *pos = (int *) MALLOC((n+1)*sizeof(int));   // vertices
    int *adj = (int *) MALLOC(((n+1)+(4*m)+(2*n))*sizeof(int));


    dijkstra_t *root = (dijkstra_t *) MALLOC(sizeof(dijkstra_t));
    root->n   = n;
    root->m   = m;
    root->src = src;
    root->dst = dst;
    root->pos = pos;    //  position
    root->adj = adj;    //  adjacent vertices

    for(int u = 0; u < n; u++)
        pos[u] = 0;     //

    int *e = g->edges;
    for(int j = 0; j < 3*m; j+=3) {
        pos[e[j]]+=2;
        pos[e[j+1]]+=2;
    }

    pos[n] = (2*n);
    prefixsum(n+1, pos, 1);

    for(int u = 0; u < n; u++)
        adj[pos[u]] = 0;

    for(int j = 0; j < 3*m; j+=3) {
        int u    = e[j+0];
        int v    = e[j+1];
        int w    = e[j+2];
        int pu   = pos[u];
        int pv   = pos[v];
        int i_pu = pu + 1 + (2*adj[pu]);
        int i_pv = pv + 1 + (2*adj[pv]);

        adj[i_pv]     = u;
        adj[i_pv + 1] = w;
        adj[pv]++;
        adj[i_pu]     = v;
        adj[i_pu + 1] = w;
        adj[pu]++; 
    }

    int u = n;
    adj[pos[u]] = 0;
    int pu = pos[u];
    for(int v = 0; v < n; v++) {
        int i_pu  = pu + 1 + (2*adj[pu]);
        adj[i_pu]     = v;
        adj[i_pu + 1] = MATH_INF;
        adj[pu]++;
    }

    time = pop_time();
    fprintf(stdout, "%.2lf ms ", time);
    fprintf(stdout, " ");
    fprintf(stdout, "\n");
    fflush(stdout);

    return root;
}

void dijkstra_free(dijkstra_t *root)
{
    if(root->pos != NULL)
        FREE(root->pos);
    if(root->adj != NULL)
        FREE(root->adj);
    FREE(root);
}


typedef struct bheap_item   // bineay heap node
{
    int item;
    int key; 
} bheap_item_t; 

typedef struct bheap
{
    int max_n;
    int n;           //  size of binary heap
    bheap_item_t *a; //  stores (distance, vertex) pairs of the binary heap
    int *p;          //  stores the positions of vertices in the binary heap
} bheap_t;           //  bineay heap t

bheap_t * bh_alloc(int n)
{
    bheap_t *h = (bheap_t *) malloc(sizeof(bheap_t));
    h->max_n = n;
    h->n = 0; 
    h->a = (bheap_item_t *) malloc((n+1)*sizeof(bheap_item_t));
    h->p = (int *) malloc(n*sizeof(int));
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

    while(k <= q) {
        bheap_item_t z = h->a[k];
        if(k < q) {
            if(z.key > h->a[k + 1].key) z = h->a[++k];
        }

        if(y.key <= z.key) break;
        h->a[j] = z;
        h->p[z.item] = j;
        j = k;
        k = 2 * j;
    }

    h->a[j] = y;
    h->p[y.item] = j;

}

bheap_item_t bh_min(bheap_t *h)     // return the min element of the binary heap
{
    return (bheap_item_t) h->a[1];
}

static void bh_insert(bheap_t *h, int item, int key)    // insert an element to a binary heap
{ 
    int i = ++(h->n);   //  add an element to h, size of heap + 1

    while(i >= 2) {
        int j = i / 2;  //  float the data, j is the parent node of i
        bheap_item_t y = h->a[j];
        if(key >= y.key) break;

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

    if(p <= n) {
        if(h->a[p].key <= h->a[n + 1].key) {
            h->a[p] = h->a[n + 1];
            h->p[h->a[p].item] = p;
            bh_siftup(h, p, n);
        }
        else {
            h->n = p - 1;
            bh_insert(h, h->a[n + 1].item, h->a[n+1].key);
            h->n = n;
        }
    }
}

static void bh_decrease_key(bheap_t *h, int item, int new_key)
{

    int i = h->p[item];
    while(i >= 2) {
        int j = i / 2;
        bheap_item_t y = h->a[j];
        if(new_key >= y.key) break;
        h->a[i] = y;
        h->p[y.item] = i;
        i = j;
    }

    h->a[i].item = item;
    h->a[i].key = new_key;
    h->p[item] = i;
}

static int bh_delete_min(bheap_t * h)
{    
    bheap_item_t min = (bheap_item_t) h->a[1];
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
    fprintf(stdout, "[source: %d] [destination: %d] [cost: %s]\n\n", 
                     s+1, v+1, cost==MAX_DISTANCE?"INFINITY":itoa(cost));
    
    if(s == v) {
        fprintf(stdout, "\n\n");
        return;
    }
    if(cost == MAX_DISTANCE) {
        fprintf(stdout, "No path from %d to %d\n\n", s+1, v+1);
        return;
    }
    int u = p[v];
    while(u != s) {
        if(u == UNDEFINED)
            return;
        fprintf(stdout, "%d<-", v+1);
        v = u;
        u = p[v];
    }
    fprintf(stdout, "%d<-%d\n\n", v+1, u+1);
}

    // dijkstra(n, m, root->pos, root->adj, src, d, visit,p,dst);

void dijkstra(int n,int m, int *pos, int *adj, int s, int *d,int *visit,int *p,int dst)
{
    fprintf(stdout, "dijkstra: ");
    push_time();
    push_time();

    heap_t *h = heap_alloc(n);
    //initialise
    for(int v = 0; v < n; v++) {
        d[v]     = MAX_DISTANCE; //mem: n
        visit[v] = 0;            //mem: n
        p[v]     = UNDEFINED;    //mem: n
    }
    d[s] = 0;   // distance from src to src is 0
    p[s] = UNDEFINED;   

    for(int v = 0; v < n; v++)
        heap_insert(h, v, d[v]);    // add vertices to binary heap



    fprintf(stdout, "[init: %.2lfms]", pop_time());
    push_time();
    //visit and label
    while(h->n > 0) {   // while heap is not empty
        int u = heap_delete_min(h);    //   get the vertice at heap top
        visit[u]  = 1;                 //   label it
        if(u == dst){                  //   if u = dst then pause search loop
            break;                     
        }
        int pos_u  = pos[u];        //  get position of u
        int *adj_u = adj + pos_u;   //  find position of U
        int n_u    = adj_u[0];      //  
        for(int i = 1; i <= 2*n_u; i += 2) {    
            int v   = adj_u[i];
            int d_v = d[u] + adj_u[i+1];    // search all the vertices related to u
            if(!visit[v] && d[v] > d_v) {   // if v > d_v
                d[v] = d_v;                 // update
                p[v] = u;                   // p_v = u
                heap_decrease_key(h, v, d_v);   //  float up
            }
        }
    }
    fprintf(stdout, " [visit: %.2lfms] [%.2lfms]\n", pop_time(), pop_time());
    fflush(stdout);

    heap_free(h);
}


int dijkstra_query(dijkstra_t *root)
{
    push_time();

    int n        = root->n;
    int m        = root->m;
    int src      = root->src;
    int dst      = root->dst;
    int *d       = (int *) MALLOC(n*sizeof(int));
    int *visit   = (int *) MALLOC(n*sizeof(int));
    int *p       = (int *) MALLOC(n*sizeof(int));

    //execute query
    push_time();
    dijkstra(n, m, root->pos, root->adj, src, d, visit,p,dst);
    double dijkstra_time = pop_time();
    push_time();

    //trace path
    int min_cost = 0;

    if(dst != -1) {
        min_cost = d[dst];

        tracepath(n, src, min_cost, dst, p);
    } else {
        for(int v = 0; v < n; v++) {
            if(v != src) {
                min_cost = d[v];
                tracepath(n, src, min_cost, v, p);
            }
        }
    }


    double trace_time = pop_time();

    fprintf(stdout, "Total Search Time [%.2lfms] ", pop_time());
    fprintf(stdout, " ");
    fprintf(stdout, "\n");
    fflush(stdout);


    FREE(d);
    FREE(visit);
    FREE(p);
    return min_cost;
}

/******************************************************* Program entry point. */

int main(int argc, char **argv)
{

    push_time();
    for(int f = 0; f < argc; f++) 
        fprintf(stdout, " %s", argv[f]);
    fprintf(stdout, "\n");

    if(argc > 1 && !strcmp(argv[1], "-h")) {
        fprintf(stdout, "Usage: %s -in <in-file> -src <source> -dst <destination>\n\n", argv[0]);
        return 0;
    } 
    char *filename = NULL;;
    int src = -1;
    int dst = -1;

    for(int f = 1; f < argc; f++) {
        if(argv[f][0] == '-') {
            if(!strcmp(argv[f], "-in")) {
                filename = argv[++f];
            }
            if(!strcmp(argv[f], "-src")) {
                src = atol(argv[++f]) - 1;
            }
            if(!strcmp(argv[f], "-dst")) {
                dst = atol(argv[++f]);
            }
        }
    }

    FILE *in = NULL;
        in = fopen(filename, "rb");
        if(dst != -1) dst--;


    //read input graph
    graph *g = graph_load(in);
    //build root query
    dijkstra_t *root = root_build(g, src, dst);
    //release graph memory
    graph_free(g);
    //execute the algorithm
    dijkstra_query(root);
    //release query memory
    dijkstra_free(root);


    double time = pop_time();
    fprintf(stdout, "Total time[%.2lfms] ", time);
    fprintf(stdout, "\n");
    get_hw_cpu();
    fflush(stdout);
    return 0;
}