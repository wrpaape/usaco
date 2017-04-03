/*
ID: wrpaape1
LANG: C
TASK: skidesign
*/


#include <stdio.h>
#include <assert.h>
#include <limits.h>

#define N_MAX    1000
#define MAX_ELEV 100

static int N;
static int elevs[N_MAX];

int
check_total_cost(const int min_ib,
		 const int max_ib)
{
	int hill, diff, total_cost, elev;

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


int
solve(void)
{
	int hill;
	int elev, min_elev, max_elev;
	int min_ib, max_ib;
	int min_total_cost, total_cost;

	min_elev = MAX_ELEV;
	max_elev = 0;

	for (hill = 0; hill < N; ++hill) {
		elev = elevs[hill];

		if (elev < min_elev)
			min_elev = elev;
		else if (elev > max_elev)
			max_elev = elev;
	}

	min_total_cost = INT_MAX;

	min_ib = min_elev;
	max_ib = min_elev + 17;

	do {
		total_cost = check_total_cost(min_ib,
					      max_ib);

		if (total_cost < min_total_cost)
			min_total_cost = total_cost;

		++min_ib;
	} while (++max_ib < max_elev);

	return min_total_cost;
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
