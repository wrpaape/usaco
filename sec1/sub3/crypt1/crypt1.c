/*
ID: wrpaape1
LANG: C
TASK: crypt1
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdio.h>   /* fdopen, fdclose, fscanf */
#include <stdbool.h> /* bool */
#include <stdlib.h>  /* qsort, exit */
#include <assert.h>  /* assert */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"crypt1"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define MAX_STALLS	200


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static size_t count_digits;
static bool digit_set[10];
static unsigned int digits[9];
static unsigned int *const restrict digits_begin = &digits[0];
static unsigned int *restrict digits_end;
static unsigned int count_solutions;

static inline void
write_output(void)
{
	FILE *output;
	assert(output = fopen(OUTPUT_FILE, "w"));
	assert(fprintf(output, "%u\n", count_solutions) >= 0);
	assert(fclose(output) == 0);
	exit(0);
}

static inline void
read_input(void)
{
	FILE *input;
	unsigned int digit;
	unsigned int *restrict digit_ptr;

	assert(input = fopen(INPUT_FILE, "r"));

	/* read in count digits */
	assert(fscanf(input, "%zu\n", &count_digits) == 1);

	/* populate list and set */
	if (count_digits == 0)
		write_output();

	digits_end = digits_begin + count_digits;
	digit_ptr  = digits_begin;

	do {
		assert(fscanf(input, "%u\n", &digit) == 1);
		digit_set[digit] = true;
		*digit_ptr++     = digit;
	} while (digit_ptr < digits_end);

	assert(fclose(input) == 0);

}

int
order_asc(const void *key1,
	  const void *key2)
{
	return (*((unsigned int *) key1)) - (*((unsigned int *) key2));
}

static inline bool
check_digits(unsigned int num)
{
	unsigned int digit;

	do {
		digit = num % 10;
		num  /= 10;

		if (!digit_set[digit])
			return false;

	} while (num > 0);

	return true;
}

static inline bool
perm_m2(const unsigned int m1)
{
	unsigned int *i, *j;
	unsigned int m2_a, m2_b, m2;
	unsigned int p1, p2, prod;
	bool valid_perms;

	valid_perms = false;

	i = digits_begin;
	do {
		m2_a = *i;
		p1   = m2_a * m1;

		if (p1 > 999)
			break;

		if (!check_digits(p1))
			continue;

		m2_a *= 10;

		j = digits_begin;
		do {
			m2_b = *j;
			p2   = m2_b * m1;

			if (p2 > 999)
				break;

			if (!check_digits(p2))
				continue;

			m2   = m2_a + m2_b;
			prod = m2 * m1;

			if (prod > 9999)
				break;

			valid_perms = true;

			if (check_digits(prod))
				++count_solutions;

		} while (++j < digits_end);
	} while (++i < digits_end);

	return valid_perms;
}


static inline void
perm_m1(void)
{
	unsigned int *i, *j, *k;
	unsigned int m1_a, m1_b, m1_c, m1_ab;

	i = digits_begin;
	do {
		m1_a = (*i) * 100;
		j    = digits_begin;
		do {
			m1_b  = (*j) * 10;
			m1_ab = m1_a + m1_b;
			k     = digits_begin;
			do {
				m1_c = *k;

				if (!perm_m2(m1_ab + m1_c))
					return; /* no more perms */

			} while (++k < digits_end);
		} while (++j < digits_end);
	} while (++i < digits_end);

}

static inline void
solve(void)
{

	qsort(digits_begin,
	      count_digits,
	      sizeof(*digits_begin),
	      &order_asc);

	perm_m1();
}

int
main(void)
{
	read_input();

	solve();

	write_output();

	return 0; /* unreachable */
}
