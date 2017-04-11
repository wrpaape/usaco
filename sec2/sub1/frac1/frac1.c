/*
ID: wrpaape1
LANG: C
TASK: frac1
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


/* 1 + 2 ... + N = (N + 1) * N / 2 */
#define N_MAX		160
#define FRAC_COUNT_MAX	(((N_MAX + 1) * N_MAX) / 2)


struct Frac {
	unsigned short numerator;
	unsigned short denominator;
};


static inline unsigned int
greatest_common_divisor(unsigned int a,
			unsigned int b)
{
	unsigned int min_shift, shift_a, shift_b;

	shift_a = __builtin_ctz(a);
	shift_b = __builtin_ctz(b);

	min_shift = (shift_a < shift_b) ? shift_a : shift_b;

	a >>= shift_a;
	b >>= shift_b;

	while (a != b)
		if (a > b) {
			a  -= b;
			a >>= __builtin_ctz(a);
		} else {
			b  -= a;
			b >>= __builtin_ctz(b);
		}

	return a << min_shift;
}

int
order_asc(const void *key1,
	  const void *key2)
{
	const struct Frac *restrict frac1;
	const struct Frac *restrict frac2;
	int num1, num2, den1, den2;
	int num1_den2, num2_den1;

	frac1 = (const struct Frac *) key1;
	num1  = (int) frac1->numerator;
	den1  = (int) frac1->denominator;

	frac2 = (const struct Frac *) key2;
	num2  = (int) frac2->numerator;
	den2  = (int) frac2->denominator;

	num1_den2 = num1 * den2;
	num2_den1 = num2 * den1;

	return num1_den2 - num2_den1;
}


int
main(void)
{
	FILE *input, *output;
	struct Frac fracs[FRAC_COUNT_MAX];
	struct Frac *restrict fracs_begin;
	struct Frac *restrict frac;
	const struct Frac *restrict fracs_end;
	unsigned int N;

	assert(input = fopen("frac1.in", "r"));
	assert(fscanf(input, "%u\n", &N) == 1);
	assert(fclose(input) == 0);

	fracs_begin = &fracs[0];
	frac        = fracs_begin;

	for (unsigned int den = 2; den <= N; ++den)
		for (unsigned int num = 1; num < den; ++num)
			if (greatest_common_divisor(num, den) == 1) {
				frac->numerator   = (unsigned short) num;
				frac->denominator = (unsigned short) den;
				++frac;
			}

	fracs_end = frac;

	qsort(fracs_begin,
	      fracs_end - fracs_begin,
	      sizeof(*fracs_begin),
	      &order_asc);

	assert(output = fopen("frac1.out", "w"));
	assert(fputs("0/1\n", output) != EOF);

	for (frac = fracs_begin; frac < fracs_end; ++frac)
		assert(fprintf(output, "%hu/%hu\n",
			       frac->numerator,
			       frac->denominator) > 0);

	assert(fputs("1/1\n", output) != EOF);
	assert(fclose(output) == 0);

	return 0;
}


