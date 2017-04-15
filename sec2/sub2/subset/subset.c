/*
ID: wrpaape1
LANG: C
TASK: subset
*/

#include <stdio.h>
#include <assert.h>

#define N_MAX 39

static unsigned int N;
static unsigned int sum_half;

unsigned int
do_partition(unsigned int acc_sum,
	     unsigned int pick,
	     unsigned int pick_upto)
{
	unsigned int next_sum, count_even;

	if (pick_upto > N)
		return (acc_sum == sum_half);

	count_even = 0;
	++pick_upto;
	do {
		next_sum = acc_sum + pick;

		if (next_sum > sum_half)
			break;

		++pick;

		count_even += do_partition(next_sum,
					   pick,
					   pick_upto);
	} while (pick < pick_upto);

	return count_even;
}

int
main(void)
{
	FILE *input, *output;
	unsigned int n_p1, n_n_p1;
	unsigned int rem, group_size, max_group_size;
	unsigned int total_count_even, last_count_even;

	assert(input = fopen("subset.in", "r"));
	assert(fscanf(input, "%u\n", &N) == 1);
	assert(fclose(input) == 0);

	n_p1     = N + 1;
	n_n_p1   = N * n_p1;
	rem      = n_n_p1 % 4;

	total_count_even = 0;

	if (!rem) {
		/* sum of 1 ... N can be divided evenly in 2 */
		sum_half = n_n_p1 / 4;

		max_group_size = (N / 2);

		for (group_size = 1; group_size < max_group_size; ++group_size)
			total_count_even += do_partition(0, 1, n_p1 - group_size);

		last_count_even = do_partition(0, 1, n_p1 - max_group_size);


		/* if N is even, last paritition will return a double count
		 * of possible even splits */
		if ((N & 1) == 0)
			last_count_even /= 2;

		total_count_even += last_count_even;

	}

	assert(output = fopen("subset.out", "w"));
	assert(fprintf(output, "%u\n", total_count_even) > 0);
	assert(fclose(output) == 0);
	return 0;
}
