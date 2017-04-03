/*
ID: wrpaape1
LANG: C
TASK: skidesign
*/


#include <stdio.h>
#include <assert.h>
#include <math.h>

#define N_MAX		1000
#define DIFF_MAX	17

static int N;
static int elevs[N_MAX];

static inline int
cost_of_window(const int min_ib,
	       const int max_ib)
{
	int hill, diff, total_cost, elev;

	/* calculate total cost for window [min_ib, max_ib] */
	total_cost = 0;

	for (hill = 0; hill < N; ++hill) {
		elev = elevs[hill];

		if (elev < min_ib)
			diff = min_ib - elev;
		else if (elev > max_ib)
			diff = elev - max_ib;
		else
			continue;

		total_cost += (diff * diff);
	}

	return total_cost;
}

static inline int
scan_windows(const int mid_elev)
{
	int min_ib;
	int max_ib;
	int min_total_cost;
	int total_cost;

	/* mid_elev is set to the rounded weighted average elevation
	 * scan 2 windows below mid_elev and 2 windows above mid_elev
	 * i.e.
	 *
	 * [mid_elev - 10, mid_elev +  7]
	 * [mid_elev -  9, mid_elev +  8]
	 * [mid_elev -  8, mid_elev +  9]
	 * [mid_elev -  7, mid_elev + 10]
	 *
	 * to account for rounding errors */

	min_ib = mid_elev - 10;
	max_ib = min_ib + DIFF_MAX;

	min_total_cost = cost_of_window(min_ib++, max_ib++);

	total_cost = cost_of_window(min_ib++, max_ib++);
	if (total_cost < min_total_cost)
		min_total_cost = total_cost;

	total_cost = cost_of_window(min_ib++, max_ib++);
	if (total_cost < min_total_cost)
		min_total_cost = total_cost;

	total_cost = cost_of_window(min_ib, max_ib);
	if (total_cost < min_total_cost)
		min_total_cost = total_cost;

	return min_total_cost;
}

static inline int
solve(void)
{
	int hill;
	int tot_elev, mid_elev;
	double avg_elev, weighted_avg_elev;
	double tot_dev, avg_dev, diff, weight, weighted_avg_elev;

	/* find average elevation */
	tot_elev = 0;
	for (hill = 0; hill < N; ++hill)
		tot_elev += elevs[hill];

	avg_elev = ((double) tot_elev) / ((double) N);

	/* get signed sum of the squares of the differences from average elevation */
	tot_dev = 0.0;

	for (hill = 0; hill < N; ++hill) {
		diff   = ((double) elevs[hill]) - avg_elev;
		weight = diff * diff;

		if (diff < 0.0)
			weight = -weight;

		tot_dev += weight;
	}

	/* divide sum of squares of the differences by N^2 to obtain
	 * offset needed to apply to average elevation
	 * s.t. total 'weight' above and below new elevation is equal */
	avg_dev = tot_dev / (double) (N * N);

	/* apply offset */
	weighted_avg_elev = avg_elev + avg_dev;

	/* round to nearest integer */
	mid_elev = (int) lround(weighted_avg_elev);

	/* evaluate 17-wide windows about mid_elev */
	return scan_windows(mid_elev);
}

int
main(void)
{
	FILE *input;
	FILE *output;
	int hill;

	assert(input = fopen("skidesign.in", "r"));
	assert(fscanf(input, "%d\n", &N) == 1);

	for (hill = 0; hill < N; ++hill)
		assert(fscanf(input, "%d\n", &elevs[hill]) == 1);
	assert(fclose(input) == 0);

	const int min_total = solve();

	assert(output = fopen("skidesign.out", "w"));
	assert(fprintf(output, "%d\n", min_total) > 0);
	assert(fclose(output) == 0);
	return 0;
}
