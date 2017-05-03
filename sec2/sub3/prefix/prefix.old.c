/*
ID: wrpaape1
LANG: C
TASK: prefix
*/

#include <stdio.h>
#include <assert.h>

#define PRIM_MIN		'A'
#define PRIM_MAX		'Z'
#define PRIM_SPAN		(PRIM_MAX + 1 - PRIM_MIN)
#define PRIM_COUNT_MAX		200
#define PRIM_LENGTH_MAX		10
#define SEQ_LENGTH_MAX		200000
#define SEQ_LINE_LENGTH_MAX	76

#define SIZE_PRIMS_MAX							\
(((PRIM_LENGTH_MAX + 2) * PRIM_COUNT_MAX))

#define SIZE_SEQ_MAX							\
(SEQ_LENGTH_MAX + (SEQ_LENGTH_MAX / SEQ_LINE_LENGTH_MAX))

#define SIZE_INPUT_BUFFER						\
(SIZE_PRIMS_MAX + 2 + SIZE_SEQ_MAX)

#define PRIM_NODE_COUNT_MAX						\
(PRIM_LENGTH_MAX * PRIM_COUNT_MAX)


#define IS_PRIM(TOKEN)	(((TOKEN) >= PRIM_MIN) && ((TOKEN) <= PRIM_MAX))
#define IS_DELIM(TOKEN) (((TOKEN) == ' ')      || ((TOKEN) == '\n'))


struct PrimNode {
	struct PrimNode *restrict next[PRIM_SPAN];
	unsigned int length;
};

static struct PrimNode node_buffer[PRIM_NODE_COUNT_MAX];


static struct PrimNode primitives;
static unsigned int longest_length;


static inline const unsigned char *
build_primitives(const unsigned char *restrict ptr)
{
	const unsigned char *restrict prim_begin;
	struct PrimNode *restrict *restrict node_ptr;
	struct PrimNode *restrict node;
	struct PrimNode *restrict alloc;
	unsigned int token;

	alloc = &node_buffer[0];
	token = *ptr;
	do {
		prim_begin = ptr;
		node       = &primitives;
		do {
			node_ptr = &node->next[token - PRIM_MIN];
			node     = *node_ptr;

			if (node == NULL) {
				node      = alloc++;
				*node_ptr = node;
			}

			token = *++ptr;
		} while (IS_PRIM(token));

		node->length = ptr - prim_begin;

		do {
			token = *++ptr;
		} while (IS_DELIM(token));
	} while (token != '.');

	return ptr + 2; /* skip ".\n" */
}


static inline unsigned int
solve(const unsigned char *restrict begin,
      const unsigned char *const restrict end)
{
	const unsigned char *restrict ptr;
	const struct PrimNode *restrict prev;
	const struct PrimNode *restrict next;
	unsigned int length;
	unsigned int longest_length;
	unsigned int token;
	unsigned int prev_length;
	size_t offset;

	longest_length = 0;

	do {
		ptr    = begin;
		prev   = &primitives;
		length = 0;

		do {
			token = *ptr;
			if (token == '\n')
				continue;

			offset = token - PRIM_MIN;
			next   = prev->next[offset];

			if (next == NULL) {
				prev_length = prev->length;

				if (prev_length == 0)
					break; /* not terminal */

				next = primitives.next[offset];

				if (next == NULL)
					break;

				length += prev_length;
			}

			prev = next;
		} while (++ptr < end);

		length += prev->length;

		if (length > longest_length)
			longest_length = length;

	} while (++begin < end);

	return longest_length;
}


int
main(void)
{
	static unsigned char input_buffer[SIZE_INPUT_BUFFER];
	FILE *input, *output;
	const unsigned char *restrict ptr;
	const unsigned char *restrict end;
	size_t size_read;

	assert(input = fopen("prefix.in", "r"));
	size_read = fread(&input_buffer[0], 1, sizeof(input_buffer), input);
	assert(size_read > 0);
	assert(ferror(input) == 0);
	assert(fclose(input) == 0);

	ptr = &input_buffer[0];
	end = ptr + size_read - 1; /* trim trailing \n */

	ptr = build_primitives(ptr);

	longest_length = solve(ptr, end);

	assert(output = fopen("prefix.out", "w"));
	assert(fprintf(output, "%u\n", longest_length) > 0);
	assert(fclose(output) == 0);

	return 0;
}
