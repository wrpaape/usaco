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


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Farmer {
	int price;
	int amount;
};


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static size_t count_farmers;
static int rem_units;
static int total;

static struct Farmer farmers[MAX_FARMERS];
static struct Farmer *const restrict farmers_begin = &farmers[0];
static struct Farmer *restrict farmers_end;


static inline void
read_input(void)
{
	FILE *input;
	struct Farmer *restrict farmer;

	assert(input = fopen(INPUT_FILE, "r"));

	/* read in count farmers */
	assert(fscanf(input, "%d %zu\n",
		      &rem_units,
		      &count_farmers) == 2);

	farmers_end = farmers_begin + count_farmers;
	farmer	    = farmers_begin;

	for (farmer = farmers_begin; farmer < farmers_end; ++farmer)
		assert(fscanf(input, "%d %d\n",
			      &farmer->price,
			      &farmer->amount) == 2);
}

static inline void
write_output(void)
{
	FILE *output;
	assert(output = fopen(OUTPUT_FILE, "w"));
	assert(fprintf(output, "%d\n",
		       total) >= 0);
	assert(fclose(output) == 0);
}


/* Farmer comparison functions
 * ────────────────────────────────────────────────────────────────────────── */
int
order_by_asc_price(const void *farmer1,
		   const void *farmer2)
{	/* 'price' is 1st field, 0 offset from farmer pointer */
	const int price1 = *((const int *) farmer1);
	const int price2 = *((const int *) farmer2);;
	return price1 - price2;
}

static inline void
solve(void)
{
	struct Farmer *restrict farmer;
	int price;
	int amount;

	/* sort farmers on 'price' in ascending order */
	qsort(farmers_begin,
	      count_farmers,
	      sizeof(struct Farmer),
	      &order_by_asc_price);

	farmer = farmers_begin;

	while (1) {
		price  = farmer->price;
		amount = farmer->amount;

		if (amount >= rem_units)
			break;

		rem_units -= amount;

		total += (price * amount);

		++farmer;
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
