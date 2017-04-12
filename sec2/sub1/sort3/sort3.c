/*
ID: wrpaape1
LANG: C
TASK: sort3
*/

#include <stdio.h>
#include <assert.h>

#define N_MAX 1000

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int
main(void)
{

	FILE *input, *output;
	unsigned int N;
	unsigned int sequence[N_MAX];
	unsigned int *restrict ptr;
	unsigned int *restrict ptr_1;
	unsigned int *restrict ptr_2;
	unsigned int *restrict ptr_3;
	unsigned int *restrict ptr_end;
	unsigned int count_1, count_2, val;
	unsigned int count_1_in_1, count_2_in_1, count_3_in_1;
	unsigned int count_1_in_2, count_2_in_2, count_3_in_2;
	unsigned int count_1_in_3, count_2_in_3;
	unsigned int min_count;
	unsigned int swap_count;
	unsigned int rem_out_of_place;

	/* initial scan, get count of 1s and 2s
	 *
	 * split sequence into 3 ranges
	 * 1: [ptr_1, ptr_2)   of length 'count_1'
	 * 2: [ptr_2, ptr_3)   of length 'count_2'
	 * 3: [ptr_3, ptr_end) of length 'N - count_1 - count_2' */
	assert(input = fopen("sort3.in", "r"));
	assert(fscanf(input, "%u\n", &N) == 1);

	ptr_1   = &sequence[0];
	ptr_end = ptr_1 + N;

	count_1 = count_2 = 0;

	ptr = ptr_1;
	do {
		assert(fscanf(input, "%u\n", ptr) == 1);
		val = *ptr;
		count_1 += (val == 1);
		count_2 += (val == 2);
	} while (++ptr < ptr_end);

	assert(fclose(input) == 0);

	ptr_2 = ptr_1 + count_1;
	ptr_3 = ptr_2 + count_2;

	/* count out of place in range 1 (2s and 3s) */
	count_2_in_1 = count_3_in_1 = 0;
	for (ptr = ptr_1; ptr < ptr_2; ++ptr) {
		val = *ptr;
		count_2_in_1 += (val == 2);
		count_3_in_1 += (val == 3);
	}
	count_1_in_1 = count_1 - (count_2_in_1 + count_3_in_1);

	/* count out of place in range 2 (1s and 3s) */
	count_1_in_2 = count_3_in_2 = 0;
	while (ptr < ptr_3) {
		val = *ptr++;
		count_1_in_2 += (val == 1);
		count_3_in_2 += (val == 3);
	}
	count_2_in_2 = count_2 - (count_1_in_2 + count_3_in_2);

	/* count out of place in range 3 (1s and 2s) */
	count_1_in_3 = count_1 - (count_1_in_1 + count_1_in_2);
	count_2_in_3 = count_2 - (count_2_in_1 + count_2_in_2);


	/* first make as many 1 swap -> 2 in place swaps as possible */

	/* swap as many 1s in 3 with 3s in 1 as possible */
	min_count         = MIN(count_1_in_3, count_3_in_1);
	count_1_in_3     -= min_count;
	count_3_in_1     -= min_count;
	swap_count        = min_count;
	rem_out_of_place  = (count_1_in_3 + count_3_in_1);


	/* swap as many 1s in 2 with 2s in 1 as possible */
	min_count         = MIN(count_1_in_2, count_2_in_1);
	count_1_in_2     -= min_count;
	count_2_in_1     -= min_count;
	swap_count       += min_count;
	rem_out_of_place += (count_1_in_2 + count_2_in_1);


	/* swap as many 2s in 3 with 3s in 2 as possible */
	min_count         = MIN(count_2_in_3, count_3_in_2);
	count_2_in_3     -= min_count;
	count_3_in_2     -= min_count;
	swap_count       += min_count;
	rem_out_of_place += (count_2_in_3 + count_3_in_2);


	/* remaining out of place: 2 swaps -> puts 3 in place */
	swap_count += ((rem_out_of_place * 2) / 3);


	assert(output = fopen("sort3.out", "w"));
	assert(fprintf(output, "%u\n", swap_count));
	assert(fclose(output) == 0);

	return 0;
}
