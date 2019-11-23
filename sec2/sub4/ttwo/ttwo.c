/*
ID: wrpaape1
LANG: C
TASK: ttwo
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>

enum {
    N         = 10,
    NUM_CELLS = N * N,
    MAX_STEPS = NUM_CELLS * 4
};

enum Direction {
    NORTH = 1 << 0,
    EAST  = 1 << 1,
    SOUTH = 1 << 2,
    WEST  = 1 << 3
};

typedef unsigned char Chart[N][N];

struct Location {
    unsigned int x;
    unsigned int y;
};

struct Agent {
    enum Direction  direction;
    struct Location location;
};

struct Scenario {
    struct Agent farmer;
    struct Agent cows;
    Chart        obstacles;
};

static void
set_location(struct Location *point,
             unsigned int  x,
             unsigned int  y)
{
    point->x = x;
    point->y = y;
}

static void
init_agent(struct Agent* agent,
           unsigned int  x,
           unsigned int  y)
{
    agent->direction = NORTH;
    set_location(&agent->location, x, y);
}

static void
init(FILE          *input,
     struct Scenario *scenario)
{
    memset(scenario, 0, sizeof(*scenario));

    int have_farmer = 0;
    int have_cows   = 0;
    for (unsigned int row = 0; row < N; ++row) {
        char line[N + 2];
        assert(fgets(line, sizeof(line), input));
        for (unsigned int col = 0; col < N; ++col)  {
            switch (line[col]) {
            case '.':
                break;
            case '*':
                scenario->obstacles[row][col] = 1;
                break;
            case 'F':
                assert(!have_farmer);
                have_farmer = 1;
                init_agent(&scenario->farmer, row, col);
                break;
            case 'C':
                assert(!have_cows);
                have_cows = 1;
                init_agent(&scenario->cows, row, col);
                break;
            default:
                assert(!"should not be here");
            }

        }
    }

    assert(have_farmer);
    assert(have_cows);
}

static int
have_collision(const struct Location *farmer,
               const struct Location *cows)
{
    return ((farmer->x == cows->x)
        &&  (farmer->y == cows->y));
}

static void
step(enum Direction direction, int *x, int *y)
{
    switch(direction) {
    case NORTH:
        --*x;
        return;
    case EAST:
        ++*y;
        return;
    case SOUTH:
        ++*x;
        return;
    case WEST:
        --*y;
        return;
    default:
        assert(!"should not be here");
    }
}

static enum Direction
rotate(enum Direction direction)
{
    switch(direction) {
    case NORTH:
        return EAST;
    case EAST:
        return SOUTH;
    case SOUTH:
        return WEST;
    case WEST:
        return NORTH;
    default:
        assert(!"should not be here");
    }

}

static int
obstructed(int         x,
           int         y,
           const Chart obstacles)
{
    return (x < 0) || (x >= N)
        || (y < 0) || (y >= N)
        || obstacles[x][y];
}

static void
update_agent(struct Agent *agent,
             const Chart   obstacles)
{
    int next_x = agent->location.x;
    int next_y = agent->location.y;

    step(agent->direction, &next_x, &next_y);
    if (!obstructed(next_x, next_y, obstacles)) {
        agent->location.x = next_x;
        agent->location.y = next_y;
    } else {
        agent->direction = rotate(agent->direction);
    }
}

static void
update(struct Scenario* scenario)
{
    update_agent(&scenario->farmer, scenario->obstacles);
    update_agent(&scenario->cows,   scenario->obstacles);
}

static unsigned int
solve(struct Scenario *scenario)
{
    for (unsigned int num_minutes = 0; num_minutes <= MAX_STEPS;
         update(scenario), ++num_minutes) {
        if (have_collision(&scenario->farmer.location,
                           &scenario->cows.location))
            return num_minutes;
    }
    return 0;
}

int
main(void)
{
	FILE *input, *output;

	assert(input = fopen("ttwo.in", "r"));
    struct Scenario scenario;
    init(input, &scenario);
	assert(fclose(input) == 0);

    unsigned int num_minutes = solve(&scenario);

	assert(output = fopen("ttwo.out", "w"));
    assert(fprintf(output, "%u\n", num_minutes) > 0);
	assert(fclose(output) == 0);

	return 0;
}
