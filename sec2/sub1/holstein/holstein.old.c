/*
ID: wrpaape1
LANG: C
TASK: holstein
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

#define V_MAX 25
#define G_MAX 15

static int V;
static int vitamins[V_MAX];

static int G;
static int feeds[G_MAX][V_MAX];
static bool is_taken[G_MAX];

static int min_scoops;
static int min_solution[V_MAX];


void
solve(const int rem_vitamins,
      const int current_scoop)
{
	static int current_solution[V_MAX];
	int next_scoop, feed, v, prev_rem, next_rem;
	int *restrict feed_ptr;
	int *restrict rem_ptr;
	int next_rem_vitamins;

	if (rem_vitamins == 0) {
		(void) memcpy(&min_solution[0],
			      &current_solution[0],
			      sizeof(min_solution[0]) * current_scoop);
		min_scoops = current_scoop;
		return;
	}

	next_scoop = current_scoop + 1;

	if (next_scoop >= min_scoops)
		return;

	for (feed = 0; feed < G; ++feed) {
		if (is_taken[feed])
			continue;

		is_taken[feed] = true;

		current_solution[current_scoop] = feed;

		feed_ptr = &feeds[feed][0];

		next_rem_vitamins = rem_vitamins;

		for (v = 0; v < V; ++v) {
			rem_ptr  = &vitamins[v];
			prev_rem = *rem_ptr;
			next_rem = prev_rem - feed_ptr[v];
			*rem_ptr = next_rem;

			next_rem_vitamins -= ((prev_rem > 0) & (next_rem <= 0));
		}

		solve(next_rem_vitamins, next_scoop);

		for (v = 0; v < V; ++v)
			vitamins[v] += feed_ptr[v];

		is_taken[feed] = false;
	}
}

int
main(void)
{
	FILE *input, *output;
	int v, g, scoop;

	assert(input = fopen("holstein.in", "r"));
	assert(fscanf(input, "%d\n", &V) == 1);
	for (v = 0; v < V; ++v)
		assert(fscanf(input, "%d\n", &vitamins[v]) == 1);

	assert(fscanf(input, "%d\n", &G) == 1);
	for (g = 0; g < G; ++g)
		for (v = 0; v < V; ++v)
			assert(fscanf(input, "%d\n", &feeds[g][v]) == 1);
	assert(fclose(input) == 0);

	min_scoops = V + 1;

	solve(V, 0);

	assert(output = fopen("holstein.out", "w"));
	assert(fprintf(output, "%d", min_scoops) > 0);
	for (scoop = 0; scoop < min_scoops; ++scoop)
		assert(fprintf(output, " %d", min_solution[scoop] + 1) > 0);
	assert(fputs("\n", output) != EOF);
	assert(fclose(output) == 0);

	return 0;
}
