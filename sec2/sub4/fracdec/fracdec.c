/*
ID: wrpaape1
LANG: C
TASK: fracdec
*/

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>

#define TASK "fracdec"

enum Limits {
	MIN_NUMBER          = 1,
	MAX_NUMBER          = 100000,
	MAX_LINE_LENGTH     = 76,
	MAX_LENGTH_REPETEND = MAX_NUMBER - 1,
	LENGTH_MAX_NUMBER   = 6,
	SIZE_SOLUTION_BUFFER  = LENGTH_MAX_NUMBER /* whole number */
		+ 1 /* decimal */
		+ MAX_LENGTH_REPETEND * 2 /* repetend */
		+ MAX_LENGTH_REPETEND /* room for single repeat */
		+ LENGTH_MAX_NUMBER /* repetend overflow */
		+ 2 /* parentheses */
		+ 1 /* \0 */
};

static FILE *
open_file(const char *filename,
	  const char *mode)
{
	FILE *file = fopen(filename, mode);
	assert(file);
	return file;
}

static FILE *
open_input(void)
{
	return open_file(TASK ".in", "r");
}

static FILE *
open_output(void)
{
	return open_file(TASK ".out", "w");
}

void
read_file(FILE *file,
	  const char *format,
	  ...)
{
	va_list args;
	va_start(args, format);
	int num_items = vfscanf(file, format, args);
	va_end(args);
	assert(num_items >= 0);
	(void) num_items;
}

void
write_file(FILE *file,
	   const char *format,
	   ...)
{
	va_list args;
	va_start(args, format);
	int num_chars = vfprintf(file, format, args);
	va_end(args);
	assert(num_chars >= 0);
	(void) num_chars;
}

static void
close_file(FILE *file)
{
	int error = fclose(file);
	assert(error == 0);
	(void) error;
}

static int
check_value(int value, int min_value, int max_value)
{
	return (value >= min_value) && (value <= max_value);
}

static void
check_number(int number)
{
	int is_valid = check_value(number, MIN_NUMBER, MAX_NUMBER);
	assert(is_valid);
	(void) is_valid;
}


static void
read_numbers(FILE *input,
	     unsigned long *numerator,
	     unsigned long *denominator)
{
	long read_numerator   = 0;
	long read_denominator = 0;
	read_file(input, "%lu %lu\n",
		  &read_numerator, &read_denominator);
	check_number(read_numerator);
	check_number(read_denominator);

	*numerator   = (unsigned long) read_numerator;
	*denominator = (unsigned long) read_denominator;
}

static void
read_input(unsigned long *numerator,
	   unsigned long *denominator)
{
	FILE *input = open_input();
	read_numbers(input, numerator, denominator);
	close_file(input);

}

static void
reverse(char *begin,
	char *end)
{
	while (begin < end) {
		char tmp = *begin;
		*begin++ = *--end;
		*end = tmp;
	}
}

static char *
write_integer(unsigned long digits,
	      char *cursor)
{
	char *start = cursor;
	while (1) {
		unsigned long div = digits / 10;
		unsigned long rem = digits % 10;
		*cursor++ = (char) (rem | '0');
		if (div == 0)
			break;

		digits = div;
	}

	reverse(start, cursor);

	return cursor;
}

static char *
write_rem_digits(unsigned long rem,
		 unsigned long denominator,
		 const char *write_boundary,
		 char *cursor)
{
	if (rem == 0) {
		*cursor++ = '0';
		return cursor;
	}

	do {
		while (1) {
			rem *= 10;
			if (rem >= denominator)
				break;
			*cursor++ = '0';
			if (cursor >= write_boundary)
				return cursor;
		}
		unsigned long div = rem / denominator;
		rem %= denominator;

		*cursor++ = (char) (div | '0');
	} while ((rem > 0) && (cursor < write_boundary));
	return cursor;
}

static int
have_repeat(const char *cursor,
	    unsigned long length_repetend)
{
	const char *string1 = cursor - length_repetend;
	const char *string2 = string1 - length_repetend;
	return strncmp(string1, string2, length_repetend) == 0;
}

static char *
find_first_repeat(char *cursor,
		  const char *repeat,
		  unsigned long length_repetend)
{
	for (; strncmp(cursor, repeat, length_repetend) != 0; ++cursor)
		; /* spin */
	return cursor;
}

static char *
find_repetend(char *cursor,
	      const char *repeat,
	      unsigned long length_repetend)
{
	char *first_repeat = find_first_repeat(cursor,
					       repeat,
					       length_repetend);
	char *repetend = first_repeat;
	while (1) {
		char *next_repetend = repetend - 1;
		--repeat;
		if (strncmp(next_repetend, repeat, length_repetend) != 0)
			break;

		repetend = next_repetend;
	}
	return repetend;
}

static char *
wrap_repetend(char *repetend,
	      unsigned long length_repetend)
{
	(void) memmove(repetend + 1,
		       repetend,
		       length_repetend);
	*repetend = '(';
	repetend[length_repetend + 1] = ')';
	return repetend + length_repetend + 2;
}

static char *
place_parentheses(char *begin_fraction,
		  char *cursor,
		  unsigned long max_length_repetend)
{
	while (*--cursor == '0')
		; /* rewind to 1 past last nonzero digit */
	++cursor;

	unsigned long length_repetend = 1;
	while (1) {
		assert(length_repetend <= max_length_repetend);
		if (have_repeat(cursor, length_repetend))
			break;

		++length_repetend;
	}

	const char *repeat = cursor - length_repetend;
	char *repetend = find_repetend(begin_fraction,
				       repeat,
				       length_repetend);

	return wrap_repetend(repetend,
			     length_repetend);

}

static unsigned long
solve(unsigned long numerator,
      unsigned long denominator,
      char solution[SIZE_SOLUTION_BUFFER])
{
	unsigned long div = numerator / denominator;
	unsigned long rem = numerator % denominator;
	char *cursor = write_integer(div, solution);
	*cursor++ = '.';
	char *begin_fraction = cursor;
	unsigned long max_length_repetend = denominator;
	unsigned long max_digits = max_length_repetend * 2;
	const char *write_boundary = cursor + max_digits;
	cursor = write_rem_digits(rem, denominator,
				  write_boundary, cursor);
	if (cursor >= write_boundary)
		cursor = place_parentheses(begin_fraction,
					   cursor,
					   max_length_repetend);

	return cursor - &solution[0];
}

static void
write_solution(FILE *output,
	     const char solution[SIZE_SOLUTION_BUFFER],
	     unsigned long length_solution)
{
	unsigned long line_length = 0;
	for (unsigned long rem_length = length_solution; rem_length > 0;
	     rem_length -= line_length) {
		line_length = MAX_LINE_LENGTH;
		if (rem_length < line_length)
			line_length = rem_length;

		write_file(output, "%.*s\n",
			   (int) line_length,
			   &solution[length_solution - rem_length]);
	}

}

static void
write_output(char solution[SIZE_SOLUTION_BUFFER],
	     unsigned long length_solution)
{
	FILE *output = open_output();
	write_solution(output, solution, length_solution);
	close_file(output);
}

int
main(void)
{
	unsigned long numerator   = 0;
	unsigned long denominator = 0;
	read_input(&numerator, &denominator);

	char solution[SIZE_SOLUTION_BUFFER];
	unsigned long length_solution = solve(numerator,
					      denominator,
					      solution);
	write_output(solution,
		     length_solution);

	return 0;
}
