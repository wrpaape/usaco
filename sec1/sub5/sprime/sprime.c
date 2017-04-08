/*
ID: wrpaape1
LANG: C
TASK: sprime
*/

#include <stdio.h>
#include <assert.h>
#include <math.h>

FILE *output;

static inline unsigned int
is_prime(const unsigned int acc)
{
	unsigned int sqrt_acc, div;

	/* ignore 1, 2, even numbers (not given as input) */
	sqrt_acc = (unsigned int) sqrt((double) acc);

	/* check all odds, min acc is 21, safe to do while */
	div = 3;
	do {
		if ((acc % div) == 0)
			return 0;

		div += 2;
	} while (div <= sqrt_acc);

	return 1;
}

void
solve(unsigned int acc,        /* known prime */
      unsigned int rem_digits) /* remaining digits to append */
{
	unsigned int next_acc;

	if (rem_digits == 0) {
		assert(fprintf(output, "%u\n", acc) > 0);
		return;
	}

	--rem_digits;
	acc *= 10; /* shift up */

#define CHECK_PRIME(INC)						\
	next_acc = acc + INC;						\
	if (is_prime(next_acc))						\
		solve(next_acc, rem_digits)
	/* check all odds except 5 in ones place (always non-prime) */
	CHECK_PRIME(1);
	CHECK_PRIME(3);
	CHECK_PRIME(7);
	CHECK_PRIME(9);
#undef CHECK_PRIME
}

int
main(void)
{
	FILE *input;
	unsigned int N;

	assert(input = fopen("sprime.in", "r"));
	assert(fscanf(input, "%u\n", &N) == 1);
	assert(fclose(input) == 0);

	assert(output = fopen("sprime.out", "w"));

	/* check all prime single digits */
	--N;
	solve(2, N);
	solve(3, N);
	solve(5, N);
	solve(7, N);

	assert(fclose(output) == 0);
	return 0;
}
