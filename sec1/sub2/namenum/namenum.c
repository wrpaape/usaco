/*
ID: wrpaape1
LANG: C
TASK: namenum
*/

/* external dependencies
 * ────────────────────────────────────────────────────────────────────────── */
#include <stdio.h>	/* fopen, getline, fclose */
#include <assert.h>	/* assert */
#include <limits.h>	/* UCHAR_MAX */


/* macro constants
 * ────────────────────────────────────────────────────────────────────────── */
#define PROG		"namenum"
#define INPUT_FILE	PROG ".in"
#define OUTPUT_FILE	PROG ".out"
#define DICT_FILE	"dict.txt"
#define SN_MAX_LENGTH	12


int
main(void)
{
	FILE *input;
	FILE *dict;
	FILE *output;
	size_t line_size;
	size_t line_capacity;
	ssize_t size_read;
	size_t i;
	size_t count_digits;
	char *line;
	static size_t size_write;
	static char serial_number[SN_MAX_LENGTH + 1];
	static char line_buffer[SN_MAX_LENGTH + 2];
	static int token_map[UCHAR_MAX + 1];
	static int found_solution;
	int token;
	int digit;

	token_map['A'] = token_map['B'] = token_map['C'] = '2';
	token_map['D'] = token_map['E'] = token_map['F'] = '3';
	token_map['G'] = token_map['H'] = token_map['I'] = '4';
	token_map['J'] = token_map['K'] = token_map['L'] = '5';
	token_map['M'] = token_map['N'] = token_map['O'] = '6';
	token_map['P'] = token_map['R'] = token_map['S'] = '7';
	token_map['T'] = token_map['U'] = token_map['V'] = '8';
	token_map['W'] = token_map['X'] = token_map['Y'] = '9';

	assert(input = fopen(INPUT_FILE, "r"));

	line_size = fread(&serial_number[0],
			  sizeof(char),
			  sizeof(serial_number),
			  input);

	assert(line_size > 0);
	assert(fclose(input) == 0);

	assert(dict   = fopen(DICT_FILE,   "r"));
	assert(output = fopen(OUTPUT_FILE, "w"));

	count_digits  = line_size - 1;
	line          = &line_buffer[0];
	line_capacity = sizeof(line_buffer);

	while (1) {
		size_read = getline(&line,
				    &line_capacity,
				    dict);

		if (size_read < 0)
			break;

		if (size_read == line_size) {
			i = 0;
			while (1) {
				digit = serial_number[i];
				token = line[i];

				if (digit != token_map[(size_t) token])
					break;

				if (++i == count_digits) {
					size_write = fwrite(line,
							    sizeof(char),
							    line_size,
							    output);
					assert(size_write == line_size);
					break;
				}
			}
		}
	}


	if (!size_write) {
		size_write = fwrite("NONE\n",
				    sizeof(char),
				    sizeof("NONE"),
				    output);

		assert(size_write == sizeof("NONE"));
	}

	assert(fclose(output) == 0);
	assert(fclose(dict)   == 0);
	return 0;
}
