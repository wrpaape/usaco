/*
ID: wrpaape1
LANG: C
TASK: lamps
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>

#define N_MAX			100
#define COUNT_BUTTONS		4
#define COUNT_SOLUTIONS_MAX	(1 << COUNT_BUTTONS)

typedef unsigned long LampBlock;

#define LAMP_BLOCK_MAX		ULONG_MAX
#define LAMP_BLOCK_BIT		(sizeof(LampBlock) * CHAR_BIT)
#define LAMP_BLOCK_MAX_BIT	(1UL << (LAMP_BLOCK_BIT - 1))
#define LAMP_LENGTH(N)							\
(((N) / LAMP_BLOCK_BIT) + (((N) % LAMP_BLOCK_BIT) > 0))

typedef LampBlock LampSet[LAMP_LENGTH(N_MAX)];

static LampSet buttons[COUNT_BUTTONS];

static unsigned int final_on[N_MAX];
static unsigned int final_off[N_MAX];
static size_t length_final_on;
static size_t length_final_off;

static LampSet solution_buffer[COUNT_SOLUTIONS_MAX];
static LampSet *solutions[COUNT_SOLUTIONS_MAX];
static size_t count_solutions;

static size_t N;
static size_t lamp_length;
static size_t lamp_size;
static size_t max_switch_count;
static LampBlock max_rem_bit;

static inline int
compare_lamp_sets(const LampSet lamp_set1,
		  const LampSet lamp_set2)
{
	LampBlock block1, block2;
	size_t i;

	i = 0;
	do {
		block1 = lamp_set1[i];
		block2 = lamp_set2[i];

		if (block1 != block2)
			return (block1 > block2) ? -1 : 1; /* reverse compare */
	} while (++i < lamp_length);

	return 0;
}

int
order_lamp_sets_asc(const void *key1,
		    const void *key2)
{
	const LampSet *restrict *lamp_set1 = (const LampSet *restrict *) key1;
	const LampSet *restrict *lamp_set2 = (const LampSet *restrict *) key2;

	return compare_lamp_sets(**lamp_set1,
				 **lamp_set2);
}

static inline void
read_input(void)
{
	FILE *input;
	size_t C, lamp_rem, rem_gt_zero;
	int lamp;

	assert(input = fopen("lamps.in", "r"));
	assert(fscanf(input, "%zu\n%zu\n", &N, &C) == 2);

	lamp_rem         = N % LAMP_BLOCK_BIT;
	rem_gt_zero      = (lamp_rem > 0);
	max_rem_bit      = rem_gt_zero << lamp_rem;
	lamp_length      = (N / LAMP_BLOCK_BIT) + rem_gt_zero;
	lamp_size	 = sizeof(LampBlock) * lamp_length;
	max_switch_count = (C < COUNT_BUTTONS) ? C : COUNT_BUTTONS;

	while (1) {
		assert(fscanf(input, "%d", &lamp) == 1);

		if (lamp < 0)
			break;

		final_on[length_final_on++] = (unsigned int) (lamp - 1);
	}

	while (1) {
		assert(fscanf(input, "%d", &lamp) == 1);

		if (lamp < 0)
			break;

		final_off[length_final_off++] = (unsigned int) (lamp - 1);
	}

	assert(fclose(input) == 0);
}

static inline void
write_lamp_block(FILE *output,
		 const LampBlock block,
		 const LampBlock bit_max)
{
	int token;
	LampBlock bit;

	bit = 1;
}

static inline void
write_lamp_set(FILE *output,
	       const LampSet lamp_set)
{
	LampBlock block;
	LampBlock bit;
	size_t i;

	i   = 0;
	bit = 1 << 1;
	while (1) {
		block = lamp_set[i];

		if (++i == lamp_length)
			break;

		while (1) {

			assert(fputc(token, output) != EOF);

		       if (bit == LAMP_BLOCK_MAX_BIT)
			       break;

		       bit <<= 1;
		}

		bit = 1;
	}
}

static inline void
write_solutions(FILE *output)
{
	qsort(&solutions[0],
	      count_solutions,
	      sizeof(solutions[0]),
	      &order_lamp_sets_asc);

	LampSet unique, next;

	unique = *solutions[--count_solutions];

	while (1) {
		write_lamp_set(output,
			       unique);

		do {
			if (count_solutions == 0)
				return;

			next = *solutions[--count_solutions];
		} while (!compare_lamp_sets(unique,
					    next));

		unique = next;
	}
}

static inline void
write_output(void)
{
	FILE *output;

	assert(output = fopen("lamps.out", "w"));

	if (count_solutions == 0)
		assert(fputs("IMPOSSIBLE\n", output) != EOF);
	else
		write_solutions(output);

	assert(fclose(output) == 0);
}

int
main(void)
{

	return 0;
}
