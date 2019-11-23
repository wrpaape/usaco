/*
ID: wrpaape1
LANG: C
TASK: maze1
*/

#include <stdio.h>
#include <limits.h>
#include <assert.h>

enum {
    W_MAX         = 38,
    H_MAX         = 100,
    LINE_SIZE_MAX = 2*W_MAX + 3,
    NUM_EXITS     = 2
};

enum Direction {
    NORTH = 1 << 1,
    WEST  = 1 << 2,
    EAST  = 1 << 3,
    SOUTH = 1 << 4
};

typedef unsigned char     EdgeTable[H_MAX][W_MAX];
typedef unsigned int  DistanceTable[H_MAX][W_MAX];

struct Maze {
    unsigned int  height;
    unsigned int  width;
    EdgeTable     edges;
};

struct TextRow {
    char *top;
    char *mid;
    char *bot;
};

struct Location {
    unsigned int row;
    unsigned int col;
};

const unsigned int INFINITY = UINT_MAX - 1;

static void
get_maze_dimensions(FILE         *input,
                    unsigned int *width,
                    unsigned int *height)
{
    assert(fscanf(input, "%u %u\n",
                  width, height) == 2);
    assert(*width  > 0);
    assert(*height > 0);
}

static void
get_line(FILE *input,
         char *buffer)
{
    assert(fgets(buffer, LINE_SIZE_MAX, input));
}

static unsigned char
get_edges(const struct TextRow *text_row,
          unsigned int          col)
{
    unsigned int i_west        = 2*col;
    unsigned int i_north_south = i_west + 1;
    unsigned int i_east        = i_north_south + 1;

    unsigned char edges = 0;

    if (text_row->top[i_north_south] == ' ')
        edges |= NORTH;

    if (text_row->mid[i_west] == ' ')
        edges |= WEST;

    if (text_row->mid[i_east] == ' ')
        edges |= EAST;

    if (text_row->bot[i_north_south] == ' ')
        edges |= SOUTH;

    return edges;
}

static void
fill_edges(struct TextRow *text_row,
           unsigned char  *edges,
           unsigned int    width)
{
    for (int col = 0; col < width; ++col) {
        edges[col] = get_edges(text_row, col);
    }
}

static void
build_maze(struct Maze *maze,
           FILE        *input)
{
    get_maze_dimensions(input, &maze->width, &maze->height);

    char buffer[3][LINE_SIZE_MAX];
    struct TextRow text_row;
    text_row.top = buffer[0];
    text_row.mid = buffer[1];
    text_row.bot = buffer[2];

    get_line(input, text_row.top);

    for (unsigned int row = 0; row < maze->height; ++row) {
        get_line(input, text_row.mid);
        get_line(input, text_row.bot);

        fill_edges(&text_row,
                   maze->edges[row],
                   maze->width);

        char *tmp    = text_row.top;
        text_row.top = text_row.bot;
        text_row.bot = tmp;
    }
}

static void
set_exit(const struct Location *location,
         unsigned int          *num_exits,
         struct Location        exits[NUM_EXITS])
{
    assert(*num_exits < NUM_EXITS);
    struct Location *exit = &exits[*num_exits];
    ++*num_exits;
    exit->row = location->row;
    exit->col = location->col;
}

static void
check_edge_for_exit(const EdgeTable        edges,
                    const struct Location *location,
                    enum Direction         direction,
                    unsigned int          *num_exits,
                    struct Location        exits[NUM_EXITS])
{
    if (edges[location->row][location->col] & direction) {
        set_exit(location, num_exits, exits);
    }
}

static void
find_exits(const struct Maze *maze,
           struct Location    exits[NUM_EXITS])
{
    unsigned int num_exits = 0;

    unsigned int last_col = maze->width  - 1;
    unsigned int last_row = maze->height - 1;

    struct Location location;
    for (location.col = 0; location.col <= last_col; ++location.col) {
        location.row = 0;
        check_edge_for_exit(maze->edges, &location, NORTH, &num_exits, exits);
        location.row = last_row;
        check_edge_for_exit(maze->edges, &location, SOUTH, &num_exits, exits);
    }

    for (location.row = 0; location.row <= last_row; ++location.row) {
        location.col = 0;
        check_edge_for_exit(maze->edges, &location, WEST, &num_exits, exits);
        location.col = last_col;
        check_edge_for_exit(maze->edges, &location, EAST, &num_exits, exits);
    }

    assert(num_exits == NUM_EXITS);
}

static void
evaluate_location(const struct Maze *maze,
                  struct Location   *location,
                  DistanceTable      distances)
{
    unsigned int last_col = maze->width  - 1;
    unsigned int last_row = maze->height - 1;
    unsigned char edge    = maze->edges[location->row][location->col];

    unsigned int min_neighbor_distance = INFINITY;

    if ((edge & NORTH) && (location->row > 0)) {
        unsigned int distance = distances[location->row - 1][location->col];
        if (distance < min_neighbor_distance)
            min_neighbor_distance = distance;
    }
    if ((edge & WEST) && (location->col > 0)) {
        unsigned int distance = distances[location->row][location->col - 1];
        if (distance < min_neighbor_distance)
            min_neighbor_distance = distance;
    }
    if ((edge & EAST) && (location->col < last_col)) {
        unsigned int distance = distances[location->row][location->col + 1];
        if (distance < min_neighbor_distance)
            min_neighbor_distance = distance;
    }
    if ((edge & SOUTH) && (location->row < last_row)) {
        unsigned int distance = distances[location->row + 1][location->col];
        if (distance < min_neighbor_distance)
            min_neighbor_distance = distance;
    }

    unsigned int min_distance_through_neighbor = min_neighbor_distance + 1;

    unsigned int *distance_ptr = &distances[location->row][location->col];
    if (min_distance_through_neighbor < *distance_ptr)
        *distance_ptr = min_distance_through_neighbor;
}

static void
find_min_distances_from(const struct Maze *maze,
                        struct Location   *src,
                        DistanceTable      distances)
{
    for (unsigned int row = 0; row < maze->height; ++row) {
        for (unsigned int col = 0; col < maze->width; ++col) {
            distances[row][col] = INFINITY;
        }
    }
    distances[src->row][src->col] = 1;

    struct Location dest;
    for (unsigned int rem_iterations = maze->height * maze->width;
         rem_iterations > 0; --rem_iterations) {
        for (dest.row = 0; dest.row < maze->height; ++dest.row) {
            for (dest.col = 0; dest.col < maze->width; ++dest.col) {
                evaluate_location(maze, &dest, distances);
            }
        }
    }
}

static unsigned int
get_max_distance(DistanceTable distances[NUM_EXITS],
                 unsigned int  height,
                 unsigned int  width)
{
    unsigned int max_distance = 0;
    for (unsigned int row = 0; row < height; ++row) {
        for (unsigned int col = 0; col < width; ++col) {
            unsigned int min_distance_to_exit = UINT_MAX;
            for (unsigned int i = 0; i < NUM_EXITS; ++i) {
                unsigned int distance_to_exit = distances[i][row][col];
                if (distance_to_exit < min_distance_to_exit)
                    min_distance_to_exit = distance_to_exit;
            }
            if (min_distance_to_exit > max_distance)
                max_distance = min_distance_to_exit;
        }
    }
    return max_distance;
}

static unsigned int
find_max_steps(const struct Maze *maze)
{
    struct Location exits[NUM_EXITS];
    find_exits(maze, exits);

    DistanceTable min_distances[NUM_EXITS];

    for (unsigned int i = 0; i < NUM_EXITS; ++i)
        find_min_distances_from(maze,
                                &exits[i],
                                min_distances[i]);


    return get_max_distance(min_distances,
                            maze->height,
                            maze->width);
}

int
main()
{
    FILE* input = fopen("maze1.in", "r");
    assert(input);
    struct Maze maze;
    build_maze(&maze, input);
    assert(fclose(input) == 0);

    unsigned int max_distance = find_max_steps(&maze);
    
    FILE* output = fopen("maze1.out", "w");
    fprintf(output, "%u\n", max_distance);
    assert(output);
    assert(fclose(output) == 0);
    return 0;
}
