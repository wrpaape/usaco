/*
ID: wrpaape1
LANG: C
TASK: templatetask
*/

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#define TASK "templatetask"

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

static void
read_input(char   buffer[],
           size_t size)
{
    FILE *input = open_input();
    read_file(input, "%.*s\n",
              (int) size,
              buffer);
    close_file(input);
}

static int
solve(char buffer[])
{
    (void) buffer;
    return 0;
}

static void
write_output(int solution)
{
    FILE *output = open_output();
    write_file(output, "%d\n",
               solution);
    close_file(output);
}

int
main(void)
{
    char buffer[sizeof("INPUT")];
    read_input(buffer, sizeof(buffer));

    int solution = solve(buffer);

    write_output(solution);

    return 0;
}
