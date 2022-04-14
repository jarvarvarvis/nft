#include "../nft.h"
#include "test1_suite_other.h"
#include "test1_suite_nice.h"

nft_suite(ok)
{
	nft_assert(5 != 9, "Compare 5 and 9");
	nft_assert(5 == 5, "Compare 5 and 5");
	nft_assert(NULL,   "Null");
}

int main()
{
	nft_run_suite(ok);
	nft_run_suite(other);
	nft_run_suite(nice);
	nft_log_final();
}
