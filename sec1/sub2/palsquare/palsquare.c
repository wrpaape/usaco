/*
ID: wrpaape1
LANG: C
TASK: palsquare
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdio.h>	/* fopen, fscan, fclose */
#include <assert.h>	/* assert */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"palsquare"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define N_MIN		1
#define N_MAX		300
#define BUFFER_SIZE	65 /* 64 bits + '\0' */

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
FILE *output;
static unsigned int base;
static unsigned int n = N_MIN;
static char sq_buffer[BUFFER_SIZE];
static char n_buffer[BUFFER_SIZE];
static char *const restrict sq_last = &sq_buffer[BUFFER_SIZE - 2];
static char *const restrict n_last  = &n_buffer[BUFFER_SIZE - 2];


static unsigned int digit_map[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J'
};

static inline char *
stringify(char *restrict buffer,
	  unsigned int integer)
{
	unsigned int digit;

	while (1) {
		digit    = integer % base;
		integer /= base;

		*buffer = digit_map[digit];

		if (integer == 0)
			return buffer;

		--buffer;
	}
}


static inline void
check_palindrome(const char *const restrict sq_begin)
{
	const char *restrict n_begin;
	const char *restrict sq_from;
	const char *restrict sq_upto;

	sq_from = sq_begin;
	sq_upto = sq_last;

	while (sq_from < sq_upto) {
		if (*sq_from != *sq_upto)
			return;

		++sq_from;
		--sq_upto;
	}

	n_begin = stringify(n_last,
			    n);

	assert(fprintf(output,
		       "%s %s\n",
		       n_begin,
		       sq_begin) > 0);
}






int
main(void)
{
	FILE *input;
	unsigned int sq;
	const char *restrict sq_begin;

	assert(input = fopen(INPUT_FILE,  "r"));
	assert(fscanf(input, "%u\n", &base) == 1);
	assert(fclose(input) == 0);

	assert(output = fopen(OUTPUT_FILE, "w"));

	do {
		sq       = n * n;
		sq_begin = stringify(sq_last,
				     sq);
		check_palindrome(sq_begin);
	} while (++n < N_MAX);

	assert(fclose(output) == 0);
	return 0;
}
