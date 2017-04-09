/*
ID: wrpaape1
LANG: C
TASK: sprime
*/

#include <stdio.h>
#include <assert.h>
#include <math.h>

static FILE *output;
static unsigned int rem_digits;

void
solve(unsigned int acc);

static inline void
solve_if_prime(const unsigned int acc)
{
	unsigned int sqrt_acc, div;

	/* ignore 1, 2, even numbers (not given as input) */
	sqrt_acc = (unsigned int) sqrt((double) acc);

	/* check all odds, min acc is 21, safe to do while */
	div = 3;
	do {
		if ((acc % div) == 0)
			return; /* composite */

		div += 2;
	} while (div <= sqrt_acc);

	solve(acc); /* acc is prime, recurse */
}

void
solve(unsigned int acc) /* acc -- known prime */
{
	if (rem_digits == 0) { /* acc is N digits long superprime */
		assert(fprintf(output, "%u\n", acc) > 0);
		return;
	}

	/* keep rem_digits in static memory, no need to have copy on every stack
	 * frame for DFS */
	--rem_digits;

	acc *= 10; /* shift up */

	/* check all odds except 5 in ones place (always non-prime) */
	solve_if_prime(acc + 1);
	solve_if_prime(acc + 3);
	solve_if_prime(acc + 7);
	solve_if_prime(acc + 9);

	++rem_digits;
}


int
main(void)
{
	FILE *input;

	assert(input = fopen("sprime.in", "r"));
	assert(fscanf(input, "%u\n", &rem_digits) == 1);
	assert(fclose(input) == 0);

	assert(output = fopen("sprime.out", "w"));

	/* check all prime single digits */
	--rem_digits;

	solve(2);
	solve(3);
	solve(5);
	solve(7);

	assert(fclose(output) == 0);
	return 0;
}
