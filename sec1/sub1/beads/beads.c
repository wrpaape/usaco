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


static inline void
read_input(unsigned int *const restrict count_beads,
	   char *const restrict beads)
{
	FILE *input;
	assert(input = fopen(PROG ".in", "r"));
	assert(fscanf(input, "%u\n%s", count_beads, beads) == 2);
	assert(fclose(input) == 0);
}

static inline void
write_solution(const unsigned int solution)
{
	FILE *output;
	assert(output = fopen(PROG ".out", "w"));
	assert(fprintf(output, "%u\n", solution) >= 0);
	assert(fclose(output) == 0);
}


static inline unsigned int
scan_streaks(const unsigned int *const restrict streaks_begin,
	     const unsigned int *const restrict streaks_end)
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
 *
 * | l_white | l_color | m_white | r_color | r_white |
 *
 * where
 *
 *	length(l_white) >= 0
 *	length(l_color) >= 1
 *	length(m_white) >= 0
 *	length(r_color) >= 1
 *	length(r_white) >= 0
 */
	streak = streaks_begin;

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

		if (streak == streaks_end)
			break; /* will end on a color */

		l_white = m_white;
		m_white = r_white;
		r_white = *streak++; /* advance whites */

		span += r_white; /* append new white */

		if (span > max_span)
			max_span = span;
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
scan_remainder_beads(unsigned int *const restrict streaks_begin,
		     unsigned int *restrict streak,
		     const unsigned int first_color_streak_length,
		     const char *restrict white_streak_begin,
		     const char *restrict bead,
		     const char *const restrict first_white_streak_begin,
		     unsigned int streak_color,
		     const unsigned int count_beads)
{
	unsigned int color;
	const char *restrict color_streak_begin;

	/* bead points to start of second color streak */
	*streak++ = first_color_streak_length;   /* 1st color */
	*streak++ = (bead - white_streak_begin); /* 2nd white */

	/* get second color streak (may not be 3 white streaks) */
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

	/* bead points to start of third color streak */

	/* get remaining color streaks */
	while (1) {
		*streak++ = (white_streak_begin - color_streak_begin); /* color */
		*streak++ = (bead               - white_streak_begin); /* white */

		streak_color	   = color;
		color_streak_begin = bead;
		do {
			do {
				if (++bead == first_white_streak_begin) {
					/* final color streak traversed */
					*streak++ = (bead - color_streak_begin);
					return scan_streaks(streaks_begin,
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

static inline unsigned int
color_wrapping_streak(unsigned int *const restrict streaks_begin,
		      const char *restrict first_white_streak_begin,
		      const char *const restrict beads_begin,
		      const char *const restrict beads_end,
		      const char *restrict color_streak_begin,
		      const char *restrict bead,
		      unsigned int streak_color,
		      const unsigned int count_beads)
{
	unsigned int *restrict streak;
	const char *restrict white_streak_begin;
	unsigned int color;
	unsigned int first_color_streak_length;

	/* wrapping streak_color!
	 * find where color streak and first white streak begin
	 * (transition to other color */
	streak = streaks_begin;
	while (1) {
		do {
			if (color_streak_begin == bead)
				return count_beads; /* white + 1 color */

			color = (unsigned int) *--color_streak_begin;
		} while (color == streak_color);

		if (color != 'w') {
			first_white_streak_begin = ++color_streak_begin;
			*streak++ = 0; /* first white streak is 0 length */
			break;
		}

		first_white_streak_begin = color_streak_begin++;

		do {
			color = (unsigned int) *--first_white_streak_begin;
		} while (color == 'w');


		if (color != streak_color) {
			/* first white streak is > 0 length */
			++first_white_streak_begin;
			*streak++ = (  color_streak_begin
				     - first_white_streak_begin);
			break;
		}

		color_streak_begin = first_white_streak_begin;
	}

	/* AT LEAST 2 BEAD COLORS */

	/* find where wrapping color streak ends */
	do {
		do {
			color = (unsigned int) *++bead;
		} while (color == streak_color);

		white_streak_begin = bead;

		if (color != 'w')
			break; /* zero length white streak */

		do {
			color = (unsigned int) *++bead;
		} while (color == 'w');
	} while (color == streak_color);

	first_color_streak_length = (beads_end          - color_streak_begin)
		                  + (white_streak_begin - beads_begin);

	return scan_remainder_beads(streaks_begin,
				    streak,
				    first_color_streak_length,
				    white_streak_begin,
				    bead,
				    first_white_streak_begin,
				    color,
				    count_beads);
}


static inline unsigned int
white_wrapping_streak(unsigned int *const restrict streaks_begin,
		      const char *const restrict first_white_streak_begin,
		      const char *const restrict beads_begin,
		      const char *const restrict beads_end,
		      const char *restrict bead,
		      unsigned int streak_color,
		      const unsigned int count_beads)
{
	unsigned int *restrict streak;
	const char *restrict color_streak_begin;
	const char *restrict white_streak_begin;
	unsigned int color;
	unsigned int first_color_streak_length;

	/* set first streak as white, wrapping beads buffer */
	streak    = streaks_begin;
	*streak++ = (beads_end - first_white_streak_begin)
		  + (bead      - beads_begin);

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

	first_color_streak_length = (white_streak_begin - color_streak_begin);

	return scan_remainder_beads(streaks_begin,
				    streak,
				    first_color_streak_length,
				    white_streak_begin,
				    bead,
				    first_white_streak_begin,
				    color,
				    count_beads);
}



static inline unsigned int
solve(const unsigned int count_beads,
      const char *const restrict beads_begin)
{
	unsigned int streaks[(2 * MAX_BEADS) - 1];
	const char *restrict bead;      /* current bead */
	const char *restrict beads_end; /* end of buffer */
	const char *restrict first_white_streak_begin;
	const char *restrict last_color_bead;
	unsigned int color;
	unsigned int streak_color;

	beads_end = beads_begin + count_beads;
	bead	  = beads_begin;

	/* get first streak of white (length >= 0) wrapping
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
	last_color_bead = beads_end;
	do {
		color = (unsigned int) *--last_color_bead;
	} while (color == 'w');

	first_white_streak_begin = last_color_bead + 1;

	return (color == streak_color)
	     ? color_wrapping_streak(&streaks[0],
				     first_white_streak_begin,
				     beads_begin,
				     beads_end,
				     last_color_bead,
				     bead,
				     streak_color,
				     count_beads)
	     : white_wrapping_streak(&streaks[0],
				     first_white_streak_begin,
				     beads_begin,
				     beads_end,
				     bead,
				     streak_color,
				     count_beads);
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
