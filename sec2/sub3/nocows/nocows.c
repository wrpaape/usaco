/*
ID: wrpaape1
LANG: C
TASK: nocows
*/

#include <stdio.h>
#include <assert.h>

#define N_MAX 200
#define K_MAX 100


static unsigned long long lookup[K_MAX + 1][N_MAX + 1];

static unsigned long long
solve(const unsigned long long rem_height,
      const unsigned long long rem_cows);


static inline unsigned long long
do_solve(unsigned long long rem_height,
	 unsigned long long rem_cows)
{
	if (rem_cows == 0)
		return 1; // successfully placed all cows

	if (rem_height == 0)
		return 0; // out of height, cows remain (no solution)

	--rem_height; // reduce remaining tree height
	--rem_cows;   // place 1 root cow

	unsigned long long peds_left;
	unsigned long long peds_right;
	unsigned long long total_peds     = 0;
	unsigned long long rem_cows_left  = 0;
	unsigned long long rem_cows_right = rem_cows;


	while (1) {
		peds_left = solve(rem_height,
				  rem_cows_left);

		if (rem_cows_left == rem_cows_right)
			return total_peds + (peds_left * peds_left);

		++rem_cows_left;

		peds_right = solve(rem_height,
				   rem_cows_right);

		total_peds += (peds_left * peds_right * 2);

		if (rem_cows_left == rem_cows_right)
			return total_peds;

		--rem_cows_right;
	}
}

static unsigned long long
solve(unsigned long long rem_height,
      unsigned long long rem_cows)
{

	unsigned long long *const cache_ptr = &lookup[rem_height][rem_cows];
	unsigned long long cached           = *cache_ptr;

	if (cached == 0) {
		cached = do_solve(rem_height,
				  rem_cows);
		*cache_ptr = cached;
	}

	return cached;
}


int
main(void)
{
	FILE *input, *output;
	unsigned long long N, K;

	assert(input = fopen("nocows.in", "r"));
	assert(fscanf(input, "%llu %llu\n", &N, &K) == 2);
	assert(fclose(input) == 0);

	const unsigned long long total_peds = solve(K, N);

	assert(total_peds >= 0);

	printf("total_peds: %llu\n", total_peds);
	printf("total_peds %% 9901: %llu\n", total_peds % 9901);

	assert(output = fopen("nocows.out", "w"));
	assert(fprintf(output, "%llu\n", total_peds % 9901) > 0);
	assert(fclose(output) == 0);

	return 0;
}
