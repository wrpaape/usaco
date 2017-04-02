/*
ID: wrpaape1
LANG: C
TASK: wormhole
*/

#include <stdio.h>
#include <assert.h>
#define N_MAX 12


static int N;          /* count of wormholes */
static int loop_count; /* solution */

/* wormholes are recorded by integer id in the range [1, N] */

/* jump_to[worm1] = worm2, jump_to[worm2] = worm1 */
static int jump_to[N_MAX + 1];

/* walk_to[worm1] = worm2 (worm2 == 0 if end of row) */
static int walk_to[N_MAX + 1];

/* y_coord[worm] = y */
static int y_coord[N_MAX + 1];

/* is_paired[worm] = 1 if is paired, 0 if available */
static int is_paired[N_MAX + 1];


static inline void
check_loop(void)
{
	int start_worm, worm, step;

	for (start_worm = 1; start_worm <= N; ++start_worm) {
		worm = start_worm;

		for (step = 1; step <= N; ++step)
			worm = walk_to[jump_to[worm]]; /* jump -> walk +x */

		if (worm != 0) {
			/* if didn't get stuck at NULL worm
			 * do to walking off of a row, loop
			 * if found -> increment solution */
			++loop_count;
			return;
		}
	}
}


void
solve(int rem_pairs)
{
	int worm1, worm2;

	if (rem_pairs == 0) {
		check_loop(); /* no more pairing */
		return;
	}

	--rem_pairs;

	for (worm1 = 1; is_paired[worm1]; ++worm1)
		; /* set worm1 to 1st unpaired */

	is_paired[worm1] = 1; /* mark worm1 */

	for (worm2 = worm1 + 1; worm2 <= N; ++worm2) {
		if (is_paired[worm2])
			continue;

		is_paired[worm2] = 1; /* mark worm2 */

		/* pair worm1 with worm2 */
		jump_to[worm1] = worm2;
		jump_to[worm2] = worm1;

		solve(rem_pairs);

		is_paired[worm2] = 0; /* unmark worm2 */
	}

	is_paired[worm1] = 0; /* unmark worm1 */
}


int
main(void)
{
	FILE *input;
	FILE *output;
	int x; /* ignored */
	int y1, y2;
	int worm, worm1, worm2;

	assert(input = fopen("wormhole.in", "r"));
	assert(fscanf(input, "%d\n", &N) == 1);

	for (worm = 1; worm <= N; ++worm)
		assert(fscanf(input, "%d %d\n", &x, &y_coord[worm]) == 2);

	assert(fclose(input) == 0);

	for (worm1 = 1; worm1 <= N; ++worm1) {
		y1 = y_coord[worm1];

		for (worm2 = worm1 + 1; worm2 <= N; ++worm2) {
			y2 = y_coord[worm2];

			if (y1 == y2) {
				walk_to[worm1] = worm2; /* same row -> link */
				break;
			}
		}
	}

	solve(N / 2); /* N / 2 available pairs */

	assert(output = fopen("wormhole.out", "w"));
	assert(fprintf(output, "%d\n", loop_count) > 0);
	assert(fclose(output) == 0);
	return 0;
}
