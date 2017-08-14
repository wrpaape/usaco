/*
ID: wrpaape1
LANG: C
TASK: nocows
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>

#define N_MIN 3
#define N_MAX 199
#define K_MIN 2
#define K_MAX 99


struct Solution
{
	short complete;
	short incomplete;
};

static struct Solution lookup[K_MAX + 1][N_MAX + 1];

static const struct Solution *
solve(const unsigned int rem_height,
      const unsigned int rem_cows);


static inline void
do_solve(unsigned int rem_height,
	 unsigned int rem_cows,
	 struct Solution *const solution)
{
	--rem_cows;   // place 1 root cow
	--rem_height; // reduce remaining tree height

	const unsigned int out_of_height = (rem_height == 0);

	if (rem_cows <= 1) {
		// placed all cows OR 1 extra child
		const unsigned int out_of_cows = (rem_cows == 0);
		solution->complete   = out_of_cows & out_of_height;
		solution->incomplete = out_of_cows & !out_of_height;
		return;
	}

	if (out_of_height) {
		// out of height, cows remain (no solution)
		solution->complete   = 0;
		solution->incomplete = 0;
		return;
	}

	const struct Solution *peds_left;
	const struct Solution *peds_right;
	unsigned int complete_left;
	unsigned int incomplete_left;
	unsigned int complete_right;
	unsigned int incomplete_right;

	unsigned int total_complete   = 0;
	unsigned int total_incomplete = 0;
	unsigned int rem_cows_left    = 1;
	unsigned int rem_cows_right   = rem_cows - 1;

	while (1) {
		peds_left 	= solve(rem_height,
				       	rem_cows_left);
		complete_left   = peds_left->complete;
		incomplete_left = peds_left->incomplete;

		if (rem_cows_left == rem_cows_right) {
			total_complete += ((complete_left * complete_left) % 9901);
			total_complete %= 9901;

			total_complete += ((incomplete_left * complete_left) % 9901);
			total_complete %= 9901;

			total_incomplete += ((incomplete_left * incomplete_left) % 9901);
			total_incomplete %= 9901;
			break;
		}

		peds_right 	 = solve(rem_height,
					 rem_cows_right);
		complete_right   = peds_right->complete;
		incomplete_right = peds_right->incomplete;

		total_complete += ((complete_left * complete_right * 2) % 9901);
		total_complete %= 9901;

		total_complete += ((incomplete_left * complete_right * 2) % 9901);
		total_complete %= 9901;

		total_complete += ((complete_left * incomplete_right * 2) % 9901);
		total_complete %= 9901;

		total_incomplete += ((incomplete_left * incomplete_right * 2) % 9901);
		total_incomplete %= 9901;

		++rem_cows_left;

		if (rem_cows_left == rem_cows_right)
			break;

		--rem_cows_right;
	}

	solution->complete   = total_complete;
	solution->incomplete = total_incomplete;
}


static const struct Solution *
solve(unsigned int rem_height,
      unsigned int rem_cows)
{

	struct Solution *const solution = &lookup[rem_height][rem_cows];

	if (solution->complete < 0) {
		do_solve(rem_height,
			 rem_cows,
			 solution);
	}

	return solution;
}


int
main(void)
{
	FILE *input, *output;
	unsigned int N, K;

	assert(input = fopen("nocows.in", "r"));
	assert(fscanf(input, "%u %u\n", &N, &K) == 2);
	assert(fclose(input) == 0);

	// set those below [N|K]_MIN for trivial cases
	const size_t size_of_n_range = sizeof(lookup[0][0]) * (N + 1);

	for (unsigned int k = 0; k <= K; ++k)
		(void) memset(&lookup[k][0],
			      -1, // safe, 2s compliment all 1's
			      size_of_n_range);

	const struct Solution *const solution = solve(K, N);

	assert(output = fopen("nocows.out", "w"));
	assert(fprintf(output, "%hd\n", solution->complete) > 0);
	assert(fclose(output) == 0);

	return 0;
}
