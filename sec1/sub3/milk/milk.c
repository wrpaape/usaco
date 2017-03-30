/*
ID: wrpaape1
LANG: C
TASK: milk
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdio.h>  /* fdopen, fdclose, fscanf */
#include <stdlib.h> /* qsort */
#include <assert.h> /* assert */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"milk"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define MAX_FARMERS	5000
#define MAX_PRICE	1000


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static unsigned int rem_units;
static unsigned int total;
static unsigned int milk_map[MAX_PRICE + 1];


static inline void
read_input(void)
{
	FILE *input;
	unsigned int count_farmers;
	unsigned int i;
	unsigned int price;
	unsigned int amount;

	assert(input = fopen(INPUT_FILE, "r"));

	/* read in count farmers */
	assert(fscanf(input, "%u %u\n",
		      &rem_units,
		      &count_farmers) == 2);

	for (i = 0; i < count_farmers; ++i) {
		assert(fscanf(input, "%u %u\n",
			      &price,
			      &amount) == 2);

		milk_map[price] += amount;
	}
}

static inline void
write_output(void)
{
	FILE *output;
	assert(output = fopen(OUTPUT_FILE, "w"));
	assert(fprintf(output, "%u\n",
		       total) >= 0);
	assert(fclose(output) == 0);
}


static inline void
solve(void)
{
	unsigned int price;
	unsigned int amount;

	price = 0;

	while (1) {
		amount = milk_map[price];

		if (amount >= rem_units)
			break;

		rem_units -= amount;

		total += (price * amount);

		++price;
	}

	total += (price * rem_units);
}

int
main(void)
{
	read_input();

	solve();

	write_output();

	return 0;
}
