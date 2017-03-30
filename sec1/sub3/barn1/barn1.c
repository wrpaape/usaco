/*
ID: wrpaape1
LANG: C
TASK: barn1
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdio.h>  /* fdopen, fdclose, fscanf */
#include <assert.h> /* assert */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"barn1"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define MAX_BOARD_LIMIT	50
#define MAX_STALLS	200


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static unsigned int rem_boards;
static unsigned int stall_count;
static unsigned int cow_count;
static unsigned int stalls_covered;
/* static unsigned int stalls_buffer[MAX_STALLS]; /1* 1-based indices *1/ */
/* static unsigned int *const restrict stalls	 = &stalls_buffer[-1]; */
/* static unsigned int *const restrict stalls_begin = &stalls_buffer[ 0]; */
/* records count of gaps of a particular size
 * count of 'gap_size' gaps = gaps[gap_size] */
/* static unsigned int gaps_buffer[MAX_STALLS - 1]; */
/* static unsigned int *const restrict gaps = &gaps_buffer[-1]; /1* gap_size > 0 *1/ */

/* quiet 'array subscript out of bounds' warning */
static unsigned int stalls_buffer[MAX_STALLS + 1]; /* 1-based indices */
static unsigned int *const restrict stalls	 = &stalls_buffer[0];
static unsigned int *const restrict stalls_begin = &stalls_buffer[1];
/* records count of gaps of a particular size
 * count of 'gap_size' gaps = gaps[gap_size] */
static unsigned int gaps_buffer[MAX_STALLS];
static unsigned int *const restrict gaps = &gaps_buffer[0];



static inline void
read_input(void)
{
	FILE *input;
	unsigned int rem_cows;
	unsigned int cow;

	assert(input = fopen(INPUT_FILE, "r"));

	/* read in count farmers */
	assert(fscanf(input, "%u %u %u\n",
		      &rem_boards,
		      &stall_count,
		      &cow_count) == 3);

	/* populate stalls */
	rem_cows = cow_count;
	do {
		assert(fscanf(input, "%u\n", &cow) == 1);
		stalls[cow] = 1;
	} while (--rem_cows > 0);

	assert(fclose(input) == 0);
}

static inline void
write_output(void)
{
	FILE *output;
	assert(output = fopen(OUTPUT_FILE, "w"));
	assert(fprintf(output, "%u\n",
		       stalls_covered) >= 0);
	assert(fclose(output) == 0);
}


static inline void
solve(void)
{
	unsigned int *restrict cows_begin;
	unsigned int *restrict stall;
	unsigned int *restrict gap_begin;
	unsigned int rem_cows;
	size_t gap_size;
	size_t max_gap_size;

	/* scan stalls for first cow */
	for (stall = stalls_begin; !*stall; ++stall)
		;

	cows_begin   = stall;
	rem_cows     = cow_count;
	max_gap_size = 0;

	while (--rem_cows > 0)
		if (!*++stall) {
			gap_begin = stall;

			do {
				++stall;
			} while (!*stall);

			gap_size = stall - gap_begin;

			if (gap_size > max_gap_size)
				max_gap_size = gap_size;

			++(gaps[gap_size]);
		}

	/* initialize as if using 1 giant board */
	stalls_covered = (stall + 1) - cows_begin;

	while (--rem_boards > 0) {
		/* split next largest gap, use another board */
		stalls_covered -= max_gap_size;

		if (stalls_covered == cow_count)
			break;

		if (--(gaps[max_gap_size]) == 0)
			do { /* no more gaps of this size, rewind */
				--max_gap_size;
			} while (gaps[max_gap_size] == 0);
	}
}

int
main(void)
{
	read_input();

	solve();

	write_output();

	return 0;
}
