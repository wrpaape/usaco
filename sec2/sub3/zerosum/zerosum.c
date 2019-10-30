/*
ID: wrpaape1
LANG: C
TASK: zerosum
*/

#include <stdio.h>
#include <assert.h>

int
next_num(const char *operations,
         int         n,
         int        *i_ptr)
{
    int num = 0;

    int i = *i_ptr;

    while (i <= n) {
        num *= 10;
        num += i++;

        if (operations[i - 2] != ' ')
            break;
    }

    *i_ptr = i;
    return num;
}

int
evaluate(const char *operations,
         int         n)
{
    int i      = 1;
    int result = next_num(operations, n, &i);

    while (i <= n) {
        char operation = operations[i - 2];

        int num = next_num(operations, n, &i);

        switch (operation) {
        case '+':
            result += num;
            break;
        case '-':
            result -= num;
            break;
        default:
            assert(!"can't get here");
        }
    }

    return result;
}

void
print(const char *operations,
      int         n,
      FILE       *output)
{
    for (int i = 1; ; ++i) {
        assert(fprintf(output, "%u", i) > 0);

        if (i == n) {
            break;
        }

        assert(fprintf(output, "%c", operations[i - 1]) > 0);
    }

    assert(fprintf(output, "\n") > 0);
}

void
do_solve(char *operations,
         int   i,
         int   n,
         FILE *output)
{
    if (i < n) {
        char* operation = &operations[i - 1];
        int next_i      = i + 1;

        static const char OP_SET[3] = { ' ', '+', '-' };
        for (int j = 0; j < 3; ++j) {
            *operation = OP_SET[j];
            do_solve(operations, next_i, n, output);
        }
    } else {
        if (evaluate(operations, n) == 0) {
            print(operations, n, output);
        }
    }
}

void
solve(int   n,
      FILE *output)
{
    char operations[] = {
        ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' '
    };

    do_solve(operations, 1, n, output);
}

int
main(void)
{
	FILE *input, *output;
	int N;

	assert(input = fopen("zerosum.in", "r"));
	assert(fscanf(input, "%d", &N) == 1);
	assert(fclose(input) == 0);

	assert(output = fopen("zerosum.out", "w"));
    solve(N, output);
	assert(fclose(output) == 0);

	return 0;
}
