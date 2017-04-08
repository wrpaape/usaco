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

#if 0 /* NOT ENOUGH MEMORY for 100M bits on stack or heap :( */
#define B_MAX		100000000
#define SIEVE_LENGTH	(((B_MAX + 1) / sizeof(unsigned int)) + 1)
static unsigned int sieve[SIEVE_LENGTH];
/* static unsigned int *restrict sieve; */
static inline unsigned int
is_composite(const unsigned int n)
{
	size_t i_div, i_rem;

	i_div = n / sizeof(sieve[0]);
	i_rem = n % sizeof(sieve[0]);

	return sieve[i_div] & (1 << i_rem);
}

static inline void
mark_composite(const unsigned int n)
{
	size_t i_div, i_rem;

	i_div = n / sizeof(sieve[0]);
	i_rem = n % sizeof(sieve[0]);

	sieve[i_div] |= (1 << i_rem);
}


static inline void
generate_primes(void)
{
	unsigned int prime, prime_sq, composite;
	/* assert(sieve = calloc(SIEVE_LENGTH, sizeof(*sieve))); */

	for (prime = 2; prime_sq = prime * prime, prime_sq <= B; ++prime) {
		if (is_composite(prime))
			continue;

		composite = prime_sq;
		do {
			mark_composite(composite);
			composite += prime;
		} while (composite <= B);
	}
}
#endif

inline bool
is_prime(const unsigned int n)
{
	unsigned int div, sqrt_n;

	if ((n & 1) == 0)
		return false; /* even */

	sqrt_n = (unsigned int) sqrt((double) n);

	/* check odds */
	for (div = 3; div <= sqrt_n; div += 2)
		if ((n % div) == 0)
			return false;

	return true;
}

void
inc_palindrome(const unsigned int acc,
	       const unsigned int inc_low,
	       const unsigned int inc_high)
{
	unsigned int next_inc_low, next_inc_high;
	unsigned int inc, inc_until;
	unsigned int next_acc, add_to_acc;

	if (inc_low > inc_high) {
		if (acc > B) {
			assert(fclose(output) == 0);
			/* free(sieve); */
			exit(0);
		}

		if ((acc >= A) && is_prime(acc))
			assert(fprintf(output, "%u\n", acc) > 0);

		return;
	}

	inc = inc_low;

	if (inc_high > inc_low)
		inc += inc_high;

	next_inc_high = inc_high / 10;
	inc_until     = inc_high * 10;
	next_inc_low  = inc_low  * 10;

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
	unsigned int add_to_acc, acc, next_acc;

	next_inc_high = 0;
	inc_high      = 1;

	while (1) {
		inc_until = inc_high * 10;

		if (inc_until > A)
			break;

		next_inc_high = inc_high;
		inc_high      = inc_until;
	}

	inc = inc_high + (A >= 10); /* 100...0001 ... 101, 11, 1 */
	assert(output = fopen("pprime.out", "w"));

	while (1) {
		acc        = inc;
		add_to_acc = 0;

		/* for all palindromes with same number of digits as 'inc' */
		do {
			next_acc = acc + add_to_acc;

			/* 10...01, 20...02, ... 90...09 */
			inc_palindrome(next_acc,
				       10,
				       next_inc_high);

			add_to_acc += inc;
		} while (add_to_acc < inc_until);

		next_inc_high  = inc_high;
		inc_high       = inc_until;
		inc_until     *= 10;
		inc            = inc_high + 1;
	}
}


int
main(void)
{
	FILE *input;

	assert(input = fopen("pprime.in", "r"));
	assert(fscanf(input, "%u %u\n", &A, &B) == 2);
	assert(fclose(input) == 0);

	/* generate_primes(); */

	check_palindromes();

	/* unreachable */
	return 0;
}
