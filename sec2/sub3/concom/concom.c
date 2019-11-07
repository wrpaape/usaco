/*
ID: wrpaape1
LANG: C
TASK: concom
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>

enum {
    MIN_COMPANY   = 1,
    MAX_COMPANY   = 100,
    MAX_CELL      = MAX_COMPANY + 1
};

void
solve(unsigned char ownership[MAX_CELL][MAX_CELL],
      unsigned char   control[MAX_CELL][MAX_CELL])
{
    unsigned char new_control;
    do {
        new_control = 0;
        for (unsigned char i = MIN_COMPANY; i <= MAX_COMPANY; ++i) {
            for (unsigned char j = MIN_COMPANY; j <= MAX_COMPANY; ++j) {
                unsigned char i_controls_j     = (ownership[i][j] > 50);
                unsigned char *control_ptr     = &control[i][j];
                unsigned char old_i_controls_j = *control_ptr;
                if (i_controls_j && !old_i_controls_j) {
                    *control_ptr = 1;
                    for (unsigned char k = MIN_COMPANY; k <= MAX_COMPANY; ++k) {
                        unsigned char *ownership_ptr  = &ownership[i][k];
                        unsigned char total_ownership = *ownership_ptr
                                                      + ownership[j][k];
                        if (total_ownership > 100) {
                            total_ownership = 100;
                        }
                        *ownership_ptr = total_ownership;
                    }
                    new_control = 1;
                }
            }
        }
    } while (new_control);
}

int
main(void)
{
	FILE *input, *output;
	unsigned char ownership[MAX_CELL][MAX_CELL];
	unsigned char control[MAX_CELL][MAX_CELL];

    memset(ownership, 0, sizeof(ownership));
    memset(control,   0, sizeof(control));

	assert(input = fopen("concom.in", "r"));
	unsigned int rem_triples = 0;
	assert(fscanf(input, "%u", &rem_triples) == 1);
    for (; rem_triples > 0; --rem_triples) {
        unsigned char i, j, p;
        assert(fscanf(input, "%hhu %hhu %hhu", &i, &j, &p) == 3);
        ownership[i][j] = p;
    }
	assert(fclose(input) == 0);

    solve(ownership, control);

	assert(output = fopen("concom.out", "w"));
    for (unsigned char i = MIN_COMPANY; i <= MAX_COMPANY; ++i) {
        for (unsigned char j = MIN_COMPANY; j <= MAX_COMPANY; ++j) {
            if ((i != j) && (control[i][j])) {
                assert(
                    fprintf(output, "%hhu %hhu\n", i, j) > 0
                );
            }
        }
    }
	assert(fclose(output) == 0);

	return 0;
}
