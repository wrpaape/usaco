/*
ID: wrpaape1
LANG: C
TASK: pprime
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


static unsigned int A, B;
static FILE *output;

inline bool
is_prime(const unsigned int n)
{
	unsigned int div, sqrt_n;

	/* no need to check even, skipped in 'check_palindromes' */
	/* if ((n & 1) == 0) */
	/* 	return false; /1* even *1/ */

	sqrt_n = (unsigned int) sqrt((double) n);

	/* check odds */
	for (div = 3; div <= sqrt_n; div += 2)
		if ((n % div) == 0)
			return false;

	return true;
}

void
inc_palindrome(const unsigned int acc,	     /* accumulating palindrome */
	       const unsigned int inc_low,   /* leftmost inc digit (i.e. 10000) */
	       const unsigned int inc_high)  /* rightmost inc digit (i.e. 100) */
{
	unsigned int next_inc_low, next_inc_high;
	unsigned int inc, inc_until;
	unsigned int next_acc, add_to_acc;

	if (inc_low > inc_high) { /* middle has been reached, acc is complete */
		if (acc > B) {
			assert(fclose(output) == 0);
			exit(0);
		}

		if ((acc >= A) && is_prime(acc))
			assert(fprintf(output, "%u\n", acc) > 0);

		return;
	}

	inc = inc_low;

	if (inc_high > inc_low)
		inc += inc_high;

	/* inc of the form 10...01, 101, 11, or 1 */

	next_inc_high = inc_high / 10; /* move leftmost digit right */
	inc_until     = inc_high * 10; /* stop when add_to_acc gains a digit */
	next_inc_low  = inc_low  * 10; /* move rightmost digit left */

	add_to_acc = 0;

	do {
		next_acc = acc + add_to_acc;

		inc_palindrome(next_acc,
			       next_inc_low,
			       next_inc_high);

		add_to_acc += inc;
	} while (add_to_acc < inc_until);
}


static inline void
check_palindromes(void)
{
	unsigned int inc, inc_high, inc_until, next_inc_high;
	unsigned int acc;

	next_inc_high = 0;
	inc_high      = 1;

	while (1) {
		inc_until = inc_high * 10;

		if (inc_until > A)
			break;

		next_inc_high = inc_high;
		inc_high      = inc_until;
	}

	acc = inc_high + (A >= 10); /* 100...0001 ... 101, 11, 1 */
	assert(output = fopen("pprime.out", "w"));

	while (1) {
		/* skip even palindromes... */
		inc = acc * 2; /* inc by 20...02 */

		/* for all palindromes with same number of digits as 'acc' */
		do {
			/* increment outer digits and work toward interior */
			/* 10...01, 30...03, ..., 70...07, 90...09 */
			inc_palindrome(acc,
				       10,
				       next_inc_high);

			acc += inc;
		} while (acc < inc_until);

		next_inc_high  = inc_high;
		inc_high       = inc_until;
		inc_until     *= 10;
		acc            = inc_high + 1; /* start at  10..01 */
	}
}


int
main(void)
{
	FILE *input;

	assert(input = fopen("pprime.in", "r"));
	assert(fscanf(input, "%u %u\n", &A, &B) == 2);
	assert(fclose(input) == 0);

	check_palindromes();

	/* unreachable */
	return 0;
}
