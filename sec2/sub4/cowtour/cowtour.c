/*
ID: wrpaape1
LANG: C
TASK: cowtour
*/

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <limits.h>


enum {
    N_MIN = 2,
    N_MAX = 150,
    COORD_MIN = 0,
    COORD_MAX = 100000
};

const double INFINITE_DISTANCE = 1e12;

struct Point {
    long x;
    long y;
};

struct Scenario {
    unsigned char num_pastures;
    double        distance[N_MAX][N_MAX];
};

unsigned char
get_num_pastures(FILE *input)
{
    long value;
    assert(fscanf(input, "%lu\n", &value) == 1);
    assert(value >= N_MIN);
    assert(value <= N_MAX);
    return (unsigned char) value;
}

void
check_coord(long coord)
{
    assert(coord >= COORD_MIN);
    assert(coord <= COORD_MAX);
}

void
get_pastures(FILE               *input,
             struct Point       *pastures,
             const struct Point *pastures_end)
{
    for (; pastures < pastures_end; ++pastures) {
        assert(fscanf(input, "%lu %lu\n",
                      &pastures->x,
                      &pastures->y) == 2);
        check_coord(pastures->x);
        check_coord(pastures->y);
    }
}

double
get_distance(const struct Point* from,
             const struct Point* to)
{
    double dx = (double) (to->x - from->x);
    double dy = (double) (to->y - from->y);

    return sqrt((dx * dx) + (dy * dy));
}

void
get_distances(FILE               *input,
              unsigned char       num_pastures,
              const struct Point  pastures[N_MAX],
              double              distance[N_MAX][N_MAX])
{
    for (unsigned char from = 0; from < num_pastures; ++from) {
        char line[N_MAX+2];
        assert(fgets(line, sizeof(line), input));
        for (unsigned char to = 0; to < num_pastures; ++to) {
            double from_to = get_distance(&pastures[from],
                                          &pastures[to]);
            if (line[to] == '0') {
                from_to = -from_to; /* not-reachable */
            }
            distance[from][to] = from_to;
        }
    }
}

void
init(FILE            *input,
     struct Scenario *scenario)
{
    scenario->num_pastures = get_num_pastures(input);

    struct Point pastures[N_MAX];
    get_pastures(input,
                 pastures,
                 &pastures[scenario->num_pastures]);

    get_distances(input,
                  scenario->num_pastures,
                  pastures,
                  scenario->distance);
}

void
get_min_distance(const struct Scenario *scenario,
                 double                 min_distance[N_MAX][N_MAX])
{
    for (unsigned char from = 0; from < scenario->num_pastures; ++from) {
        for (unsigned char to = 0; to < scenario->num_pastures; ++to) {
            double from_to = scenario->distance[from][to];
            min_distance[from][to] = (from_to >= 0.0)
                                   ? from_to
                                   : INFINITE_DISTANCE;
        }
    }

    for (unsigned char next = 0; next < scenario->num_pastures; ++next) {
        for (unsigned char from = 0; from < scenario->num_pastures; ++from) {
            for (unsigned char to = 0; to < scenario->num_pastures; ++to) {
                double *from_to = &min_distance[from][to];
                double next_min_distance = min_distance[from][next]
                                         + min_distance[next][to];
                if (*from_to > next_min_distance) {
                    *from_to = next_min_distance;
                }
            }
        }
    }
}

void
get_max_min_distance(const struct Scenario *scenario,
                     double        	 		max_min_distance[N_MAX])
{
    double min_distance[N_MAX][N_MAX];
    get_min_distance(scenario, min_distance);

    for (unsigned char from = 0; from < scenario->num_pastures; ++from) {
        double max_distance = 0.0;
        for (unsigned char to = 0; to < scenario->num_pastures; ++to) {
            double from_to = min_distance[from][to];
            if ((from_to < INFINITE_DISTANCE) && (from_to > max_distance)) {
                max_distance = from_to;
            }
        }
        max_min_distance[from] = max_distance;
    }
}

double
visit_pasture(const struct Scenario *scenario,
              unsigned char          fields[N_MAX],
              const double           max_min_distance[N_MAX],
              unsigned char          field,
              double                 field_max_min_distance,
              unsigned char          pasture)
{
    fields[pasture] = field;
    double pasture_max_min_distance = max_min_distance[pasture];
    if (pasture_max_min_distance > field_max_min_distance) {
        field_max_min_distance = pasture_max_min_distance;
    }

    for (unsigned char to = 0; to < scenario->num_pastures; ++to) {
        if ((scenario->distance[pasture][to] >= 0.0) &&
            (fields[to] == UCHAR_MAX)) {
            field_max_min_distance = visit_pasture(scenario,
                                                   fields,
                                                   max_min_distance,
                                                   field,
                                                   field_max_min_distance,
                                                   to);
        }
    }

    return field_max_min_distance;
}

unsigned char
get_unvisted(unsigned char fields[N_MAX])
{
    unsigned char pasture = 0;
    for (; (pasture < N_MAX) && (fields[pasture] != UCHAR_MAX); ++pasture)
        ; /* spin */

    return pasture;
}


void
get_fields(const struct Scenario *scenario,
           const double           max_min_distance[N_MAX],
           unsigned char          fields[N_MAX],
           double                 diameter[N_MAX])
{
    unsigned char visited[N_MAX];
    (void) memset(fields, UCHAR_MAX, sizeof(visited));

    unsigned char field   = 0;
    unsigned char pasture = 0;
    do {
        double field_diameter = visit_pasture(scenario,
                                              fields,
                                              max_min_distance,
                                              field,
                                              0.0,
                                              pasture);
        diameter[field++] = field_diameter;
        pasture = get_unvisted(fields);
    } while (pasture < N_MAX);
}

double
get_max(double value1,
        double value2,
        double value3)
{
    double max_value = value1;

    if (value2 > max_value)
        max_value = value2;

    if (value3 > max_value)
        max_value = value3;

    return max_value;
}

double
solve(struct Scenario *scenario)
{
    double max_min_distance[N_MAX];
    get_max_min_distance(scenario,
                         max_min_distance);

    unsigned char fields[N_MAX];
    double        diameter[N_MAX];
    get_fields(scenario,
               max_min_distance,
               fields,
               diameter);

    double min_connected_diameter = INFINITE_DISTANCE;

    unsigned char last_pasture = scenario->num_pastures - 1;

    for (unsigned char from = 0; from < last_pasture; ++from) {
        unsigned char field_from     = fields[from];
        double diameter_from         = diameter[field_from];
        double max_min_distance_from = max_min_distance[from];
        for (unsigned char to = from + 1; to <= last_pasture; ++to) {
            unsigned char field_to = fields[to];
            if (field_from == field_to)
                continue;

            double from_to           = scenario->distance[from][to];
            double connected_from_to = -from_to;
            double diameter_from_to = max_min_distance_from
                                    + max_min_distance[to]
                                    + connected_from_to;
            double connected_diameter = get_max(diameter_from_to,
                                                diameter_from,
                                                diameter[field_to]);
            if (connected_diameter < min_connected_diameter)
                min_connected_diameter = connected_diameter;
        }
    }

    return min_connected_diameter;
}

int
main(void)
{
	FILE *input, *output;

	assert(input = fopen("cowtour.in", "r"));
    struct Scenario scenario;
    init(input, &scenario);
	assert(fclose(input) == 0);

    double min_diameter = solve(&scenario);

	assert(output = fopen("cowtour.out", "w"));
    assert(fprintf(output, "%.6f\n", min_diameter) > 0);
	assert(fclose(output) == 0);

	return 0;
}
