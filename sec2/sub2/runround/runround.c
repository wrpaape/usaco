/*
ID: wrpaape1
LANG: C
TASK: runround
*/

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define MAX_LENGTH 9

static unsigned long digits_buffer[MAX_LENGTH];
static unsigned long *const restrict digits_end = &digits_buffer[MAX_LENGTH];
static unsigned long *restrict digits_begin;
static unsigned long length;

static inline const char *
reduce_digits(void)
{
	static char solution[MAX_LENGTH + 2];
	unsigned long i;

	i = 0;
	do {
		solution[i] = digits_begin[i] | 48;
	} while (++i < length);

	solution[i] = '\n';

	return &solution[0];
}

static inline bool
init_digits(unsigned long start)
{
	unsigned long digit;
	bool incremented;


	incremented  = false;
	digits_begin = digits_end - 1;

	while (1) {
		digit  = start % 10;
		start /= 10;

		if (digit == 0) {
			digit       = 1;
			incremented = true;
		}

		*digits_begin = digit;

		if (start == 0)
			break;

		--digits_begin;
	}

	length = digits_end - digits_begin;

	return incremented;
}

static inline void
increment_digits(void)
{
	unsigned long *restrict ptr;

	ptr = digits_end;

	while (*--ptr == 9)
		*ptr = 1; /* skip 0 */

	++(*ptr);

	if (ptr < digits_begin) {
		digits_begin = ptr;
		++length;
	}
}

static inline bool
is_runround(void)
{
	bool visited[MAX_LENGTH + 1] = { false };
	bool *restrict visit_ptr;
	unsigned long i;
	unsigned long count;
	unsigned long digit;

	i     = 0;
	count = 0;

	do {
		digit     = digits_begin[i];
		visit_ptr = &visited[digit];

		if (*visit_ptr)
			return false;

		*visit_ptr = true;

		i += digit;
		i %= length;
	} while (++count < length);

	return (i == 0);
}

static inline const char *
solve(const unsigned long start)
{
	if (init_digits(start))
		goto HEAD_START;

	while (1) {
		increment_digits();
HEAD_START:
		if (is_runround())
			return reduce_digits();
	}
}

int
main(void)
{
	FILE *input, *output;
	unsigned long start;
	const char *restrict solution;

	assert(input = fopen("runround.in", "r"));
	assert(fscanf(input, "%lu\n", &start) == 1);
	assert(fclose(input) == 0);

	solution = solve(start);

	assert(output = fopen("runround.out", "w"));
	assert(fputs(solution, output) != EOF);
	assert(fclose(output) == 0);
	return 0;
}
