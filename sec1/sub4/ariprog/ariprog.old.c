/*
ID: wrpaape1
LANG: C
TASK: ariprog
*/

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define N_MIN		3
#define N_MAX		25
#define M_MAX		250
#define BI_SQ_MAX_MAX	(2 * M_MAX * M_MAX)


int N, M;
int bi_sq_max;

bool bi_sq[BI_SQ_MAX_MAX + 1];

FILE *output;

static inline void
init_bisquares(void)
{
	int p, q, p_sq, q_sq;

	for (p = 0; p <= M; ++p) {
		p_sq = p * p;
		for (q = p; q <= M; ++q) {
			q_sq		 = q * q;
			bi_sq_max	 = p_sq + q_sq;
			bi_sq[bi_sq_max] = true;
		}
	}

}

static inline bool
solve(void)
{
	int a, b, num;
	int a_max, b_max, n_max, b_n_max, num_max;
	bool seq_found;

	init_bisquares();

	seq_found = false;

	n_max = N - 1;
	b_max = bi_sq_max / n_max;

	for (b = 1; b <= b_max; ++b) {
		b_n_max = b * n_max;
		a_max   = bi_sq_max - b_n_max;

		for (a = 0; a <= a_max; ++a) {
			num_max = a + b_n_max;

			for (num = a; bi_sq[num]; num += b)
				if (num == num_max) {
					assert(fprintf(output,
						       "%d %d\n",
						       a, b) > 0);

					seq_found = true;
					break;
				}
		}
	}

	return seq_found;
}

int
main(void)
{
	FILE *input;

	assert(input = fopen("ariprog.in", "r"));
	assert(fscanf(input, "%d\n%d\n", &N, &M) == 2);
	assert(fclose(input) == 0);

	assert(output = fopen("ariprog.out", "w"));
	if (!solve())
		assert(fputs("NONE\n", output) != EOF);
	assert(fclose(output) == 0);
	return 0;
}
