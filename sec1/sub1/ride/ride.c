/*
ID: wrpaape1
LANG: C
TASK: ride
*/

#include <stdio.h>  /* fdopen, fdclose, getline */
#include <stdlib.h> /* free */

#define TOKEN_VALUE(TOKEN) (((TOKEN) + 1) - 'A')

int
main(void)
{
	FILE *input;
	FILE *output;
	char *line;
	const char *ptr;
	size_t capacity;
	unsigned int comet;
	unsigned int group;
	unsigned int token;

	input = fopen("ride.in", "r");
	if (input == NULL) {
		perror("fopen(\"ride.in\")");
		return 1;
	}

	line     = NULL;
	capacity = 0;

	if (getline(&line,
		    &capacity,
		    input) < 0) {
		perror("getline");
		goto FAIL1;
	}

	comet = TOKEN_VALUE((unsigned int) *line);

	for (ptr = line + 1; token = (unsigned int) *ptr, token != '\n'; ++ptr)
		comet *= TOKEN_VALUE(token);

	if (getline(&line,
		    &capacity,
		    input) < 0) {
		perror("getline");
		goto FAIL1;
	}

	group = TOKEN_VALUE((unsigned int) *line);

	for (ptr = line + 1; token = (unsigned int) *ptr, token != '\n'; ++ptr)
		group *= TOKEN_VALUE(token);

	output = fopen("ride.out", "w");
	if (output == NULL) {
		perror("fopen(\"ride.out\")");
		goto FAIL1;
	}

	const char *solution = ((comet % 47) == (group % 47))
		             ? "GO\n"
			     : "STAY\n";

	if (fputs(solution, output) < 0) {
		perror("fputs");
		goto FAIL2;
	}

	if (fclose(output) != 0) {
		perror("fclose(output)");
		goto FAIL1;
	}

	if (fclose(input) != 0) {
		perror("fclose(input)");
		goto FAIL0;
	}

	free(line);
	return 0; /* sucess */

FAIL2:	(void) fclose(output);
FAIL1:	(void) fclose(input);
FAIL0:	free(line);
	return 1;
}
