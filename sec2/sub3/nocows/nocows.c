/*
ID: wrpaape1
LANG: C
TASK: nocows
*/

#include <stdio.h>
#include <assert.h>

#define N_MAX 200
#define K_MAX 100


static unsigned int lookup[K_MAX + 1][N_MAX + 1];

static unsigned int
solve(const unsigned int rem_height,
      const unsigned int rem_cows);


static inline unsigned int
do_solve(unsigned int rem_height,
	 unsigned int rem_cows)
{
	if (rem_cows == 0)
		return 1; // successfully placed all cows

	if (rem_height == 0)
		return 0; // out of height, cows remain

	--rem_height; // reduce remaining tree height
	--rem_cows;   // place 1 root cow

	unsigned int odd_peds;

	unsigned int total_peds     = 0;
	unsigned int rem_cows_left  = 0;
	unsigned int rem_cows_right = rem_cows;


	while (1) {
		odd_peds = solve(rem_height,
				 rem_cows_left);

		if (rem_cows_left == rem_cows_right)
			return (total_peds * 2) + odd_peds;

		++rem_cows_left;

		total_peds += odd_peds;
		total_peds += solve(rem_height,
				    rem_cows_right);

		if (rem_cows_left == rem_cows_right)
			return total_peds * 2;

		--rem_cows_right;
	}
}

static unsigned int
solve(unsigned int rem_height,
      unsigned int rem_cows)
{

	unsigned int *const cache_ptr = &lookup[rem_height][rem_cows];
	unsigned int cached    	      = *cache_ptr;

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
	unsigned int N, K;

	assert(input = fopen("nocows.in", "r"));
	assert(fscanf(input, "%u %u\n", &N, &K) == 2);
	assert(fclose(input) == 0);

	const unsigned int total_peds = solve(K, N);

	assert(total_peds >= 0);

	assert(output = fopen("nocows.out", "w"));
	assert(fprintf(output, "%u\n", total_peds % 9901) > 0);
	assert(fclose(output) == 0);

	return 0;
}
