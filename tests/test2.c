#include "../nft.h"

#include <math.h>

float slow_function(int input, int i)
{
	float res = cos((float)i) * tanh(input - i) / 0.34234789;
	for (int i = 0; i < 700000; ++i)
	{
		res = tan(sin(res));
	}	
	return res;
}

nft_suite(slow, int input)
{
	for (int i = 0; i < input; ++i)
	{
		char text[32];
		sprintf(text, "Calculation %d", i);
		nft_assert(slow_function(input, i) > 0.25, text);
	}
}

nft_suite(first, int x)
{
	nft_assert(x, "Check input value not null");
	nft_assert_eq(x, 6, "Check input value equals 6");
	nft_assert(x > 0, "Check input value greater than 0");
}

int main()
{
	nft_run_suite(first, 5);
	nft_run_suite(slow, 100);
	nft_log_final();
}
