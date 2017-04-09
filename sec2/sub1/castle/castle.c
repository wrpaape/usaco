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
	unsigned int visit_id;
	unsigned int walls;
};

size_t last_col, last_row;
static struct Cell castle[N_MAX][N_MAX];
static size_t init_room_count;
static size_t max_init_room_size;
static size_t max_rem_room_size;
static size_t max_rem_wall_row;
static size_t max_rem_wall_col;
static char max_rem_wall_dir;

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
	     const unsigned int visit_id)
{
	struct Cell *restrict cell;
	unsigned int walls;
	unsigned int room_size;


	cell = &castle[row][col];

	if (cell->visit_id == visit_id)
		return 0;

	cell->visit_id = visit_id; /* mark */

	walls     = cell->walls;
	room_size = 1;

	if (!(walls & NORTH))
		room_size += explore_room(row - 1, col, visit_id);

	if (!(walls & WEST))
		room_size += explore_room(row, col - 1, visit_id);

	if (!(walls & EAST))
		room_size += explore_room(row, col + 1, visit_id);

	if (!(walls & SOUTH))
		room_size += explore_room(row + 1, col, visit_id);

	return room_size;
}

static inline unsigned int
explore_castle(size_t *const restrict room_count_ptr)
{
	size_t room_size, max_room_size, room_count;
	static unsigned int visit_id;

	max_room_size = 0;
	room_count    = 0;

	++visit_id;

	for (size_t row = 0; row <= last_row; ++row)
		for (size_t col = 0; col <= last_col; ++col) {
			room_size   = explore_room(row, col, visit_id);
			room_count += (room_size > 0);

			if (room_size > max_room_size)
				max_room_size = room_size;
		}

	*room_count_ptr = room_count;

	return max_room_size;
}

static inline unsigned int
remove_wall(unsigned int *const restrict walls1_ptr,
	    unsigned int *const restrict walls2_ptr,
	    const unsigned int wall1,
	    const unsigned int wall2)
{
	unsigned int max_room_size;
	size_t unused_room_count;

	*walls1_ptr ^= wall1;
	*walls2_ptr ^= wall2;

	max_room_size = explore_castle(&unused_room_count);

	*walls1_ptr ^= wall1;
	*walls2_ptr ^= wall2;

	return max_room_size;
}

static inline unsigned int
remove_walls(const size_t row,
	     const size_t col,
	     char *const restrict rem_dir_ptr)
{
	unsigned int walls1;
	unsigned int *restrict walls1_ptr;
	unsigned int *restrict walls2_ptr;
	unsigned int max_room_size, room_size;
	char rem_dir;


	walls1_ptr = &castle[row][col].walls;
	walls1     = *walls1_ptr;

	if ((row > 0) && (walls1 & NORTH)) {
		walls2_ptr    = &castle[row - 1][col].walls;
		max_room_size = remove_wall(walls1_ptr,
					    walls2_ptr,
					    NORTH,
					    SOUTH);
		rem_dir        = 'N';

	} else {
		max_room_size = 0;
		rem_dir       = '\0';
	}

	if ((col < last_col) && (walls1 & EAST)) {
		walls2_ptr = &castle[row][col + 1].walls;
		room_size  = remove_wall(walls1_ptr,
					 walls2_ptr,
					 EAST,
					 WEST);

		if (room_size > max_room_size) {
			max_room_size = room_size;
			rem_dir       = 'E';
		}
	}

	*rem_dir_ptr = rem_dir;

	return max_room_size;
}

static inline void
solve(void)
{
	size_t rem_room_size;
	char rem_dir;

	max_init_room_size = explore_castle(&init_room_count);

	for (size_t col = 0; col <= last_col; ++col)
		for (ssize_t row = last_row; row >= 0; --row) {
			rem_room_size = remove_walls((size_t) row,
						     col,
						     &rem_dir);

			if (rem_room_size > max_rem_room_size) {
				max_rem_room_size = rem_room_size;
				max_rem_wall_dir  = rem_dir;
				max_rem_wall_row  = row;
				max_rem_wall_col  = col;
			}
		}
}

static inline void
write_output(void)
{
	FILE *output;
	assert(output = fopen("castle.out", "w"));
	assert(fprintf(output,
		       "%zu\n"
		       "%zu\n"
		       "%zu\n"
		       "%zu %zu %c\n",
		       init_room_count,
		       max_init_room_size,
		       max_rem_room_size,
		       max_rem_wall_row + 1,
		       max_rem_wall_col + 1,
		       max_rem_wall_dir) > 0);
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
