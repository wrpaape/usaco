/*
ID: wrpaape1
LANG: C
TASK: nocows
*/

#include <stdio.h>
#include <assert.h>


static inline size_t
solve(unsigned int rem_cows,
      unsigned int rem_height)
{
	if (rem_height == 0)
		return (rem_cows == 0);

	size_t total_peds;

	--rem_cows;

}


int
main(void)
{
	FILE *input, *output;
	unsigned int N, K;

	assert(input = fopen("nocows.in", "r"));
	assert(fscanf(input, "%u %u\n", &N, &K) == 2);
	assert(fclose(input) == 0);

	const size_t total_peds = solve(N, K);

	assert(output = fopen("nocows.out", "w"));
	assert(fprintf(output, "%zu\n", total_peds % 9901) > 0);
	assert(fclose(output) == 0);

	return 0;
}
