/*
ID: wrpaape1
LANG: C
TASK: lamps
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
/* #include <string.h> */

#define N_MAX			100
#define COUNT_BUTTONS		4
#define COUNT_SOLUTIONS_MAX	(1 << COUNT_BUTTONS)
#define LAMP_BLOCK_LENGTH	6
#define ALL_LAMPS							\
(1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5)

static size_t N;
static unsigned int max_switch_count;
static unsigned int solutions[COUNT_SOLUTIONS_MAX];
static size_t count_solutions;


static unsigned int buttons[COUNT_BUTTONS] = {
	/* all lamps */
	ALL_LAMPS,
	/* odd lamps */
	(1 << 0) | (1 << 2) | (1 << 4),
	/* even lamps */
	(1 << 1) | (1 << 3) | (1 << 5),
	/* 3 x K + 1 lamps*/
	(1 << 0) | (1 << 3)
};

static unsigned int final_on;
static unsigned int final_off;

int
order_lamp_sets_asc(const void *key1,
		    const void *key2)
{
	unsigned int bit, bit1, bit2;

	const unsigned int lamp1 = *((const unsigned int *) key1);
	const unsigned int lamp2 = *((const unsigned int *) key2);

	for (bit = 1; bit < (1 << LAMP_BLOCK_LENGTH); bit <<= 1) {
		bit1 = lamp1 & bit;
		bit2 = lamp2 & bit;

		if (bit1 ^ bit2)
			return bit1 ? 1 : -1;
	}

	return 0;
}

static inline void
check_solution(const unsigned int candidate)
{
	if (   ((candidate & final_off) == 0)
	    && ((candidate & final_on) == final_on))
		solutions[count_solutions++] = candidate;
}

static inline unsigned int
map_lamp(const unsigned int lamp)
{
	return 1 << (lamp % LAMP_BLOCK_LENGTH);
}

static inline void
write_lamp_set(FILE *output,
	       const unsigned int lamp_set);

static inline void
read_input(void)
{
	FILE *input;
	int lamp;
	unsigned int C;

	assert(input = fopen("lamps.in", "r"));
	assert(fscanf(input, "%zu\n%u\n", &N, &C) == 2);

	max_switch_count = (C < COUNT_BUTTONS) ? C : COUNT_BUTTONS;

	while (1) {
		assert(fscanf(input, "%d", &lamp) == 1);

		if (lamp < 0)
			break;

		final_on |= map_lamp(lamp - 1);
	}

	while (1) {
		assert(fscanf(input, "%d", &lamp) == 1);

		if (lamp < 0)
			break;

		final_off |= map_lamp(lamp - 1);
	}

	assert(fclose(input) == 0);
}

static inline void
write_lamp_set(FILE *output,
	       const unsigned int lamp_set)
{
	unsigned int i, token;

	for (i = 0; i < N; ++i) {
		token = '0' + ((lamp_set & map_lamp(i)) != 0);

		assert(fputc(token, output) != EOF);
	}

	assert(fputc('\n', output) != EOF);
}

static inline void
write_solutions(FILE *output)
{
	unsigned int unique, next;
	size_t i;

	if (count_solutions == 0) {
		assert(fputs("IMPOSSIBLE\n", output) != EOF);
		return;
	}

	qsort(&solutions[0],
	      count_solutions,
	      sizeof(solutions[0]),
	      &order_lamp_sets_asc);

	unique = solutions[0];
	i      = 0;
	while (1) {
		write_lamp_set(output,
			       unique);

		do {
			if (++i == count_solutions)
				return;

			next = solutions[i];
		} while (next == unique);

		unique = next;
	}
}

static inline void
write_output(void)
{
	FILE *output;

	assert(output = fopen("lamps.out", "w"));

	write_solutions(output);

	assert(fclose(output) == 0);
}

void
check_next_candidate(const unsigned int candidate,
		     unsigned int rem_switches,
		     unsigned int button_id)
{
	unsigned int next_candidate;

	check_solution(candidate);

	if (rem_switches == 0)
		return;

	--rem_switches;

	while (button_id < COUNT_BUTTONS) {
		next_candidate = candidate ^ buttons[button_id++];

		check_next_candidate(next_candidate,
				     rem_switches,
				     button_id);
	}
}


int
main(void)
{
	read_input();

	check_next_candidate(ALL_LAMPS,
			     max_switch_count,
			     0);

	write_output();

	return 0;
}
