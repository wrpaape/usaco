/*
ID: wrpaape1
LANG: C
TASK: lamps
*/

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define N_MAX			100
#define COUNT_BUTTONS		4
#define COUNT_SOLUTIONS_MAX	(1 << COUNT_BUTTONS)

#define ODD_PATTERN							\
(  (1 <<  0) | (1 <<  2) | (1 <<  4) | (1 <<  6)			\
 | (1 <<  8) | (1 << 10) | (1 << 12) | (1 << 14))
#define EVEN_PATTERN (~ODD_PATTERN)

typedef unsigned long LampBlock;

#define LAMP_BLOCK_MAX		ULONG_MAX
#define LAMP_BLOCK_ONE		1UL
#define LAMP_BLOCK_BIT		(sizeof(LampBlock) * CHAR_BIT)
#define LAMP_BLOCK_MAX_BIT	(LAMP_BLOCK_ONE << (LAMP_BLOCK_BIT - 1))
#define LAMP_LENGTH(N)							\
(((N) / LAMP_BLOCK_BIT) + (((N) % LAMP_BLOCK_BIT) > 0))

typedef LampBlock LampSet[LAMP_LENGTH(N_MAX)];

static LampSet buttons[COUNT_BUTTONS];

static LampSet candidate;
static LampSet final_on;
static LampSet final_off;
static LampSet solutions_buffer[COUNT_SOLUTIONS_MAX];
static LampSet *solutions[COUNT_SOLUTIONS_MAX];
static size_t count_solutions;

static size_t N;
static size_t lamp_length;
static size_t lamp_size;
static unsigned int max_switch_count;
static LampBlock last_max_bit;

static inline int
compare_lamp_bytes(const int byte1,
		   const int byte2)
{
	int bit1, bit2, bit;

	bit = 1;

	while (1) {
		bit1 = byte1 & bit;
		bit2 = byte2 & bit;

		if (bit1 ^ bit2)
			return bit1 ? 1 : -1;

		bit <<= 1;
	}
}

static inline int
compare_lamp_blocks(const LampBlock block1,
		    const LampBlock block2)
{
	int byte1, byte2;
	unsigned int shift;

	shift = 0;
	while (1) {
		byte1 = (unsigned char) (block1 >> shift);
		byte2 = (unsigned char) (block2 >> shift);

		if (byte1 != byte2)
			return compare_lamp_bytes(byte1,
						  byte2);

		shift += CHAR_BIT;
	}
}

static inline int
compare_lamp_sets(const LampSet set1,
		  const LampSet set2)
{
	LampBlock block1, block2;
	size_t i;

	i = 0;
	do {
		block1 = set1[i];
		block2 = set2[i];

		if (block1 != block2)
			return compare_lamp_blocks(block1,
						   block2);
	} while (++i < lamp_length);

	return 0;
}

int
order_lamp_sets_asc(const void *key1,
		    const void *key2)
{
	const LampSet *restrict *set1 = (const LampSet *restrict *) key1;
	const LampSet *restrict *set2 = (const LampSet *restrict *) key2;

	return compare_lamp_sets(**set1,
				 **set2);
}

static inline void
check_solution(void)
{
	size_t i;
	LampBlock cand_block, on_block, off_block;
	LampSet *restrict sol_ptr;

	i = 0;
	do {
		cand_block = candidate[i];
		on_block   = final_on[i];

		if ((cand_block & on_block) != on_block)
			return; /* not enough required lamps on */

		off_block  = final_off[i];

		if ((cand_block & off_block))
			return; /* some lamps on that should be off */

	} while (++i < lamp_length);

	/* pass, append to solutions */

	sol_ptr			     = &solutions_buffer[count_solutions];
	solutions[count_solutions++] = sol_ptr;

	(void) memcpy(*sol_ptr,
		      candidate,
		      lamp_size);

}

static inline void
toggle_button(const LampSet button)
{
	size_t i;

	i = 0;
	do {
		candidate[i] ^= button[i];
	} while (++i < lamp_length);
}

static inline void
put_lamp(LampSet set,
	 const unsigned int lamp)
{
	size_t lamp_div, lamp_rem;

	lamp_div = lamp / LAMP_BLOCK_BIT;
	lamp_rem = lamp % LAMP_BLOCK_BIT;

	set[lamp_div] |= (LAMP_BLOCK_ONE << lamp_rem);
}

void
solve(const size_t)

static inline void
read_input(void)
{
	FILE *input;
	size_t lamp_rem;
	int lamp;
	unsigned int C, skip_lamp;
	LampSet *restrict skip_set_ptr;

	assert(input = fopen("lamps.in", "r"));
	assert(fscanf(input, "%zu\n%u\n", &N, &C) == 2);

	lamp_length = N / LAMP_BLOCK_BIT;
	lamp_rem    = N % LAMP_BLOCK_BIT;
	lamp_size   = (sizeof(LampBlock) * lamp_length);

	if (lamp_rem == 0) {
		last_max_bit   = LAMP_BLOCK_MAX_BIT;

	} else {
		++lamp_length;
		lamp_size    += (  (lamp_rem / CHAR_BIT)
				 + ((lamp_rem % CHAR_BIT) > 0));
		last_max_bit  = LAMP_BLOCK_ONE << (lamp_rem - 1);
	}

	max_switch_count = (C < COUNT_BUTTONS) ? C : COUNT_BUTTONS;

	while (1) {
		assert(fscanf(input, "%d", &lamp) == 1);

		if (lamp < 0)
			break;

		put_lamp(final_on, lamp - 1);
	}

	while (1) {
		assert(fscanf(input, "%d", &lamp) == 1);

		if (lamp < 0)
			break;

		put_lamp(final_off, lamp - 1);
	}

	assert(fclose(input) == 0);

	/* set buttons */

	/* all lamps */
	(void) memset(&buttons[0][0],
		      UCHAR_MAX,
		      lamp_size);


	/* "odd" (if 1-based index) lamps */
	(void) memset(&buttons[1][0],
		      ODD_PATTERN,
		      lamp_size);

	/* "even" (if 1-based index) lamps */
	(void) memset(&buttons[2][0],
		      EVEN_PATTERN,
		      lamp_size);

	/* 3 X K + 1 (if 1-based index) lamps */
	skip_set_ptr = &buttons[3];
	skip_lamp    = 0;
	do {
		put_lamp(*skip_set_ptr,
			 skip_lamp);
		skip_lamp += 3;
	} while (skip_lamp < N);
}

static inline void
write_lamp_block(FILE *output,
		 const LampBlock block,
		 const LampBlock bit_max)
{
	int token;
	LampBlock bit;

	bit = 1;
	while (1) {
		token = '0' + ((block & bit) != 0);

		assert(fputc(token, output) != EOF);

		if (bit == bit_max)
			return;

		bit <<= 1;
	}
}

static inline void
write_lamp_set(FILE *output,
	       const LampSet set)
{
	LampBlock block;
	size_t i;

	i   = 0;
	while (1) {
		block = set[i];

		if (++i == lamp_length)
			break;

		write_lamp_block(output,
				 block,
				 LAMP_BLOCK_MAX_BIT);
	}

	write_lamp_block(output,
			 block,
			 last_max_bit);

	assert(fputc('\n', output) != EOF);
}

static inline void
write_solutions(FILE *output)
{
	LampSet *restrict unique;
	LampSet *restrict next;

	if (count_solutions == 0) {
		assert(fputs("IMPOSSIBLE\n", output) != EOF);
		return;
	}

	qsort(&solutions[0],
	      count_solutions,
	      sizeof(solutions[0]),
	      &order_lamp_sets_asc);

	unique = solutions[--count_solutions];

	while (1) {
		write_lamp_set(output,
			       *unique);

		do {
			if (count_solutions == 0)
				return;

			next = solutions[--count_solutions];
		} while (!compare_lamp_sets(*unique,
					    *next));

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
check_next_comb(unsigned int rem_switches,
		unsigned int button_id)
{
	LampSet *restrict button_ptr;

	if (rem_switches == 0) {
		check_solution();
		return;
	}

	--rem_switches;

	while (button_id < COUNT_BUTTONS) {
		button_ptr = &buttons[button_id];

		check_next_comb()

		toggle_button(*button_ptr);


		toggle_button
	}
}

static inline void
solve(void)
{
	/* turn on all lamps for candidate */
	(void) memset(&candidate[0],
		      UCHAR_MAX,
		      lamp_size);

	check_next_comb(max_switch_count, 0);
}

int
main(void)
{
	read_input();

	solve();

	write_output();

	return 0;
}
