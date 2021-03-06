/*
ID: wrpaape1
LANG: C
TASK: ariprog
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#define N_MIN		3
#define N_MAX		25
#define M_MAX		250
#define M_MAX_SQ	(M_MAX * M_MAX)
#define BI_SQ_MAX_MAX	(2 * M_MAX_SQ)


int N, M;
int bi_sq_max;
int count_bi_sqs;

bool is_bi_sq[BI_SQ_MAX_MAX + 1];
int bi_sqs[M_MAX_SQ];

FILE *output;

int
order_asc(const void *key1,
	  const void *key2)
{
	return *((int *) key1) - *((int *) key2);
}

static inline void
init_bisquares(void)
{
	int p, q, p_sq, q_sq;

	for (p = 0; p <= M; ++p) {
		p_sq = p * p;
		for (q = p; q <= M; ++q) {
			q_sq		       = q * q;
			bi_sq_max	       = p_sq + q_sq;
			is_bi_sq[bi_sq_max]    = true;
			bi_sqs[count_bi_sqs++] = bi_sq_max;
		}
	}

	qsort(&bi_sqs[0],
	      count_bi_sqs,
	      sizeof(bi_sqs[0]),
	      &order_asc);
}

static inline bool
solve(void)
{
	int a, b, num;
	int a_max, b_max, n_max, b_n_max, num_max;
	bool seq_found;
	int *restrict bi_sq;

	init_bisquares();

	seq_found = false;

	n_max = N - 1;
	b_max = bi_sq_max / n_max;

	for (b = 1; b <= b_max; ++b) {
		b_n_max = b * n_max;
		a_max   = bi_sq_max - b_n_max;

		for (bi_sq = &bi_sqs[0]; a = *bi_sq, a <= a_max; ++bi_sq) {
			num_max = a + b_n_max;

			for (num = a; is_bi_sq[num]; num += b)
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
