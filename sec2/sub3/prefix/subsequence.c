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
	size_t length;
	struct PrimNode *restrict next[PRIM_SPAN];
};

static struct PrimNode node_buffer[PRIM_NODE_COUNT_MAX];
static struct PrimNode *restrict primitives[PRIM_SPAN];

static size_t max_prim_length;
static size_t max_prefix_length;






static inline unsigned char *
build_primitives(const unsigned char *restrict ptr)
{
	const unsigned char *restrict prim_begin;
	struct PrimNode *restrict *restrict node_ptr;
	struct PrimNode *restrict *restrict next;
	struct PrimNode *restrict node;
	struct PrimNode *restrict alloc;
	size_t prim_length;
	unsigned int token;

	alloc = &node_buffer[0];
	token = *ptr;
	do {
		prim_begin = ptr;
		next       = &primitives[0];
		do {
			node_ptr = &next[token - PRIM_MIN];
			node     = *node_ptr;

			if (node == NULL) {
				node      = alloc++;
				*node_ptr = node;
			}

			next  = &node->next[0];
			token = *++ptr;
		} while (IS_PRIM(token));

		prim_length  = ptr - prim_begin;
		node->length = prim_length;

		if (prim_length > max_prim_length)
			max_prim_length = prim_length;

		do {
			token = *++ptr;
		} while (IS_DELIM(token));
	} while (token != '.');

	return (unsigned char *) (ptr + 2); /* skip ".\n" */
}


static inline size_t
compact_sequence(unsigned char *const restrict sequence,
		 const unsigned char *const end)
{
	unsigned char *restrict seq_ptr;
	unsigned char *restrict buf_ptr;
	unsigned int token;

	seq_ptr = buf_ptr = sequence;

	do {
		token = *buf_ptr;
		if (token != '\n')
			*seq_ptr++ = token;
	} while (++buf_ptr < end);

	return seq_ptr - sequence;
}


static inline void
patch_step(const unsigned char *const restrict sequence,
	   const size_t i_seq,
	   const size_t prim_length_limit)
{
	/* PRIM_LENGTH_MAX -> next power of 2 */
	static size_t acc_lengths[16];
	static size_t mask      = 15;
	size_t max_length_here;
	size_t length_here;
	size_t max_length_there;
	size_t length_prim;
	size_t i_prim;
	unsigned int token;
	struct PrimNode *restrict node;
	struct PrimNode *restrict *restrict next;

	next            = &primitives[0];
	i_prim		= 0;
	max_length_here = 0;

	while (1) {
		token = sequence[i_seq + i_prim];
		node  = next[token - PRIM_MIN];

		if (node == NULL)
			break;

		++i_prim;

		length_prim = node->length; /* 0 if not terminal */

		if (length_prim > 0) { /* able to bridge to earlier solution */
			max_length_there = acc_lengths[(i_seq + i_prim) & mask];
			length_here	 = max_length_there + length_prim;

			if (length_here > max_length_here)
				max_length_here = length_here;
		}

		if (i_prim == prim_length_limit)
			break;

		next = &node->next[0];
	}

	/* set max sequence length from 'i_seq' */
	acc_lengths[i_seq & mask] = max_length_here;

	if (max_length_here > max_prefix_length)
		max_prefix_length = max_length_here;
}


static inline void
solve(const unsigned char *const restrict sequence,
      const size_t length_sequence)
{
	size_t i_seq;
	size_t prim_length_limit;

	prim_length_limit = 1;

	while (1) {
		if (prim_length_limit > length_sequence)
			return;

		i_seq = length_sequence - prim_length_limit;

		patch_step(sequence,
			   i_seq,
			   prim_length_limit);

		if (prim_length_limit == max_prim_length)
			break;

		++prim_length_limit;
	}

	while (i_seq > 0)
		patch_step(sequence,
			   --i_seq,
			   prim_length_limit);
}


int
main(void)
{
	FILE *input, *output;
	static unsigned char input_buffer[SIZE_INPUT_BUFFER];
	unsigned char *restrict ptr;
	const unsigned char *restrict end;
	unsigned char *restrict sequence;
	size_t size_read;
	size_t length_sequence;

	assert(input = fopen("prefix.in", "r"));
	size_read = fread(&input_buffer[0], 1, sizeof(input_buffer), input);
	assert(size_read > 0);
	assert(ferror(input) == 0);
	assert(fclose(input) == 0);

	ptr = &input_buffer[0];
	end = ptr + size_read;

	sequence	= build_primitives(ptr);	   /* build tree */
	length_sequence = compact_sequence(sequence, end); /* remove newlines */

	solve(sequence, length_sequence);

	assert(output = fopen("prefix.out", "w"));
	assert(fprintf(output, "%zu\n", max_prefix_length) > 0);
	assert(fclose(output) == 0);

	return 0;
}
