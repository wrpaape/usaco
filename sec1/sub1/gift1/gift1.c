/*
ID: wrpaape1
LANG: C
TASK: gift1
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <fcntl.h>	/* open */
#include <sys/stat.h>	/* fstat */
#include <sys/types.h>	/* needed for read */
#include <sys/uio.h>	/* needed for read */
#include <unistd.h>	/* STDOUT/IN/ERR_FILENO, read, write, close */
#include <string.h>     /* memcmp, memcpy */
#include <assert.h>	/* assert */
#include <stdlib.h>	/* malloc, free, strtol */
#include <errno.h>


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"gift1"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define MAX_GROUP	10
#define MAP_SIZE	16 /* next power of two */
#define MAP_SIZE_M1	(MAP_SIZE - 1)


/* function like macros
 * ────────────────────────────────────────────────────────────────────────── */
#define OPEN_INPUT()							\
open(INPUT_FILE,							\
     O_RDONLY)

#define OPEN_OUTPUT()							\
open(OUTPUT_FILE,							\
     O_CREAT | O_TRUNC | O_WRONLY,					\
     S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)

#define DIGIT_TO_ASCII(DIGIT) ((char) ((DIGIT) | 48u)) /* 0...9 → '0'...'9' */


/* typedefs, struct declarations
 * ────────────────────────────────────────────────────────────────────────── */
struct Key {
	unsigned long hash;
	size_t length;
	const void *restrict name;
};

struct MapNode {
	struct Key key;
	long total;
	const struct MapNode *restrict next;
};

/* global variables
 * ────────────────────────────────────────────────────────────────────────── */
char *restrict input_begin;
const char *restrict input_end;
char *restrict entries_end;
static struct MapNode *restrict map[MAP_SIZE];
static struct MapNode nodes[MAX_GROUP];
static struct MapNode *restrict node_ptr = &nodes[0];


/* helper functions
 * ────────────────────────────────────────────────────────────────────────── */
char *
put_total(char *restrict buffer,
	  const long total)
{
	int tmp;
        char *restrict ptr;
        char *restrict end_ptr;

        if (total < 0) {
                total = -total;  /* ensure total is non-negative */
                *buffer++ = '-'; /* add sign */
        }

        ptr = buffer;

        /* write digits in reverse order */
        do {
                *ptr++ = DIGIT_TO_ASCII(total % 10);
                total /= 10;
        } while (total > 0);

        end_ptr = ptr; /* copy pointer beyond last digit */

        /* swap digits in place */
        while (--ptr > buffer) {
                tmp       = (int) *ptr;
                *ptr      = *buffer;
                *buffer++ = (char) tmp;
        }

        return end_ptr; /* done */
}


static inline unsigned long
key_init(struct Key *const restrict key,
	 const char *const restrict name_begin,
	 const char *const restrict name_end)
{
	unsigned long hash;
	const unsigned char *restrict ptr;

	hash = 0;
	ptr  = (const unsigned char *) name_begin;

	while (((const char *) ptr) < name_end) /* 0 length name possible */
		hash = 33 * hash ^ ((unsigned long) *ptr++);

	key->hash   = hash;
	key->name   = (const void *) name_begin;
	key->length = name_end - name_begin;

	return hash;
}

static inline int
keys_equal(const struct Key *const restrict key1,
	   const struct Key *const restrict key2)
{
	return (key1->hash   == key2->hash)
	    && (key1->length == key2->length)
	    && (memcmp(key1->name,
		       key2->name,
		       key1->length) == 0);
}

static inline void
make_entry(const char *const restrict name_begin,
	   const char *const restrict name_end)
{
	const struct MapNode *restrict *restrict bucket;
	unsigned long hash;

	hash = key_init(&node_ptr->key,
			name_begin,
			name_end);

	bucket	       = &map[hash & MAP_SIZE_M1];
	node_ptr->next = *bucket;
	*bucket        = node_ptr++; /* advance buffer */
}


static inline long *
get_total(const char *const restrict name_begin,
	  const char *const restrict name_end)
{
	struct Key key;
	unsigned long hash;
	struct MapNode *restrict node;

	hash = key_init(&key,
			name_begin,
			name_end);

	node = map[hash & MAP_SIZE_M1];

	while (!keys_equal(&key,
			   &node->key))
		node = node->next;

	return &node->total;
}


static inline void
read_input(void)
{
	int input_fd;
	size_t input_size;
	struct stat stat_buffer;

	input_fd = OPEN_INPUT();

	assert(input_fd >= 0);

	assert(fstat(input_fd,
		     &stat_buffer) == 0);

	input_size = stat_buffer.st_size;

	assert(input_begin = malloc(input_size));

	assert(read(input_fd,
		    input_begin,
		    input_size) == (ssize_t) input_size);

	assert(close(input_fd) == 0);

	input_end = input_begin + input_size;
}

static inline void
solve(void)
{
	const char *restrict ptr;
	const char *restrict name_begin;
	long group_size;

	/* read in entries */
	ptr = input_begin;

	group_size = strtol(ptr, &ptr, 10);

	do {
		name_begin = ++ptr; /* skip newline */

		while (*ptr != '\n')
			++ptr;

		make_entry(name_begin,
			   ptr);
	} while (--group_size > 0);

	entries_end = ptr;

	/* read in gifts */
}

static inline void
write_output(void)
{
	const struct MapNode *restrict node;
	char *restrict ptr;
	size_t name_length;
	size_t output_size;
	int output_fd;

	node = &nodes[0];
	ptr  = entries_end; /* overwrite input buffer after entries */
	do {
		name_length = node->key.length;
		(void) memcpy(ptr,
			      node->key.name,
			      name_length);
		ptr   += name_length;
		*ptr++ = ' ';
		ptr    = put_total(ptr,
				   node->total);
		*ptr++ = '\n';
	} while (++node < node_ptr);

	output_fd = OPEN_OUTPUT();

	assert(output_fd >= 0);

	output_size = ptr - gifts_begin;

	assert(write(output_fd,
		     gifts_begin,
		     output_size) == (ssize_t) output_size);

	assert(close(output_fd) == 0);

	free(input_begin);
}


int
main(void)
{
	read_input();

	solve();

	write_output();

	return 0;
}
