/*
ID: wrpaape1
LANG: C
TASK: combo
*/

#include <stdio.h>
#include <assert.h>
#define N_MAX 100

static inline unsigned int
within_tolerance(const unsigned int i,
		 const unsigned int num,
		 const unsigned int N)
{
	unsigned int large, small;
	unsigned int distance;

	if (i < num)
		small = i, large = num;
	else
		large = i, small = num;

	distance = large - small;

	if (distance <= 2)
		return 1;

	distance = (N - large) + small;

	return (distance <= 2);
}

static inline void
init_valid(unsigned int *const restrict valid,
	   const unsigned int num,
	   const unsigned int N)
{
	unsigned int i;

	i = 1;
	do {
		valid[i] = within_tolerance(i, num, N);
	} while (++i <= N);
}

static inline unsigned int
overlap(const unsigned int f,
	const unsigned int m,
	const unsigned int N)
{
	unsigned int valid_f[N_MAX + 1];
	unsigned int valid_m[N_MAX + 1];
	unsigned int i;
	unsigned int overlap;

	init_valid(&valid_f[0], f, N);
	init_valid(&valid_m[0], m, N);

	overlap = 0;
	i       = 1;

	do {
		overlap += (valid_f[i] & valid_m[i]);
	} while (++i <= N);

	return overlap;
}


int
main(void)
{
	FILE *input;
	FILE *output;
	unsigned int combo_count;
	unsigned int N;
	unsigned int f1, f2, f3;
	unsigned int m1, m2, m3;
	unsigned int o1, o2, o3;

	assert(input = fopen("combo.in", "r"));
	assert(fscanf(input,
		      "%u\n%u %u %u\n%u %u %u\n",
		      &N,
		      &f1, &f2, &f3,
		      &m1, &m2, &m3) == 7);
	assert(fclose(input) == 0);

	if (N <= 5) {
		combo_count = N * N * N; /* all combos work, overlap */

	} else {
		/* start with total combos, ignore overlaps */
		combo_count = 2 * 5 * 5 * 5;

		o1 = overlap(f1, m1, N);
		o2 = overlap(f2, m2, N);
		o3 = overlap(f3, m3, N);

		combo_count -= o1 * o2 * o3;
	}

	assert(output = fopen("combo.out", "w"));
	assert(fprintf(output, "%u\n", combo_count) > 0);
	assert(fclose(output) == 0);
	return 0;
}
