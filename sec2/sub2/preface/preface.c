/*
ID: wrpaape1
LANG: C
TASK: preface
*/

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define N_MAX		3500
#define DIGIT_COUNT	7

static const char digit_display[] = {
	'I', 'V', 'X', 'L', 'C', 'D', 'M'
};

static unsigned int total_counts[DIGIT_COUNT];

static inline int
abs(const int x)
{
	return (x < 0) ? -x : x;
}

static inline void
decompose(unsigned int n)
{
	unsigned int i, rem;
	unsigned int big_ten, lil_ten;
	int count;

	i       = 0;
	lil_ten = 1;

	do {
		big_ten  = lil_ten * 10;
		rem      = n % big_ten;
		n       -= rem;
		count    = rem / lil_ten;

		/* rem is expressable as ... */
		/* 0, 1, 2, or 3 X lil_ten  */
		if (count < 4) {
			total_counts[i] += count;
			i += 2;

		/* 4, 5, 6, 7, or 8 X lil_ten  */
		} else if (count < 9) {
			total_counts[i++] += abs(count - 5);
			++total_counts[i++]; /* add 1 to multiple of 5 */

		/* 9 X lil_ten */
		} else {
			++total_counts[i];
			i += 2;
			++total_counts[i];
		}

		lil_ten = big_ten;
	} while (n > 0);

}


int
main(void)
{
	FILE *input, *output;
	unsigned int i, n, N, count;


	assert(input = fopen("preface.in", "r"));
	assert(fscanf(input, "%u\n", &N) == 1);
	assert(fclose(input) == 0);

	for (n = 1; n <= N; ++n)
		decompose(n);

	assert(output = fopen("preface.out", "w"));
	for (i = 0; i < DIGIT_COUNT; ++i) {
		count = total_counts[i];

		if (count > 0)
			assert(fprintf(output, "%c %u\n",
				       digit_display[i],
				       count) > 0);
	}
	assert(fclose(output) == 0);
}

