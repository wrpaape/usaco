/*
ID: wrpaape1
LANG: C
TASK: namenum
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <fcntl.h>	/* open */
#include <sys/stat.h>	/* fstat */
#include <sys/types.h>	/* needed for read */
#include <sys/uio.h>	/* needed for read */
#include <unistd.h>	/* STDOUT/IN/ERR_FILENO, read, write, close */
#include <assert.h>	/* assert */
#include <string.h>	/* memcmp, memcpy */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"namenum"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define DICT_FILE	"dict.txt"
#define MAX_NAMES	5000
#define SN_MAX_LENGTH	12
#define BUFFER_SIZE	((SN_MAX_LENGTH + 1) * MAX_NAMES)
#define MIN_DIGIT	'2'
#define NONE_OUTPUT	"NONE\n"


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct PermNode {
	const int *restrict map_key;
	char *restrict name_ptr;
};


/* function-like macros
 * ────────────────────────────────────────────────────────────────────────── */
#define OPEN_INPUT()							\
open(INPUT_FILE,							\
     O_RDONLY)

#define OPEN_DICT()							\
open(DICT_FILE,								\
     O_RDONLY)

#define OPEN_OUTPUT()							\
open(OUTPUT_FILE,							\
     O_CREAT | O_TRUNC | O_WRONLY,					\
     S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static char solution_buffer[BUFFER_SIZE];
static char *restrict solution_begin = &solution_buffer[0];
static char *restrict solution_end   = &solution_buffer[0];
static char dict_buffer[BUFFER_SIZE];
static char *restrict dict[MAX_NAMES];
static char *restrict *const restrict dict_begin = &dict[0];
static char *restrict *restrict dict_end         = &dict[0];
static struct PermNode nodes[SN_MAX_LENGTH];
static struct PermNode *const restrict nodes_begin = &nodes[0];
static struct PermNode *restrict nodes_end;

const int digit_map[][4] = {
	['2' - MIN_DIGIT] = { 'A', 'B', 'C', '\0' },
	['3' - MIN_DIGIT] = { 'D', 'E', 'F', '\0' },
	['4' - MIN_DIGIT] = { 'G', 'H', 'I', '\0' },
	['5' - MIN_DIGIT] = { 'J', 'K', 'L', '\0' },
	['6' - MIN_DIGIT] = { 'M', 'N', 'O', '\0' },
	['7' - MIN_DIGIT] = { 'P', 'R', 'S', '\0' },
	['8' - MIN_DIGIT] = { 'T', 'U', 'V', '\0' },
	['9' - MIN_DIGIT] = { 'W', 'X', 'Y', '\0' }
};

static char name_line[SN_MAX_LENGTH + 1]; /* +1 for \n' */
static size_t line_size;
static size_t count_digits;





/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
static inline void
put_name(void)
{
	(void) memcpy(solution_end,
		      &name_line[0],
		      line_size);

	solution_end += line_size;
}

static inline void
check_name(void)
{
	int compare;
	char *restrict *restrict names_begin;
	char *restrict *restrict name;
	char *restrict *restrict names_end;
	size_t offset;

	names_begin = dict_begin;
	names_end   = dict_end;

	while (names_begin != names_end) {
		offset = (names_end - names_begin) / 2;
		name   = names_begin + offset;

		compare = memcmp(*name,
				 &name_line[0],
				 count_digits);

		if (compare < 0) {
			names_begin = name + 1;

		} else if (compare > 0) {
			names_end   = name;

		} else {
			put_name();
			return;
		}
	}
}

static inline void
read_dict(void)
{
	int dict_fd;
	ssize_t dict_size;
	char *restrict ptr;
	char *restrict name_begin;
	char *restrict dict_buffer_end;

	/* read in buffer */
	dict_fd = OPEN_DICT();
	assert(dict_fd >= 0);
	dict_size = read(dict_fd,
			 &dict_buffer[0],
			 sizeof(dict_buffer));
	assert(dict_size > 0);
	assert(close(dict_fd) == 0);

	ptr		= &dict_buffer[0];
	dict_buffer_end = ptr + dict_size;

	do {
		name_begin = ptr;

		do {
			++ptr;
		} while (*ptr != '\n');

		/* add proper length names to dict */
		if ((ptr - name_begin) == count_digits)
			*dict_end++ = name_begin;

	} while (++ptr != dict_buffer_end);
}


static inline void
read_input(void)
{
	int input_fd;

	/* read in buffer */
	input_fd = OPEN_INPUT();
	assert(input_fd >= 0);
	line_size = read(input_fd,
			 &name_line[0],
			 sizeof(name_line));
	assert(((ssize_t) line_size) > 0);
	assert(close(input_fd) == 0);

	count_digits = line_size - 1;
}

static void
do_permute(struct PermNode *const restrict perm_ptr)
{
	int token;
	const int *restrict map_key;
	char *restrict name_ptr;
	struct PermNode *restrict next_perm_ptr;

	if (perm_ptr == nodes_end) {
		check_name();
		return;
	}

	map_key  = perm_ptr->map_key;
	name_ptr = perm_ptr->name_ptr;

	next_perm_ptr = perm_ptr + 1;

	while (1) {
		do_permute(next_perm_ptr);

		token = *++map_key;

		if (token == '\0')
			break;

		*name_ptr = token;
	}

	/* reset map_key */
	map_key -= 3;
	perm_ptr->map_key = map_key;
	*name_ptr         = *map_key;
}

static inline void
solve(void)
{
	struct PermNode *restrict perm_ptr;
	char *restrict name_line_ptr;
	const int *restrict map_key;
	int digit;

	nodes_end    = nodes_begin + count_digits;
	perm_ptr     = nodes_begin;

	name_line_ptr = &name_line[0];

	do {
		digit          = *name_line_ptr;
		map_key        = &digit_map[digit - MIN_DIGIT][0];
		*name_line_ptr = *map_key;

		perm_ptr->map_key  = map_key;
		perm_ptr->name_ptr = name_line_ptr++;
	} while (++perm_ptr != nodes_end);

	do_permute(nodes_begin);
}


static inline void
write_output(void)
{
	size_t solution_size;
	int output_fd;

	output_fd = OPEN_OUTPUT();
	assert(output_fd >= 0);

	if (solution_begin == solution_end) {
		solution_begin = NONE_OUTPUT;
		solution_size  = sizeof(NONE_OUTPUT) - 1;

	} else {
		solution_size  = solution_end - solution_begin;
	}

	assert(write(output_fd,
		     solution_begin,
		     solution_size) == (ssize_t) solution_size);

	assert(close(output_fd) == 0);
}


int
main(void)
{
	read_input();

	read_dict();

	solve();

	write_output();

	return 0;
}
