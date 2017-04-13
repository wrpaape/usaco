/*
ID: wrpaape1
LANG: C
TASK: hamming
*/

#include <stdio.h>
#include <assert.h>

#define N_MAX			64
#define HAMMING_DISTANCE(X, Y)	__builtin_popcount((X) ^ (Y))


static unsigned int N, B, D;
static unsigned int overflow;
static unsigned int solution[N_MAX];
static unsigned int count;

static inline void
read_input(void)
{
	FILE *input;
	assert(input = fopen("hamming.in", "r"));
	assert(fscanf(input, "%u %u %u\n", &N, &B, &D) == 3);
	assert(fclose(input) == 0);

	overflow = 1 << B;
}

static inline unsigned int
belongs_in_set(const unsigned int x)
{
	for (unsigned int i = 0; i < count; ++i)
		if (HAMMING_DISTANCE(x, solution[i]) < D)
			return 0;

	return 1;
}

static inline unsigned int
solve(void)
{
	for (unsigned int start = 0; start < overflow; ++start) {
		count = 0;
		for (unsigned int x = start; x < overflow; ++x) {
			if (!belongs_in_set(x))
				continue;

			solution[count++] = x;

			if (count == N)
				return 1;
		}
	}

	return 0; /* failure */
}

static inline void
write_output(void)
{
	unsigned int n;
	unsigned int next_line;
	const char *restrict format;
	FILE *output;

	assert(output = fopen("hamming.out", "w"));

	assert(fprintf(output, "%u", solution[0]) > 0);

	next_line = 10;
	for (n = 1; n < count; ++n) {

		if (n == next_line) {
			next_line += 10;
			format = "\n%u";

		} else {
			format = " %u";
		}

		assert(fprintf(output, format, solution[n]) > 0);
	}

	assert(fputs("\n", output) != EOF);

	assert(fclose(output) == 0);
}


int
main(void)
{

	read_input();

	assert(solve());

	write_output();

	return 0;
}
