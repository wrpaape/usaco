/*
ID: wrpaape1
LANG: C
TASK: castle
*/


#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define N_MAX	50
#define WEST	1
#define NORTH	2
#define EAST	4
#define SOUTH	8

struct Cell {
	unsigned int room;  /* integer id >= 1 */
	unsigned int walls; /* mask of listed directions above */
};

size_t last_col, last_row;

static struct Cell castle[N_MAX][N_MAX];             /* 0-based index */
static unsigned int room_sizes[(N_MAX * N_MAX) + 1]; /* 1-based index */
static size_t room_count;

static size_t max_init_room_size;
static size_t max_rem_room_size;
static size_t max_rem_row;
static size_t max_rem_col;
static char max_rem_dir;

static inline void
read_input(void)
{
	FILE *input;
	size_t M, N;
	assert(input = fopen("castle.in", "r"));
	assert(fscanf(input, "%zu %zu\n", &M, &N) == 2);

	last_col = M - 1;
	last_row = N - 1;

	for (size_t row = 0; row <= last_row; ++row)
		for (size_t col = 0; col <= last_col; ++col)
			assert(fscanf(input, "%u",
				      &castle[row][col].walls) == 1);

	assert(fclose(input) == 0);
}

unsigned int
explore_room(const size_t row,
	     const size_t col,
	     const unsigned int room)
{
	struct Cell *restrict cell;
	unsigned int walls;
	unsigned int room_size;


	cell = &castle[row][col];

	if (cell->room)
		return 0; /* already visted */

	cell->room = room; /* mark */
	walls      = cell->walls;
	room_size  = 1;

	if (!(walls & NORTH))
		room_size += explore_room(row - 1, col, room);

	if (!(walls & WEST))
		room_size += explore_room(row, col - 1, room);

	if (!(walls & EAST))
		room_size += explore_room(row, col + 1, room);

	if (!(walls & SOUTH))
		room_size += explore_room(row + 1, col, room);

	return room_size;
}

static inline void
explore_castle(void)
{
	size_t room_size, room;

	room = 1;

	for (size_t row = 0; row <= last_row; ++row)
		for (size_t col = 0; col <= last_col; ++col) {
			room_size  = explore_room(row, col, room);

			if (room_size > 0) {
				room_sizes[room++] = room_size;

				if (room_size > max_init_room_size)
					max_init_room_size = room_size;
			}
		}

	room_count = room - 1;
}

static inline unsigned int
check_remove_wall(const unsigned int row1,
		  const unsigned int col1,
		  char *const restrict rem_dir_ptr)
{
	struct Cell *restrict cell1;
	unsigned int room1, room2;
	unsigned int room1_size, room2_size;
	unsigned int max_room2_size;
	unsigned int walls1;
	char rem_dir;

	cell1  = &castle[row1][col1];
	room1  = cell1->room;
	walls1 = cell1->walls;

	room1_size = room_sizes[room1];

	/* set sentinel values in case no wall can be removed to join
	 * 2 unique rooms */
	max_room2_size = 0;
	rem_dir        = '\0';

	/* check the walled room of the cell directly NORTH of cell1 */
	if ((row1 > 0) && (walls1 & NORTH)) {
		room2 = castle[row1 - 1][col1].room;

		/* if unique room, get its size */
		if (room2 != room1) {
			max_room2_size = room_sizes[room2];
			rem_dir        = 'N';
		}
	}

	/* check the walled room of the cell directly EAST of cell1 */
	if ((col1 < last_col) && (walls1 & EAST)) {
		room2 = castle[row1][col1 + 1].room;

		/* if unique room, get its size */
		if (room2 != room1) {
			room2_size = room_sizes[room2];

			/* if larger than current max, update */
			if (room2_size > max_room2_size) {
				max_room2_size = room2_size;
				rem_dir        = 'E';
			}
		}
	}

	/* rem_dir is a character token indicating the direction of the
	 * removed wall ('N' or 'E'), or '\0' if no wall could be removed
	 * to combine 2 unique rooms */
	*rem_dir_ptr = rem_dir;

	/* combine size of room1 and room2
	 * where room2 is the larger of walled rooms NORTH and EAST of cell1
	 * max_room2_size is 0 if in NE corner of castle or no unique walled
	 * rooms NORTH or EAST of cell1 */
	return room1_size + max_room2_size;
}


static inline void
solve(void)
{
	size_t rem_room_size;
	char rem_dir;

	/* assign all castle 'cell's a positive 'room' id,
	 * set room_sizes[room] = <count of contiguous cells in room>
	 * find room_count
	 */
	explore_castle();

	/* traverse castle starting at SW cell and moving NORTH then EAST
	 *
	 * for each cell, get maximum combined room size of removing a NORTH
	 * or EAST wall from current cell or cell's room size if no wall can
	 * be removed to combine 2 unique rooms
	 *
	 * set 'max_rem' solution variables accordingly at absolute maximum
	 * over M X N cells */
	for (size_t col = 0; col <= last_col; ++col)
		for (ssize_t row = last_row; row >= 0; --row) {
			rem_room_size = check_remove_wall((size_t) row,
							  col,
							  &rem_dir);

			if (rem_room_size > max_rem_room_size) {
				max_rem_room_size = rem_room_size;
				max_rem_dir       = rem_dir;
				max_rem_row       = row;
				max_rem_col       = col;
			}
		}
}

static inline void
write_output(void)
{
	FILE *output;
	assert(output = fopen("castle.out", "w"));
	/* increment max_rem_row|col to display w/ 1-based index */
	assert(fprintf(output,
		       "%zu\n"
		       "%zu\n"
		       "%zu\n"
		       "%zu %zu %c\n",
		       room_count,
		       max_init_room_size,
		       max_rem_room_size,
		       max_rem_row + 1, max_rem_col + 1, max_rem_dir) > 0);
	assert(fclose(output) == 0);
}


int
main(void)
{
	read_input();
	solve();
	write_output();
	return 0;
}
