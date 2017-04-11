/*
ID: wrpaape1
LANG: C
TASK: frac1
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>


/* 1 + 2 + ... (N - 1) = (N - 1) * N / 2 */
#define N_MAX		160
#define FRAC_COUNT_MAX	(((N_MAX - 1) * N_MAX) / 2)

#define SIZE(BEGIN, END) (((char *) (END)) - ((char *) (BEGIN)))

static unsigned int N;
static FILE *output;

struct Frac {
	unsigned short numerator;
	unsigned short denominator;
};

static struct Frac fracs[FRAC_COUNT_MAX];
static struct Frac *const restrict fracs_begin = &fracs[0];
static struct Frac *restrict fracs_end	       = &fracs[0];

static inline void
append_frac(const unsigned int num,
	    const unsigned int den)
{
	fracs_end->numerator   = (unsigned short) num;
	fracs_end->denominator = (unsigned short) den;
	++fracs_end;
}

static inline unsigned int
greatest_common_divisor(unsigned int a,
			unsigned int b)
{
	unsigned int d;
	unsigned int a_odd, b_odd;

	d = 0;

	while (1) {
		a_odd = (a & 1);
		b_odd = (b & 1);

		if (a_odd || b_odd)
			break;

		++d;
		a /= 2;
		b /= 2;
	}

	while (a != b) {
		if (!a_odd)
			a /= 2, a_odd = (a & 1);
		else if (!b_odd)
			b /= 2, b_odd = (b & 1);
		else if (a > b)
			a = (a - b) / 2, a_odd = (a & 1);
		else
			b = (b - a) / 2, b_odd = (b & 1);
	}

	return a << d;
}

static inline void
generate_fracs(void)
{
	for (unsigned int den = 2; den <= N; ++den)
		for (unsigned int num = 1; num < den; ++num)
			if (greatest_common_divisor(num, den) <= 1)
				append_frac(num, den);
}


static inline int
frac_greater_than(const struct Frac *const restrict frac1,
		  const struct Frac *const restrict frac2)
{
	int num1, num2, den1, den2;
	int num1_den2, num2_den1;

	num1  = (int) frac1->numerator;
	den1  = (int) frac1->denominator;

	num2  = (int) frac2->numerator;
	den2  = (int) frac2->denominator;

	num1_den2 = num1 * den2;
	num2_den1 = num2 * den1;

	return num1_den2 > num2_den1;
}

static void
merge(struct Frac *const restrict begin,
      struct Frac *const restrict mid,
      struct Frac *const restrict end)
{
	static struct Frac tmp1[(FRAC_COUNT_MAX / 2) + 1];
	static struct Frac tmp2[(FRAC_COUNT_MAX / 2) + 1];
	struct Frac *restrict end1;
	struct Frac *restrict end2;
	struct Frac *restrict frac1;
	struct Frac *restrict frac2;
	struct Frac *restrict frac;
	size_t size1, size2;

	size1 = SIZE(begin, mid);
	size2 = SIZE(mid,   end);

	(void) memcpy(&tmp1[0], begin, size1);
	(void) memcpy(&tmp2[0], mid,   size2);

	frac1 = &tmp1[0];
	end1  = (struct Frac *) (((char *) frac1) + size1);
	frac2 = &tmp2[0];
	end2  = (struct Frac *) (((char *) frac2) + size2);

	frac = begin;

	while (1) {
		while (1) {
			if (frac1 == end1) {
				(void) memcpy(frac, frac2,
					      SIZE(frac2, end2));
				return;
			}

			if (frac_greater_than(frac1,
					      frac2))
				break;

			*frac++ = *frac1++;
		}

		while (2) {
			if (frac2 == end2) {
				(void) memcpy(frac, frac1,
					      SIZE(frac1, end1));
				return;
			}

			if (frac_greater_than(frac2,
					      frac1))
				break;

			*frac++ = *frac2++;
		}
	}

}

static void
stable_sort(struct Frac *const restrict begin,
	    struct Frac *const restrict end)
{
	struct Frac *restrict mid;
	size_t len;

	len = end - begin;

	if (len <= 1)
		return;

	mid = begin + (len / 2);

	stable_sort(begin, mid);
	stable_sort(mid,   end);

	merge(begin, mid, end);
}

static inline void
print_unique_asc_fracs(void)
{
	const struct Frac *restrict frac;

	assert(output = fopen("frac1.out", "w"));
	assert(fputs("0/1\n", output) != EOF);

	/* stable so equal fractions are ordered by
	 * smallest numerator/denominator */
	stable_sort(fracs_begin,
		    fracs_end);

	for (frac = fracs_begin; frac < fracs_end; ++frac)
		assert(fprintf(output, "%hu/%hu\n",
			       frac->numerator,
			       frac->denominator) > 0);

	assert(fputs("1/1\n", output) != EOF);
	assert(fclose(output) == 0);
}

int
main(void)
{
	FILE *input;

	assert(input = fopen("frac1.in", "r"));
	assert(fscanf(input, "%u\n", &N) == 1);
	assert(fclose(input) == 0);

	generate_fracs();

	print_unique_asc_fracs();

	return 0;
}


