/*
ID: wrpaape1
LANG: C
TASK: agrinet
*/

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>

#define TASK "agrinet"

const unsigned long INFINITE_DISTANCE = ULONG_MAX;

enum Limits {
    N_MIN        = 3,
    N_MAX        = 100,
    DISTANCE_MAX = 100000
};

struct Graph {
    unsigned char num_nodes;
    unsigned long distance[N_MAX][N_MAX];
};

struct DistanceQueue {
    unsigned char       num_nodes;
    unsigned char       nodes[N_MAX];
    unsigned long       distance_to[N_MAX];
    const struct Graph *graph;
};

static void
dq_init(struct DistanceQueue *dq,
        const struct Graph   *graph)
{
    dq->num_nodes = graph->num_nodes;
    for (unsigned char node = 0; node < dq->num_nodes; ++node) {
        dq->nodes[node]       = node;
        dq->distance_to[node] = INFINITE_DISTANCE;
    }
    dq->distance_to[0] = 0;
    dq->graph          = graph;
}

static unsigned long
dq_get_distance_to(const struct DistanceQueue *dq,
                   unsigned char               i_node)
{
    return dq->distance_to[dq->nodes[i_node]];
}

static void
dq_check_child(const struct DistanceQueue *dq,
               unsigned char               i_child,
               unsigned long              *distance_to_closest,
               unsigned char              *i_closest)
{
    unsigned long distance_to_child = dq_get_distance_to(dq, i_child);
    if (distance_to_child < *distance_to_closest) {
        *distance_to_closest = distance_to_child;
        *i_closest           = i_child;
    }
}

static void
dq_swap(struct DistanceQueue *dq,
        unsigned char         i_node1,
        unsigned char         i_node2)
{
    unsigned char *ptr1 = &dq->nodes[i_node1];
    unsigned char *ptr2 = &dq->nodes[i_node2];

    unsigned char tmp = *ptr2;
    *ptr2             = *ptr1;
    *ptr1             = tmp;
}

static void
dq_heapify_down(struct DistanceQueue *dq)
{
    unsigned char replacement_node = dq->nodes[--dq->num_nodes];
    dq->nodes[0]                   = replacement_node;

    unsigned char i_parent = 0;
    while (1) {
        unsigned char i_closest           = i_parent;
        unsigned long distance_to_closest = dq_get_distance_to(dq, i_closest);

        unsigned char i_lchild = i_closest * 2;
        if (i_lchild >= dq->num_nodes)
            break;

        dq_check_child(dq, i_lchild, &distance_to_closest, &i_closest);

        unsigned char i_rchild = i_lchild + 1;
        if (i_rchild < dq->num_nodes)
            dq_check_child(dq, i_rchild, &distance_to_closest, &i_closest);

        if (i_closest == i_parent)
            break;

        dq_swap(dq, i_parent, i_closest);

        i_parent = i_closest;
    }
}

static void
dq_heapify_up(struct DistanceQueue *dq,
              unsigned char         i_node)
{
    unsigned long distance_to_node = dq_get_distance_to(dq, i_node);
    while (1) {
        if (i_node == 0)
            break;

        unsigned char i_parent = i_node / 2;
        if (distance_to_node >= dq_get_distance_to(dq, i_parent))
            break;

        dq_swap(dq, i_node, i_parent);

        i_node = i_parent;
    }
}

static void
dq_update_distance_to(struct DistanceQueue *dq,
                      unsigned char       closest_node,
                      unsigned char       i_node)
{
    unsigned char node     = dq->nodes[i_node];
    unsigned long distance = dq->graph->distance[closest_node][node];

    unsigned long *distance_to = &dq->distance_to[node];
    if (distance < *distance_to) {
        *distance_to = distance;
        dq_heapify_up(dq, i_node);
    }
}

static void
dq_correct(struct DistanceQueue *dq)
{
    unsigned char closest_node = dq->nodes[0];
    dq_heapify_down(dq);

    for (unsigned char i_node = 0; i_node < dq->num_nodes; ++i_node) {
        dq_update_distance_to(dq, closest_node, i_node);
    }
}

static long
dq_pop_closest(struct DistanceQueue *dq)
{
    long closest_distance = -1;
    if (dq->num_nodes > 0) {
        unsigned char closest_node = dq->nodes[0];
        closest_distance           = (long) dq->distance_to[closest_node];
        dq_correct(dq);
    }
    return closest_distance;
}

static FILE *
open_file(const char *filename,
          const char *mode)
{
    FILE *file = fopen(filename, mode);
    assert(file);
    return file;
}

static FILE *
open_input(void)
{
    return open_file(TASK ".in", "r");
}

static FILE *
open_output(void)
{
    return open_file(TASK ".out", "w");
}

static void
read_file(FILE       *file,
          const char *format,
          ...)
{
    va_list args;
    va_start(args, format);
    int num_items = vfscanf(file, format, args);
    va_end(args);
    assert(num_items >= 0);
    (void) num_items;
}

static void
write_file(FILE       *file,
           const char *format,
           ...)
{
    va_list args;
    va_start(args, format);
    int num_chars = vfprintf(file, format, args);
    va_end(args);
    assert(num_chars >= 0);
    (void) num_chars;
}

static void
close_file(FILE *file)
{
    int error = fclose(file);
    assert(error == 0);
    (void) error;
}

static void
read_num(FILE          *file,
         unsigned long *num)
{
    read_file(file, "%lu",
              num);
}

static unsigned char
read_num_nodes(FILE *input)
{
    unsigned long num_nodes = 0;
    read_num(input, &num_nodes);
    assert(num_nodes >= N_MIN);
    assert(num_nodes <= N_MAX);
    return (unsigned char) num_nodes;
}

static unsigned long
read_distance(FILE *input)
{
    unsigned long distance = 0;
    read_num(input, &distance);
    assert(distance <= DISTANCE_MAX);
    return distance;
}

static void
read_input(struct Graph *graph)
{
    FILE *input = open_input();

    graph->num_nodes = read_num_nodes(input);
    for (unsigned long from = 0; from < graph->num_nodes; ++from) {
        for (unsigned long to = 0; to < graph->num_nodes; ++to) {
            graph->distance[from][to] = read_distance(input);
        }
    }

    close_file(input);
}

static unsigned long
solve(const struct Graph *graph)
{
    struct DistanceQueue dq;
    dq_init(&dq, graph);

    unsigned long total_length = 0;

    while (1) {
        long closest_distance = dq_pop_closest(&dq);
        if (closest_distance < 0)
            break;

        total_length += (unsigned long) closest_distance;
    }

    return total_length;
}

static void
write_output(unsigned long min_req_len)
{
    FILE *output = open_output();
    write_file(output, "%lu\n",
               min_req_len);
    close_file(output);
}

int
main(void)
{
    struct Graph graph;
    read_input(&graph);

    unsigned long min_req_len = solve(&graph);

    write_output(min_req_len);

    return 0;
}
