/*
ID: wrpaape1
LANG: C
TASK: money
*/

#include <stdio.h>
#include <assert.h>

enum {
	V_MAX = 25,
	N_MAX = 10000,
};

unsigned long long
solve(const unsigned int *coins,
      unsigned int        v,
      unsigned int	  n)
{
	unsigned long long SOLUTION[2][N_MAX + 1];

	for (unsigned int num_coins = 1; num_coins <= v; ++num_coins) {
		unsigned int prev_num_coins    = num_coins - 1;
		unsigned int prev_num_coins_id = prev_num_coins & 1;
		unsigned int num_coins_id      = num_coins & 1;
		SOLUTION[prev_num_coins_id][0] = 1;
		for (unsigned int money = 0; money <= n; ++money) {
			unsigned long long sol = 0;
			unsigned int coin = coins[prev_num_coins];

			for (int rem_money = money; rem_money >= 0;
			     rem_money -= coin) {
				sol += SOLUTION[prev_num_coins_id][rem_money];
			}

			SOLUTION[num_coins_id][money] = sol;
		}
	}

	return SOLUTION[v & 1][n];
}


int
main(void)
{
	FILE *input, *output;
	unsigned int V, N;
	unsigned int coins[V_MAX];

	assert(input = fopen("money.in", "r"));
	assert(fscanf(input, "%u %u", &V, &N) == 2);
	for (unsigned int i = 0; i < V; ++i) {
		assert(fscanf(input, "%u", &coins[i]) == 1);
	}
	assert(fclose(input) == 0);

	unsigned long long solution = solve(coins, V, N);

	assert(output = fopen("money.out", "w"));
	assert(fprintf(output, "%llu\n", solution) > 0);
	assert(fclose(output) == 0);

	return 0;
}
