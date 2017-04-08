/*
ID: wrpaape1
LANG: C
TASK: numtri
*/

#include <stdio.h>
#include <assert.h>

#define R_MAX	1000

/* track max sum for (row, col) at (col, row + 1) */
static int tri[R_MAX][R_MAX + 1];

int
get_max_sum_at(const ssize_t row,
	       const ssize_t col)
{
	int max_sum_above, sum_above, max_sum_at;
	int *restrict max_sum_at_ptr;
	ssize_t row_above;

	if ((row < 0) || (col < 0) || (col > row))
		return 0; /* out of bounds */

	max_sum_at_ptr = &tri[col][row + 1];
	max_sum_at     = *max_sum_at_ptr;

	if (max_sum_at >= 0)
		return max_sum_at; /* already calculated */

	row_above = row - 1; /* move up 1 row */

	max_sum_above = get_max_sum_at(row_above, col - 1); /* up left */
	sum_above     = get_max_sum_at(row_above, col);     /* up right */
	if (sum_above > max_sum_above)
		max_sum_above = sum_above;

	max_sum_at      = tri[row][col] + max_sum_above;
	*max_sum_at_ptr = max_sum_at; /* mark (row, col) */
	return max_sum_at;
}

int
main(void)
{
	FILE *input, *output;
	int max_sum, sum;
	ssize_t R, last_row, row, col;

	assert(input = fopen("numtri.in", "r"));
	assert(fscanf(input, "%zd\n", &R) == 1);
	for (row = 0; row < R; ++row) {
		for (col = 0; col <= row; ++col)
			assert(fscanf(input, "%d", &tri[row][col]) == 1);

		do {
			tri[row][col] = -1; /* sentinel for lookup */
		} while (++col <= R);
	}
	assert(fclose(input) == 0);

	last_row = R - 1;
	max_sum  = -1;

	/* find max for each column in last row */
	for (col = 0; col < R; ++col) {
		sum = get_max_sum_at(last_row, col);
		if (sum > max_sum)
			max_sum = sum;
	}

	assert(output = fopen("numtri.out", "w"));
	assert(fprintf(output, "%d\n", max_sum) > 0);
	assert(fclose(output) == 0);
	return 0;
}
