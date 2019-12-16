/*
ID: wrpaape1
LANG: C
TASK: comehome
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>

#define TASK "comehome"

enum Limits {
	MIN_NUM_PATHS      = 1,
	MAX_NUM_PATHS      = 10000,
	MIN_DISTANCE       = 1,
	MAX_DISTANCE       = 10000,
	MIN_COW_PASTURE    = 'A',
	MAX_COW_PASTURE    = 'Z',
	NUM_COW_PASTURES   = MAX_COW_PASTURE + 1 - MIN_COW_PASTURE,
	MIN_EMPTY_PASTURE  = 'a',
	MAX_EMPTY_PASTURE  = 'z',
	NUM_EMPTY_PASTURES = MAX_EMPTY_PASTURE + 1 - MIN_EMPTY_PASTURE,
	NUM_PASTURES       = NUM_COW_PASTURES + NUM_EMPTY_PASTURES
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

static unsigned short
get_node(char pasture)
{
	if (isupper(pasture))
		return pasture - MIN_COW_PASTURE;

	return (pasture - MIN_EMPTY_PASTURE) + NUM_COW_PASTURES;
}

static unsigned short
read_num_paths(FILE *input)
{
	int num_paths = 0;
	read_file(input, "%d\n", &num_paths);
	assert(num_paths >= MIN_NUM_PATHS);
	assert(num_paths <= MAX_NUM_PATHS);
	return (unsigned short) num_paths;
}

static int
check_value(int value, int min_value, int max_value)
{
	return (value >= min_value) && (value <= max_value);
}

static void
check_pasture(char pasture)
{
	int is_cow_pasture = check_value(pasture,
					 MIN_COW_PASTURE,
					 MAX_COW_PASTURE);
	int is_empty_pasture = check_value(pasture,
					   MIN_EMPTY_PASTURE,
					   MAX_EMPTY_PASTURE);
	assert(is_cow_pasture || is_empty_pasture);
	(void) is_cow_pasture;
	(void) is_empty_pasture;
}

static void
check_distance(int distance)
{
	int is_valid_distance = check_value(distance,
					    MIN_DISTANCE,
					    MAX_DISTANCE);
	assert(is_valid_distance);
	(void) is_valid_distance;
}


static void
read_distance(FILE *input,
	      unsigned short distance[NUM_PASTURES][NUM_PASTURES])
{
	char pasture1       = '\0';
	char pasture2       = '\0';
	int read_distance12 = 0;
	read_file(input, "%c %c %d\n",
		  &pasture1, &pasture2, &read_distance12);
	check_pasture(pasture1);
	check_pasture(pasture2);
	check_distance(read_distance12);
	unsigned short node1      = get_node(pasture1);
	unsigned short node2      = get_node(pasture2);
	unsigned short distance12 = (unsigned short) read_distance12;

	unsigned short *distance_ptr = &distance[node1][node2];
	if (distance12 < *distance_ptr) {
		*distance_ptr          = distance12;
		distance[node2][node1] = distance12;
	}
}

static void
initialize(FILE *input,
	   unsigned short distance[NUM_PASTURES][NUM_PASTURES])
{
	for (unsigned short i = 0; i < NUM_PASTURES; ++i)
		for (unsigned short j = 0; j < NUM_PASTURES; ++j)
			distance[i][j] = USHRT_MAX;

	unsigned short num_paths = read_num_paths(input);
	for (unsigned short i = 0; i < num_paths; ++i)
		read_distance(input, distance);
}

static void
find_shortest_paths(const unsigned short distance[NUM_PASTURES][NUM_PASTURES],
		    unsigned short dest,
		    unsigned long path_lengths[NUM_PASTURES])
{
	for (unsigned short i = 0; i < NUM_PASTURES; ++i)
		path_lengths[i] = ULONG_MAX;

	path_lengths[dest] = 0;

	for (unsigned short num_iterations = 0, updated = 1; updated;
	     ++num_iterations) {
		assert(num_iterations <= NUM_PASTURES);
		updated = 0;
		for (unsigned short from = 0; from < NUM_PASTURES; ++from) {
			for (unsigned short to = 0; to < NUM_PASTURES; ++to) {
				unsigned long barn_from_to = path_lengths[from]
					+ distance[from][to];
				unsigned long *barn_to = &path_lengths[to];
				if (*barn_to > barn_from_to) {
					*barn_to = barn_from_to;
					updated = 1;
				}
			}
		}
	}
}

static void
solve(const unsigned short distance[NUM_PASTURES][NUM_PASTURES],
      char *cow_pasture,
      unsigned long *path_length)
{
	static const unsigned short BARN = NUM_COW_PASTURES - 1;

	unsigned long path_lengths[NUM_PASTURES];
	find_shortest_paths(distance,
			    BARN,
			    path_lengths);

	unsigned short fastest_cow         = 0;
	unsigned long shortest_path_length = ULONG_MAX;
	for (unsigned short cow = 0; cow < BARN; ++cow) {
		unsigned long cow_path_length = path_lengths[cow];
		if (cow_path_length < shortest_path_length) {
			shortest_path_length = cow_path_length;
			fastest_cow = cow;
		}
	}

	*cow_pasture = (char) (MIN_COW_PASTURE + fastest_cow);
	*path_length = shortest_path_length;
}

int
main(void)
{
	FILE *input = open_input();
	unsigned short distance[NUM_PASTURES][NUM_PASTURES];
	initialize(input, distance);
	close_file(input);

	char cow_pasture          = '\0';
	unsigned long path_length = 0;
	solve(distance,
	      &cow_pasture,
	      &path_length);

	FILE *output = open_output();
	write_file(output, "%c %lu\n",
		   cow_pasture, path_length);
	close_file(output);

	return 0;
}
