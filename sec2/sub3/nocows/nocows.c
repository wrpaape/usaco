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


static int lookup[K_MAX + 1][N_MAX + 1];

static unsigned int
solve(const unsigned int rem_height,
      const unsigned int rem_cows);


static inline unsigned int
do_solve(unsigned int rem_height,
	 unsigned int rem_cows)
{
	--rem_cows; // place 1 root cow
	if (rem_cows <= 1)
		return (rem_cows == 0); // placed all cows OR 1 extra child

	--rem_height; // reduce remaining tree height
	if (rem_height == 0)
		return 0; // out of height, cows remain (no solution)

	unsigned int peds_left;
	unsigned int peds_right;
	unsigned int total_peds = 0;
	unsigned int rem_cows_left  = 1;
	unsigned int rem_cows_right = rem_cows - 1;

	while (1) {
		peds_left = solve(rem_height,
				  rem_cows_left);

		if (rem_cows_left == rem_cows_right) {
			total_peds += ((peds_left * peds_left) % 9901);
			total_peds %= 9901;
			return total_peds;
		}

		peds_right = solve(rem_height,
				   rem_cows_right);

		total_peds += ((peds_left * peds_right * 2) % 9901);
		total_peds %= 9901;

		++rem_cows_left;

		if (rem_cows_left == rem_cows_right)
			return total_peds;

		--rem_cows_right;
	}
}

static unsigned int
solve(unsigned int rem_height,
      unsigned int rem_cows)
{

	int *const cache_ptr = &lookup[rem_height][rem_cows];
	int cached           = *cache_ptr;

	if (cached < 0) {
		cached = (int) do_solve(rem_height,
					rem_cows);
		*cache_ptr = cached;
	}

	return (unsigned int) cached;
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
	const size_t size_of_n_range = sizeof(int) * (N + 1);

	for (unsigned int k = 0; k <= K; ++k)
		(void) memset(&lookup[k][0],
			      -1, // safe, 2s compliment all 1's
			      size_of_n_range);

	const unsigned int total_peds_mod_9901 = solve(K, N);

	printf("total_peds_mod_9901: %u\n", total_peds_mod_9901);

	assert(output = fopen("nocows.out", "w"));
	assert(fprintf(output, "%u\n", total_peds_mod_9901) > 0);
	assert(fclose(output) == 0);

	return 0;
}
