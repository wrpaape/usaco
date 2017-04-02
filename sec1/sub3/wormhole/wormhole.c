/*
ID: wrpaape1
LANG: C
TASK: wormhole
*/

#include <stdio.h>
#include <assert.h>
#define N_MAX 12


struct YNode {
	size_t y;
	size_t count;
	struct YNode *restrict next;
};

static struct YNode *restrict y_map[N_MAX];
static struct YNode y_nodes[N_MAX];
static struct YNode *restrict y_node      = &y_nodes[0];
static struct YNode *restrict y_nodes_end = &y_nodes[0];

struct GraphNode {
	unsigned int visited;
	struct GraphNode *jump;
	struct GraphNode *forward;
	struct GraphNode *next;
};

static struct GraphNode graph_nodes[N_MAX];
static struct GraphNode *restrict graph_nodes_begin = &graph_nodes[0];
static struct GraphNode *restrict graph_nodes_end   = &graph_nodes[0];
static struct GraphNode *restrict rem_unpaired;

static size_t N;
static size_t loop_count;

static inline void
update_map(const size_t y)
{
	struct YNode *restrict *restrict bucket;
	struct YNode *restrict y_node;


	bucket = &y_map[y % N];

	while (y_node = *bucket, y_node) {
		if (y_node->y == y) {
			++(y_node->count);
			return;
		}

		bucket = &y_node->next;
	}

	y_nodes_end->y     = y;
	y_nodes_end->count = 1;

	*bucket = y_nodes_end++;
}

static inline void
clear_visted(void)
{
	struct GraphNode *restrict graph_node;
	graph_node = graph_nodes_begin;
	do {
		graph_node->visited = 0;
	} while (++graph_node < graph_nodes_end);
}

static inline void
check_loop(void)
{
	struct GraphNode *restrict graph_node;
	struct GraphNode *restrict start_node;

	start_node = graph_nodes_begin;
	do {
		clear_visted();

		graph_node = start_node;

		while (1) {
			/* mark nodes that have been jumped */
			graph_node->visited = 1;

			graph_node = graph_node->jump->forward;

			if (!graph_node)
				break;

			if (graph_node->visited) {
				++loop_count;
				return;
			}
		}
	} while (++start_node < graph_nodes_end);
}

static inline void
init_row(size_t count)
{
	struct GraphNode *restrict next_graph_node;
	struct GraphNode *restrict prev_graph_node;

	prev_graph_node = graph_nodes_end++;

	while (1) {
		prev_graph_node->next = rem_unpaired;
		rem_unpaired          = prev_graph_node;

		if (--count == 0)
			return;

		next_graph_node          = graph_nodes_end++;
		prev_graph_node->forward = next_graph_node;
		prev_graph_node          = next_graph_node;
	}
}

void
comb_pairs(void)
{
	struct GraphNode *restrict node1;
	struct GraphNode *restrict node2;
	struct GraphNode *restrict *restrict prev_ptr;
	struct GraphNode *restrict *restrict node1_jump;
	struct GraphNode *restrict next;

	if (!rem_unpaired) {
		check_loop(); /* all nodes paired, check cycle */
		return;
	}

	/* pop 'node1' from head of 'rem_unpaired'
	 * traverse remaining list, popping 'node2'
	 * from middle and pairing with 'node1'
	 */

	node1        = rem_unpaired;
	node1_jump   = &node1->jump;
	rem_unpaired = rem_unpaired->next;
	prev_ptr     = &rem_unpaired;
	node2        = rem_unpaired;

	while (1) {
		/* pair nodes */
		*node1_jump = node2;
		node2->jump = node1;

		/* pop node2 from list */
		next      = node2->next;
		*prev_ptr = next;

		/* recurse */
		comb_pairs();

		/* re-insert node2 */
		*prev_ptr = node2;

		if (!next)
			break; /* none left */

		prev_ptr = &node2->next;
		node2    = next;
	}

	rem_unpaired = node1; /* restore rem_unpaired */
}


static inline void
solve(void)
{
	/* link nodes on same row (same y) by 'forward' */
	do {
		init_row(y_node->count);
	} while (++y_node < y_nodes_end);

	/* check each possible pairing */
	comb_pairs();
}


int
main(void)
{
	FILE *input;
	FILE *output;
	size_t x;
	size_t y;
	size_t i;

	assert(input = fopen("wormhole.in", "r"));
	assert(fscanf(input, "%zu\n", &N) == 1);

	i = N;
	do {
		assert(fscanf(input, "%zu %zu\n", &x, &y) == 2);
		update_map(y);
	} while (--i > 0);

	assert(fclose(input) == 0);

	solve();

	assert(output = fopen("wormhole.out", "w"));
	assert(fprintf(output, "%zu\n", loop_count) > 0);
	assert(fclose(output) == 0);
	return 0;
}
