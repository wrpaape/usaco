/*
ID: wrpaape1
LANG: C
TASK: transform
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <fcntl.h>	/* open */
#include <sys/stat.h>	/* fstat */
#include <sys/types.h>	/* needed for read */
#include <sys/uio.h>	/* needed for read */
#include <unistd.h>	/* STDOUT/IN/ERR_FILENO, read, write, close */
#include <assert.h>	/* assert */
#include <stdlib.h>	/* exit */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"transform"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define N_MAX		10


/* function like macros
 * ────────────────────────────────────────────────────────────────────────── */
#define OPEN_INPUT()							\
open(INPUT_FILE,							\
     O_RDONLY)

#define OPEN_OUTPUT()							\
open(OUTPUT_FILE,							\
     O_CREAT | O_TRUNC | O_WRONLY,					\
     S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)

#define DIGIT_TO_ASCII(DIGIT) ((char) ((DIGIT) | 48u))
#define ASCII_TO_DIGIT(ASCII) (((size_t) (ASCII)) & 15u)


/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
static size_t N;
static unsigned int min_transformations;

static unsigned int initial[N_MAX * N_MAX];
static unsigned int *restrict initial_begin = &initial[0];
static const unsigned int *restrict initial_end;

static unsigned int final[N_MAX * N_MAX];
static unsigned int *restrict final_begin = &final[0];
static const unsigned int *restrict final_end;


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
char *
put_min_transformations(char *restrict buffer)
{
	int tmp;
        char *restrict ptr;
        char *restrict end_ptr;

        ptr = buffer;

        /* write digits in reverse order */
        do {
                *ptr++ = DIGIT_TO_ASCII(min_transformations % 10);
                min_transformations /= 10;
        } while (min_transformations > 0);

        end_ptr = ptr; /* copy pointer beyond last digit */

        /* swap digits in place */
        while (--ptr > buffer) {
                tmp       = (int) *ptr;
                *ptr      = *buffer;
                *buffer++ = (char) tmp;
        }

        return end_ptr; /* done */
}


static inline void
write_output(void)
{
	char solution[21]; /* INT64_MAX + \n */
	char *restrict end_ptr;
	size_t output_size;
	int output_fd;

	output_fd = OPEN_OUTPUT();
	assert(output_fd >= 0);

	end_ptr    = put_min_transformations(&solution[0]);
	*end_ptr++ = '\n';

	output_size = end_ptr - &solution[0];

	assert(write(output_fd,
		     &solution[0],
		     output_size) == (ssize_t) output_size);

	assert(close(output_fd) == 0);
}

static inline void
read_input(void)
{
	int input_fd;
	char input_buffer[(2 * ((N_MAX * N_MAX) + N_MAX)) + 3];
	const char *restrict ptr;
	const char *restrict ptr_end;

	/* read in buffer */
	input_fd = OPEN_INPUT();
	assert(input_fd >= 0);
	assert(read(input_fd,
		    &input_buffer[0],
		    sizeof(input_buffer)) > 0);
	assert(close(input_fd) == 0);

	/* read in N */
	ptr = &input_buffer[0];

	N = ASCII_TO_DIGIT(*ptr++);

	if (N == 0) {
		if (*ptr == '\n') {
			min_transformations = 0;
			write_output();
		}
	}



}


static inline void
solve(void)
{
}

int
main(void)
{
	read_input();

	solve();

	write_output();

	return 0;
}
