/*
ID: wrpaape1
LANG: C
TASK: subset
*/

#include <stdio.h>
#include <assert.h>

#define N_MAX		39
#define TARGET_MAX	((N_MAX * (N_MAX + 1)) / 4)


static long long cache[N_MAX + 1][TARGET_MAX + 1];

long long
count_combinations(const long long n_max,
		   const long long target);

static inline long long
solve_combinations(const long long n_max,
		   const long long target)
{
	long long n_max_m1;

	if (n_max >= target) /* ignore range [target + 1, n_max] */
		return 1 + count_combinations(target - 1, target);

	n_max_m1 = n_max - 1;

	return count_combinations(n_max_m1, target - n_max)
	     + count_combinations(n_max_m1, target);
}

long long
count_combinations(const long long n_max,
		   const long long target)
{
	long long *restrict cache_ptr;
	long long cached_solution;

	cache_ptr       = &cache[n_max][target];
	cached_solution = *cache_ptr;

	if (cached_solution < 0) {
		cached_solution = solve_combinations(n_max, target);
		*cache_ptr      = cached_solution;
	}

	return cached_solution;
}

static inline long long
solve(const long long sum_1_to_N_div_2,
      const long long N)
{
	long long n, target;
	for (n = 1; n <= N; ++n)
		for (target = 1; target <= sum_1_to_N_div_2; ++target)
			cache[n][target] = -1;

	/* leave cache[0...N][0] and cache[0][0...target] == 0 */

	return count_combinations(N, sum_1_to_N_div_2) / 2;
}

int
main(void)
{
	FILE *input, *output;
	long long N, n_n_p1, rem, total_count;

	assert(input = fopen("subset.in", "r"));
	assert(fscanf(input, "%lld\n", &N) == 1);
	assert(fclose(input) == 0);

	n_n_p1   = N * (N + 1);
	rem      = n_n_p1 % 4;

	/* if sum of 1 ... N cannot be divided evenly in 2 -> 0 possible
	 * partions, else call 'solve' */
	total_count = rem ? 0 : solve(n_n_p1 / 4, N);

	assert(output = fopen("subset.out", "w"));
	assert(fprintf(output, "%lld\n", total_count) > 0);
	assert(fclose(output) == 0);
	return 0;
}
