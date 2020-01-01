/*
ID: wrpaape1
LANG: C
TASK: inflate
*/

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#define TASK "inflate"

enum Limits {
    NUM_MIN = 1,
    NUM_MAX = 10000
};

struct ProblemCategory {
    unsigned short num_pts;
    unsigned short num_mins;
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

static unsigned short
read_num(FILE *file)
{
    unsigned long num = 0;
    read_file(file, "%lu", &num);
    assert(num >= NUM_MIN);
    assert(num <= NUM_MAX);
    return (unsigned short) num;
}

static void
read_problem_category(FILE                   *file,
                      struct ProblemCategory *pc)
{
    pc->num_pts  = read_num(file);
    pc->num_mins = read_num(file);
}

static void
read_problem_categories(FILE                   *file,
                        struct ProblemCategory  pc[],
                        unsigned short          num_pc)
{
    for (unsigned short i = 0; i < num_pc; ++i) {
        read_problem_category(file, &pc[i]);
    }
}

static void
read_input(unsigned short         *tot_mins,
           unsigned short         *num_pc,
           struct ProblemCategory  pc[])
{
    FILE *input = open_input();
    *tot_mins   = read_num(input);
    *num_pc     = read_num(input);
    read_problem_categories(input,
                            pc,
                            *num_pc);
    close_file(input);
}

static unsigned long
solve(unsigned short               tot_mins,
      unsigned short               num_pc,
      const struct ProblemCategory pc[])
{
    unsigned long max_pts[2][NUM_MAX + 1] = { 0 };

    for (unsigned short rem_pc = 1; rem_pc <= num_pc; ++rem_pc) {
        const struct ProblemCategory *next_pc = &pc[rem_pc - 1];
        unsigned short i_next                 = rem_pc & 1;
        unsigned short i_prev                 = !i_next;
        for (unsigned short next_num_mins = next_pc->num_mins;
             next_num_mins <= tot_mins; ++next_num_mins) {
            unsigned short prev_num_mins = next_num_mins - next_pc->num_mins;

            unsigned long max_pts_so_far = max_pts[i_prev][next_num_mins];
            unsigned long next_max_pts   = max_pts[i_next][prev_num_mins]
                                         + next_pc->num_pts;

            if (max_pts_so_far > next_max_pts)
                next_max_pts = max_pts_so_far;

            max_pts[i_next][next_num_mins] = next_max_pts;
        }
    }

    return max_pts[num_pc & 1][tot_mins];
}

static void
write_output(unsigned long max_pts)
{
    FILE *output = open_output();
    write_file(output, "%lu\n",
               max_pts);
    close_file(output);
}

int
main(void)
{
    unsigned short tot_mins = 0;
    unsigned short num_pc   = 0;
    struct ProblemCategory pc[NUM_MAX];
    read_input(&tot_mins,
               &num_pc,
               pc);

    unsigned long max_pts = solve(tot_mins,
                                  num_pc,
                                  pc);

    write_output(max_pts);

    return 0;
}
