/*
ID: wrpaape1
LANG: C
TASK: milk3
*/


#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define CAP_MAX		20
#define CAP_MAX_P1	(CAP_MAX + 1)
#define MIN(X, Y)	(((X) < (Y)) ? (X) : (Y))

unsigned int A, B, C;
bool rem_C[CAP_MAX_P1];
bool visited[CAP_MAX_P1][CAP_MAX_P1];

void
pour_step(const unsigned int a,
	  const unsigned int b,
	  const unsigned int c)
{
	bool *restrict visited_ptr;
	unsigned int room_a, room_b, room_c;
	unsigned int pour;

	visited_ptr = &visited[a][b];

	if (*visited_ptr)
		return;

	*visited_ptr = true; /* mark visted state */

	if (a == 0)
		rem_C[c] = true;

	room_a = A - a;
	room_b = B - b;
	room_c = C - c;

	/* pour a -> b */
	pour = MIN(a, room_b);
	pour_step(a - pour, b + pour, c);

	/* pour a -> c */
	pour = MIN(a, room_c);
	pour_step(a - pour, b, c + pour);

	/* pour b -> a */
	pour = MIN(b, room_a);
	pour_step(a + pour, b - pour, c);

	/* pour b -> c */
	pour = MIN(b, room_c);
	pour_step(a, b - pour, c + pour);

	/* pour c -> a */
	pour = MIN(c, room_a);
	pour_step(a + pour, b, c - pour);

	/* pour c -> b */
	pour = MIN(c, room_b);
	pour_step(a, b + pour, c - pour);
}



int
main(void)
{
	FILE *input, *output;
	unsigned int c;

	assert(input = fopen("milk3.in", "r"));
	assert(fscanf(input, "%u %u %u\n", &A, &B, &C) == 3);
	assert(fclose(input) == 0);

	pour_step(0, 0, C);

	for (c = 0; !rem_C[c]; ++c)
		; /* find first c remainder */

	assert(output = fopen("milk3.out", "w"));
	assert(fprintf(output, "%u", c) > 0);

	while (++c <= C)
		if (rem_C[c])
			assert(fprintf(output, " %u", c) > 0);

	assert(fputs("\n", output) != EOF);
	assert(fclose(output) == 0);

	return 0;
}
