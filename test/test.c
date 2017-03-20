/*
ID: wrpaape1
LANG: C
TASK: test
*/

#include <stdio.h> /* fdopen, fdclose */

int
main(void)
{
	FILE *input;
	FILE *output;
	int x;
	int y;

	input = fopen("test.in", "r");
	if (input == NULL) {
		perror("fopen(\"test.in\")");
		return 1;
	}

	if (fscanf(input, "%d %d", &x, &y) != 2) {
		perror("fscanf");
		goto FAIL0;
	}

	output = fopen("test.out", "w");
	if (output == NULL) {
		perror("fopen(\"test.out\")");
		goto FAIL0;
	}

	if (fprintf(output, "%d\n", x + y) < 0) {
		perror("fprintf");
		goto FAIL1;
	}

	if (fclose(output) != 0) {
		perror("fclose(output)");
		goto FAIL0;
	}

	if (fclose(input) != 0) {
		perror("fclose(input)");
		return 1;
	}

	return 0; /* sucess */

FAIL1:	(void) fclose(output);
FAIL0:	(void) fclose(input);
	return 1;
}
