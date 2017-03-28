/*
ID: wrpaape1
LANG: C
TASK: dualpal
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdio.h>	/* fopen, fscan, fclose */
#include <assert.h>	/* assert */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"dualpal"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define BUFFER_SIZE	65 /* 64 bits + '\0' */
#define BASE_MIN	2
#define BASE_MAX	10


/* function like macros
 * ────────────────────────────────────────────────────────────────────────── */
#define DIGIT_TO_ASCII(DIGIT) ((char) ((DIGIT) | 48u)) /* 0...9 → '0'...'9' */


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
FILE *output;
static char s_buffer[BUFFER_SIZE];
static char *const restrict s_last = &s_buffer[BUFFER_SIZE - 1];


static inline char *
stringify(unsigned int integer,
	  unsigned int base)
{
	char *restrict buffer;
	unsigned int digit;

	buffer = s_last;

	while (1) {
		digit    = integer % base;
		integer /= base;

		*buffer = DIGIT_TO_ASCII(digit);

		if (integer == 0)
			return buffer;

		--buffer;
	}
}


static inline unsigned int
is_palindrome(const char *const restrict s_begin)
{
	const char *restrict from;
	const char *restrict upto;

	from = s_begin;
	upto = s_last;

	while (from < upto) {
		if (*from != *upto)
			return 0;

		++from;
		--upto;
	}

	return 1;
}






int
main(void)
{
	FILE *input;
	unsigned int base;
	unsigned int n;
	unsigned int N;
	unsigned int s;
	unsigned int acc_pal;
	const char *restrict s_begin;

	assert(input = fopen(INPUT_FILE,  "r"));
	assert(fscanf(input, "%u %u\n", &N, &s) == 2);
	assert(fclose(input) == 0);

	assert(output = fopen(OUTPUT_FILE, "w"));

	n = 0;

	while (1) {
		acc_pal = 0;
		base    = BASE_MIN;
		++s;
		do {
			s_begin = stringify(s,
					    base);

			if (!is_palindrome(s_begin) || (++acc_pal < 2))
				continue;

			assert(fprintf(output, "%u\n", s) > 0);

			if (++n < N)
				break;

			assert(fclose(output) == 0);
			return 0;

		} while (++base <= BASE_MAX);
	}
	/* unreachable */
}
