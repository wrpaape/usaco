/*
ID: wrpaape1
LANG: C
TASK: transform
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <fcntl.h>	/* open */
#include <sys/stat.h>	/* fstat */
#include <sys/types.h>	/* needed for read */
#include <sys/uio.h>	/* needed for read */
#include <unistd.h>	/* STDOUT/IN/ERR_FILENO, read, write, close */
#include <assert.h>	/* assert */
#include <stdlib.h>	/* exit */
#include <string.h>	/* memcmp */
#include <stdbool.h>	/* bool */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"transform"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define N_MAX		10
#define N_SQ_MAX	(N_MAX * N_MAX)
#define LENGTH_MAX	((N_SQ_MAX / sizeof(unsigned int)) + 1)
#define PATTERN_MAX	(N_SQ_MAX + N_MAX) /* N x N '\n'-delimited rows */
#define BUFFER_MAX	((2 * PATTERN_MAX) + 3) /* 2 patterns + '10\n' */


/* function like macros
 * ────────────────────────────────────────────────────────────────────────── */
#define OPEN_INPUT()							\
open(INPUT_FILE,							\
     O_RDONLY)

#define OPEN_OUTPUT()							\
open(OUTPUT_FILE,							\
     O_CREAT | O_TRUNC | O_WRONLY,					\
     S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)

#define DIGIT_TO_ASCII(DIGIT) ((char) ((DIGIT) | 48u))
#define ASCII_TO_DIGIT(ASCII) (((size_t) (ASCII)) & 15u)


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static size_t N;
static size_t N_m1;
static size_t skip_rows;
static size_t row_size;
static size_t pattern_size;
static char *restrict pattern1; /* may be reflected, not const */
static const char *restrict pattern2;
static char buffer[BUFFER_MAX];




/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
write_output(const unsigned int transform)
{
	char solution[2]; /* 1-7 + \n */
	int output_fd;

	output_fd = OPEN_OUTPUT();
	assert(output_fd >= 0);

	solution[0] = transform;
	solution[1] = '\n';

	assert(write(output_fd,
		     &solution[0],
		     2) == 2);

	exit(close(output_fd) != 0); /* 1/0, failure/success */
}

static inline void
read_input(void)
{
	int input_fd;

	/* read in buffer */
	input_fd = OPEN_INPUT();
	assert(input_fd >= 0);
	assert(read(input_fd,
		    &buffer[0],
		    sizeof(buffer)) > 0);
	assert(close(input_fd) == 0);

	/* read in N */
	pattern1 = &buffer[0];

	N = ASCII_TO_DIGIT(*pattern1++);

	if (N == 1) {
		if (*pattern1 == '\n')
			write_output('1'); /* N is 1, any trans, '1' is min */

		N = 10;
		pattern1 += 2; /* skip '0\n' */

	} else {
		++pattern1; /* skip '\n' */
	}

	N_m1         = N - 1;
	row_size     = N + 1;
	pattern_size = row_size * N;
	skip_rows    = pattern_size - row_size;
	pattern2     = pattern1 + pattern_size;
}

static inline bool
match_rotate_90(void)
{
	const char *restrict top1;
	const char *restrict ptr1;
	const char *restrict ptr2;

	top1 = pattern1;
	ptr2 = pattern2;

	while (1) {
		ptr1 = top1 + skip_rows;

		do {
			if (*ptr1 != *ptr2)
				return false; /* no match */

			++ptr2;
			ptr1 -= row_size;

		} while (ptr1 >= top1);

		if (*++top1 == '\n') /* end of top row */
			return true;

		++ptr2; /* skip '\n' */
	}
}

static inline bool
match_rotate_180(void)
{
	const char *restrict ptr1;
	const char *restrict ptr2;

	ptr2 = pattern2;
	ptr1 = pattern2 - 2;

	while (1) {
		if (*ptr1 != *ptr2)
			return false; /* no match */

		if (ptr1 == pattern1) /* complete match */
			return true;

		++ptr2;
		--ptr1;
	}
}

static inline bool
match_rotate_270(void)
{
	const char *restrict top1;
	const char *restrict ptr1;
	const char *restrict ptr2;

	top1 = pattern1 + N_m1;
	ptr2 = pattern2;

	while (1) {
		ptr1 = top1;

		do {
			if (*ptr1 != *ptr2)
				return false; /* no match */

			++ptr2;
			ptr1 += row_size;
		} while (ptr1 < pattern2);

		if (--top1 < pattern1) /* end of top row */
			return true;

		++ptr2; /* skip \n */
	}
}

static inline void
reflect(void)
{
	char *restrict end_ptr;
	char *restrict begin_ptr;
	char *restrict row_ptr;
	unsigned int token;

	row_ptr = pattern1;

	do {
		begin_ptr = row_ptr;
		end_ptr   = row_ptr + N_m1;

		do {
			token	   = *begin_ptr;
			*begin_ptr = *end_ptr;
			*end_ptr   = token;
		} while (++begin_ptr < --end_ptr);

		row_ptr += row_size;
	} while (row_ptr < pattern2);
}


static inline unsigned int
solve(void)
{
	if (match_rotate_90())
		return '1';

	if (match_rotate_180())
		return '2';

	if (match_rotate_270())
		return '3';

	reflect();

	if (memcmp(pattern1,
		   pattern2,
		   pattern_size) == 0)
		return '4';

	if (   match_rotate_90()
	    || match_rotate_180()
	    || match_rotate_270())
		return '5';

	reflect();

	return '6' + (memcmp(pattern1,
			     pattern2,
			     pattern_size) != 0);
}


int
main(void)
{
	read_input();

	const unsigned int transform = solve();

	write_output(transform);

	/* unreachable */
	return 0;
}
