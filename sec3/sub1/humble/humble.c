/*
ID: wrpaape1
LANG: C
TASK: humble
*/

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <limits.h>
#include <stddef.h>

#define TASK "humble"

enum Limits {
    K_MIN      = 1,
    K_MAX      = 100,
    N_MIN      = 1,
    N_MAX      = 100000
};

struct Problem {
    unsigned long k;
    unsigned long n;
    unsigned long primes[K_MAX];
};

static FILE *
open_file(const char *filename,
          const char *mode)
{
    FILE *file = fopen(filename, mode);
    assert(file);
    return file;
}

static FILE *
open_input(void)
{
    return open_file(TASK ".in", "r");
}

static FILE *
open_output(void)
{
    return open_file(TASK ".out", "w");
}

static void
read_file(FILE       *file,
          const char *format,
          ...)
{
    va_list args;
    va_start(args, format);
    int num_items = vfscanf(file, format, args);
    va_end(args);
    assert(num_items >= 0);
    (void) num_items;
}

static void
write_file(FILE       *file,
           const char *format,
           ...)
{
    va_list args;
    va_start(args, format);
    int num_chars = vfprintf(file, format, args);
    va_end(args);
    assert(num_chars >= 0);
    (void) num_chars;
}

static void
close_file(FILE *file)
{
    int error = fclose(file);
    assert(error == 0);
    (void) error;
}

static unsigned long
read_num(FILE *input)
{
    unsigned long value = 0;
    read_file(input, "%lu", &value);
    return value;
}

static unsigned long
read_bound_num(FILE          *input,
               unsigned long  min,
               unsigned long  max)
{
    unsigned long value = read_num(input);
    assert(value >= min);
    assert(value <= max);
    (void) min;
    (void) max;
    return value;
}

static unsigned long
read_k(FILE *input)
{
    return read_bound_num(input, K_MIN, K_MAX);
}

static unsigned long
read_n(FILE *input)
{
    return read_bound_num(input, N_MIN, N_MAX);
}

static void
read_input(struct Problem *problem)
{
    FILE *input = open_input();

    problem->k = read_k(input);
    problem->n = read_n(input);
    for (unsigned long i = 0; i < problem->k; ++i) {
        problem->primes[i] = read_num(input);
    }

    close_file(input);
}

static unsigned long
find_lowest_humble(unsigned long humbles[N_MAX + 1],
                   unsigned long begin,
                   unsigned long end,
                   unsigned long humble_floor)
{
    while (1) {
        unsigned long mid    = begin + ((end - begin) / 2);
        unsigned long humble = humbles[mid];

        if (humble < humble_floor) {
            begin = mid + 1;
            if (begin == end)
                break;
        } else if (humble > humble_floor) {
            end = mid;
            if (begin == end)
                break;
        } else {
            return mid;
        }
    }
    return begin;
}

static unsigned long
get_next_humble(const struct Problem *problem,
                unsigned long         lower_bounds[K_MAX],
                unsigned long         humbles[N_MAX + 1],
                unsigned long         num_humbles)
{
    unsigned long last_humble     = humbles[num_humbles - 1];
    unsigned long min_next_humble = last_humble + 1;

    unsigned long next_humble = ULONG_MAX;
    for (unsigned long i = 0; i < problem->k; ++i) {
        unsigned long prime        = problem->primes[i];
        unsigned long div          = min_next_humble / prime;
        unsigned long rem          = min_next_humble % prime;
        unsigned long humble_floor = div + (rem > 0);
        unsigned long i_humble     = find_lowest_humble(humbles,
                                                        lower_bounds[i],
                                                        num_humbles,
                                                        humble_floor);
        lower_bounds[i] = i_humble;

        unsigned long humble = humbles[i_humble] * prime;
        if (humble < next_humble)
            next_humble = humble;
    }

    return next_humble;
}

static unsigned long
solve(const struct Problem *problem)
{
    unsigned long humbles[N_MAX + 1];
    humbles[0] = 1;

    unsigned long lower_bounds[K_MAX] = { 0 };

    for (unsigned long num_humbles = 1; num_humbles <= problem->n;) {
        unsigned long humble = get_next_humble(problem,
                                               lower_bounds,
                                               humbles,
                                               num_humbles);
        humbles[num_humbles++] = humble;
    }

    return humbles[problem->n];
}

static void
write_output(unsigned long solution)
{
    FILE *output = open_output();
    write_file(output, "%lu\n",
               solution);
    close_file(output);
}

int
main(void)
{
    struct Problem problem;
    read_input(&problem);

    unsigned long solution = solve(&problem);

    write_output(solution);

    return 0;
}
