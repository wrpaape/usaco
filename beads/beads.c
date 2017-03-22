/*
ID: wrpaape1
LANG: C
TASK: beads
*/

#include <stdio.h>  /* fdopen, fdclose, getline */
#include <assert.h>

/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG "beads"
#define MAX_BEADS 350


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
/* struct Streak { */
/* 	unsigned int length; */
/* 	unsigned int start; */
/* }; */


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
read_input(unsigned int *const restrict count_beads,
	   char *const restrict beads)
{
	FILE *input;
	assert(input = fopen(PROG ".in", "r"));
	assert(fscanf(input, "%u\n%s", count_beads, beads) == 2);
	assert(fclose(input) == 0);
}

static inline unsigned int
scan_streaks(const unsigned int *const restrict streak_begin,
	     const unsigned int *const restrict streak_end)
{
	const unsigned int *restrict streak;
	unsigned int max_span;
	unsigned int span;
	unsigned int first_color_white;
	unsigned int first_white;
	unsigned int first_color;
	unsigned int l_white;
	unsigned int l_color;
	unsigned int m_white;
	unsigned int r_color;
	unsigned int r_white;

/* observe 'span' window of streaks of the form:
 * | L_WHITE >= 0 | L_COLOR > 0 | M_WHITE >= 0 | R_COLOR > 0 | R_WHITE >= 0 | */
	streak = streak_begin;

	l_white = *streak++;
	l_color = *streak++;
	m_white = *streak++;
	r_color = *streak++;
	r_white = *streak++;

	first_white       = l_white;
	first_color       = l_color;
	first_color_white = first_color + m_white;

	span      = l_white + first_color_white + r_color + r_white;
	max_span  = span;

	while (1) {
		span -= (l_white + l_color); /* ditch trailing 2 streaks */

		l_color = r_color;
		r_color = *streak++; /* advance colors */

		span += r_color; /* append new color */

		if (streak == streak_end)
			break; /* will end on a color */

		l_white = m_white;
		m_white = r_white;
		r_white = *streak++; /* advance whites */

		span += r_white; /* append new white */
	}

	span += first_white; /* append white streak from beads_begin */

	if (span > max_span)
		max_span = span;

	span -= (m_white + l_color); /* m_white is trailing */
	span += first_color_white;   /* append first color + leading white */

	if (span > max_span)
		max_span = span;

	return max_span;
}

static inline unsigned int
solve(const unsigned int count_beads,
      const char *const restrict beads_begin)
{
	unsigned int streaks[(2 * MAX_BEADS) - 1];
	unsigned int *restrict streak;
	const char *restrict bead;      /* current bead */
	const char *restrict beads_end; /* end of buffer */
	const char *restrict first_white_streak_begin;
	const char *restrict color_streak_begin;
	const char *restrict white_streak_begin;
	unsigned int color;
	unsigned int streak_color;

	beads_end = beads_begin + count_beads;
	bead	  = beads_begin;

	/* get first streak of white (length >= 0) spanning
	 * (..., beads_end) + (beads_begin, ...) */
	while (1) {
		color = (unsigned int) *bead;

		if (color != 'w')
			break;

		if (++bead == beads_end)
			return count_beads; /* all white */
	}

	/* save start of color streak */
	streak_color = color;

	/* get first half of first white streak */
	first_white_streak_begin = beads_end;
	do {
		color = (unsigned int) *--first_white_streak_begin;
	} while (color == 'w');

	++first_white_streak_begin;

	/* first streak is white */
	*streak++ = (beads_end - first_white_streak_begin)
		  + (bead - beads_begin);

	/* get first color streak */
	color_streak_begin = bead;
	do {
		do {
			if (++bead == first_white_streak_begin)
				return count_beads; /* all white and 1 color */

			color = (unsigned int) *bead;
		} while (color == streak_color);

		white_streak_begin = bead;

		if (color != 'w')
			break;

		do {
			color = (unsigned int) *++bead;
		} while (color == 'w');
	} while (color == streak_color);

	/* bead point to start of second color streak */
	*streak++ = (white_streak_begin - color_streak_begin); /* color */
	*streak++ = (bead - white_streak_begin);	       /* white */

	/* get second color streak (may not be 3 white streaks) */
	streak_color	   = color;
	color_streak_begin = bead;
	do {
		do {
			if (++bead == first_white_streak_begin)
				return count_beads; /* 2 color streaks */

			color = (unsigned int) *bead;
		} while (color == streak_color);

		white_streak_begin = bead;

		if (color != 'w')
			break;

		do {
			color = (unsigned int) *++bead;
		} while (color == 'w');
	} while (color == streak_color);

	/* bead point to start of third color streak */

	/* get remaining color streaks */
	while (1) {
		*streak++ = (white_streak_begin - color_streak_begin);
		*streak++ = (bead - white_streak_begin);

		streak_color	   = color;
		color_streak_begin = bead;
		do {
			do {
				if (++bead == first_white_streak_begin) {
					/* final color streak traversed */
					*streak++ = (bead - color_streak_begin);
					return scan_streaks(&streaks[0],
							    streak);
				}

				color = (unsigned int) *bead;
			} while (color == streak_color);

			white_streak_begin = bead;

			if (color != 'w')
				break;

			do {
				color = (unsigned int) *++bead;
			} while (color == 'w');
		} while (color == streak_color);
	}
}


static inline void
write_solution(const unsigned int solution)
{
	FILE *output;
	assert(output = fopen(PROG ".out", "w"));
	assert(fprintf(output, "%u\n", solution) >= 0);
	assert(fclose(output) == 0);
}


int
main(void)
{
	char beads[MAX_BEADS + 1];
	unsigned int count_beads;
	unsigned int solution;

	read_input(&count_beads,
		   &beads[0]);

	solution = solve(count_beads,
			 &beads[0]);

	write_solution(solution);

	return 0;
}
