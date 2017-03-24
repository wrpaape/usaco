/*
ID: wrpaape1
LANG: C
TASK: friday
*/

#include <stdio.h>  /* fdopen, fdclose, fscanf */
#include <stdlib.h> /* exit */

static inline void
exit_on_failure(const char *const restrict failure)
{
	perror(failure);
	exit(1);
}

/* 0 ... 6 -> Saturday ... Friday */
static unsigned int fri_13_counts[7];

static unsigned int year = 1900;
static unsigned int day  = 2; /* Monday */

#define ADVANCE_DAY() ++day, day %= 7

static inline void
process_month(const unsigned int month_days)
{
	unsigned int month_day;

	month_day = 1;

	do {
		ADVANCE_DAY();
	} while (++month_day < 13);

	++fri_13_counts[day];

	do {
		ADVANCE_DAY();
	} while (++month_day <= month_days);
}

static inline void
process_year(void)
{
	unsigned int is_leap_year;

	process_month(31); /* January */

	is_leap_year = (   ((year % 4) == 0)
		        && (   ((year % 100) != 0)
			    || ((year % 400) == 0)));

	process_month(28 + is_leap_year); /* February */
	process_month(31); /* March */
	process_month(30); /* April */
	process_month(31); /* May */
	process_month(30); /* June */
	process_month(31); /* July */
	process_month(31); /* August */
	process_month(30); /* September */
	process_month(31); /* October */
	process_month(30); /* November */
	process_month(31); /* December */
}

int
main(void)
{
	FILE *input;
	FILE *output;
	unsigned int N;
	unsigned int year_until;

	input = fopen("friday.in", "r");
	if (input == NULL)
		exit_on_failure("fopen(\"friday.in\")");

	if (fscanf(input, "%u", &N) != 1)
		exit_on_failure("fscanf");

	if (fclose(input) != 0)
		exit_on_failure("fclose(input)");

	year_until = year + N;

	do {
		process_year();
		++year;
	} while (year < year_until);

	output = fopen("friday.out", "w");
	if (output == NULL)
		exit_on_failure("fopen(\"friday.out\")");

	if (fprintf(output,
		    "%u %u %u %u %u %u %u\n",
		    fri_13_counts[0],
		    fri_13_counts[1],
		    fri_13_counts[2],
		    fri_13_counts[3],
		    fri_13_counts[4],
		    fri_13_counts[5],
		    fri_13_counts[6]) < 0)
		exit_on_failure("fprintf");

	if (fclose(output) != 0)
		exit_on_failure("fclose(output)");

	return 0;
}
