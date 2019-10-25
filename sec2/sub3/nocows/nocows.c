/*
ID: wrpaape1
LANG: C
TASK: nocows
*/

#include <stdio.h>
#include <assert.h>

unsigned int
solve(unsigned int n,
      unsigned int k)
{
    enum {
        N_MAX = 200,
        K_MAX = 100
    };
    static const unsigned int MAGIC_NUMBER = 9901;

    unsigned short SOLUTION[K_MAX + 1][N_MAX + 1];
    SOLUTION[1][1] = 1;

    unsigned int max_child_nodes = n - 2;
    for (unsigned int child_height1 = 1; child_height1 < k; ++child_height1) {
        unsigned int parent_height = child_height1 + 1;
        for (unsigned int child_height2 = 1; child_height2 <= child_height1;
             ++child_height2) {
            for (unsigned int child_nodes1 = 1; child_nodes1 <= max_child_nodes;
                 child_nodes1 += 2) {
                for (unsigned int child_nodes2 = 1;
                     child_nodes2 <= child_nodes1; child_nodes2 += 2) {
                    unsigned int combs = SOLUTION[child_height1][child_nodes1]
                                       * SOLUTION[child_height2][child_nodes2];
                    combs %= MAGIC_NUMBER;

                    if (child_nodes1 != child_nodes2) {
                        unsigned int combs2 =
                            SOLUTION[child_height1][child_nodes2]
                          * SOLUTION[child_height2][child_nodes1];
                        combs2 %= MAGIC_NUMBER;

                        combs += combs2;
                        combs %= MAGIC_NUMBER;
                    }

                    if (child_height1 != child_height2) {
                        combs *= 2;
                        combs %= MAGIC_NUMBER;
                    }

                    unsigned int parent_nodes = child_nodes1
                                              + child_nodes2
                                              + 1;
                    unsigned short *parent_combs_ptr =
                        &SOLUTION[parent_height][parent_nodes];

                    unsigned int parent_combs = *parent_combs_ptr;

                    /* increment the parent combinations */
                    parent_combs += combs;
                    parent_combs %= MAGIC_NUMBER;

                    *parent_combs_ptr = (unsigned short) parent_combs;
                }
            }
        }
    }

    return SOLUTION[k][n];
}

int
main(void)
{
	FILE *input, *output;
	unsigned int N, K;

	assert(input = fopen("nocows.in", "r"));
	assert(fscanf(input, "%u %u\n", &N, &K) == 2);
	assert(fclose(input) == 0);

	unsigned int solution = solve(N, K);

	assert(output = fopen("nocows.out", "w"));
	assert(fprintf(output, "%u\n", solution) > 0);
	assert(fclose(output) == 0);

	return 0;
}
