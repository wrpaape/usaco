/*
ID: wrpaape1
LANG: C
TASK: milk2
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdio.h>  /* fdopen, fdclose, fscanf */
#include <stdlib.h> /* qsort, mergesort */
#include <stddef.h> /* offsetof */
#include <assert.h> /* assert */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"milk2"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define MAX_FARMERS	5000


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Interval {
	int begin;
	int end;
};


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static struct Interval intervals[MAX_FARMERS];
static struct Interval *restrict intervals_begin = &intervals[0];
static struct Interval *restrict intervals_end;
static size_t count_farmers;
static unsigned int max_milk_span;
static unsigned int max_idle_span;


static inline void
read_input(void)
{
	FILE *input;
	struct Interval *restrict interval;

	assert(input = fopen(INPUT_FILE, "r"));

	/* read in count farmers */
	assert(fscanf(input, "%zu\n", &count_farmers) == 1);

	intervals_end = intervals_begin + count_farmers;
	interval      = intervals_begin;

	/* read in intervals */
	do { /* at least 1 farmer */
		assert(fscanf(input, "%d %d\n",
			      &interval->begin,
			      &interval->end) == 2);
	} while (++interval < intervals_end);

	assert(fclose(input) == 0);
}

static inline void
write_output(void)
{
	FILE *output;
	assert(output = fopen(OUTPUT_FILE, "w"));
	assert(fprintf(output, "%u %u\n",
		       max_milk_span,
		       max_idle_span) >= 0);
	assert(fclose(output) == 0);
}


/* Interval comparison functions
 * ────────────────────────────────────────────────────────────────────────── */
int
order_by_asc_begin(const void *interval1,
		   const void *interval2)
{	/* 'begin' is 1st field, 0 offset from interval pointer */
	const int begin1 = *((const int *) interval1);
	const int begin2 = *((const int *) interval2);;
	return begin1 - begin2;
}

static inline void
solve(void)
{
	struct Interval *restrict interval;
	int acc_begin;
	int acc_end;
	int next_begin;
	int next_end;
	unsigned int span;

	/* sort intervals on 'begin' in ascending order */
	qsort(intervals_begin,
	      count_farmers,
	      sizeof(struct Interval),
	      &order_by_asc_begin);

	interval  = intervals_begin;
	acc_begin = interval->begin;
	acc_end	  = interval->end;

	/* set initial milk span (idle span static, set to 0) */
	max_milk_span = acc_end - acc_begin;

	while (++interval < intervals_end) {
		next_begin = interval->begin;
		next_end   = interval->end;

		if (next_begin > acc_end) {
			/* 'interval' does not overlap accumulating milk span
			 * calculate idle span */
			span = next_begin - acc_end;

			if (span > max_idle_span)
				max_idle_span = span;

			/* calculate accumulating milk span */
			span = acc_end - acc_begin;

			if (span > max_milk_span)
				max_milk_span = span;

			/* reset milk span */
			acc_begin = next_begin;
			acc_end   = next_end;

		} else {
			/* 'interval' is in accumulating milk span */
			if (next_end > acc_end)
				acc_end = next_end; /* extend span */
		}
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
