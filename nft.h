#ifndef _LIB_NFT_HEADER
#define _LIB_NFT_HEADER


// C++ compatibility
#ifdef __cplusplus
extern "C" {
#endif


/// Utility and configuration
#define NFT_STRINGIFY(v) #v

// The NFT_NO_STL macro disables the usage of C standard library functions.
// This is mainly for compatibility reasons: If you don't have access to 
// the standard library, you can provide alternative definitions to the
// wrapper macros.
#ifndef NFT_NO_STL
#include <stdio.h>
#define NFT_PRINTF(fmt, ...)          printf(fmt __VA_OPT__(,) __VA_ARGS__)

#ifndef NFT_NO_STREQ
#include <string.h>
#define NFT_STRCMP(a, b)              strcmp(a, b)
#endif

#endif

// Colored output breaks with terminals that do not support ANSI escape sequences.
// The NFT_NO_COLOR macro disables the use of these escape sequences.
#ifdef NFT_NO_COLOR
#define NFT_ANSI_RESET
#define NFT_ANSI_RED
#define NFT_ANSI_GREEN
#define NFT_ANSI_WHITE
#define NFT_ANSI_COLORED(str, col) str
#else
#define NFT_ANSI_RESET "\033[0m"
#define NFT_ANSI_RED   "\033[0;31m"
#define NFT_ANSI_GREEN "\033[0;32m"
#define NFT_ANSI_WHITE "\033[0;37m"
#define NFT_ANSI_COLORED(str, col) NFT_ANSI_##col str NFT_ANSI_RESET
#endif


/// Tests
#define nft_assert(value, description) \
	__suite_data->tests += 1; \
	NFT_PRINTF("  %s... ", description); \
	if ((value)) \
	{ \
	       	NFT_PRINTF(NFT_ANSI_COLORED("ok\n", GREEN)); \
	} \
	else \
	{ \
		NFT_PRINTF(NFT_ANSI_COLORED("failed\n", RED)); \
		__suite_data->failed += 1; \
	}

#define nft_assert_eq(a, b, description)     nft_assert((a) == (b), description)
#define nft_assert_neq(a, b, description)    nft_assert((a) != (b), description)

#ifndef NFT_NO_STREQ
#define nft_assert_streq(a, b, description)  nft_assert(NFT_STRCMP((a), (b)) == 0, description)
#define nft_assert_strneq(a, b, description) nft_assert(NFT_STRCMP((a), (b)) != 0, description)
#endif

/// Test suites
struct nft_suite_data
{
	long tests;
	long failed;
};

#define nft_suite(name, ...) \
	void __nft_suite_##name(struct nft_suite_data *__suite_data __VA_OPT__(,) __VA_ARGS__)

void nft_log_results(struct nft_suite_data *data)
{
	if (data->failed == 0)
		NFT_PRINTF("\nNo tests failed.\n\n");
	else
		NFT_PRINTF("\n%d out of %d test(s) failed.\n\n", 
				data->failed, data->tests);
}

// The NFT_NO_FINAL_RESULTS macro disables the collection of all test results into a final
// result. If this macro is defined, the nft_log_final macro will also not be defined.
#ifdef NFT_NO_FINAL_RESULTS

#define nft_run_suite(name, ...) \
	struct nft_suite_data __nft_suite_##name##_data = {.tests = 0, .failed = 0}; \
	NFT_PRINTF("Running suite \"%s\"...\n", NFT_STRINGIFY(name)); \
	__nft_suite_##name(&__nft_suite_##name##_data __VA_OPT__(,) __VA_ARGS__); \
	nft_log_results(&__nft_suite_##name##_data); \

#else

static struct nft_suite_data __nft_final_data = {.tests = 0, .failed = 0};

#define nft_run_suite(name, ...) \
	struct nft_suite_data __nft_suite_##name##_data = {.tests = 0, .failed = 0}; \
	NFT_PRINTF("Running suite \"%s\"...\n", NFT_STRINGIFY(name)); \
	__nft_suite_##name(&__nft_suite_##name##_data __VA_OPT__(,) __VA_ARGS__); \
	nft_log_results(&__nft_suite_##name##_data); \
	__nft_final_data.tests  += __nft_suite_##name##_data.tests; \
	__nft_final_data.failed += __nft_suite_##name##_data.failed;

#define nft_log_final() \
	NFT_PRINTF("\n ====== Final ====== \n"); \
	nft_log_results(&__nft_final_data); \
	NFT_PRINTF(" =================== \n")

#endif

// C++ compatibility
#ifdef __cplusplus
}
#endif

#endif /* _LIB_NFT_HEADER */
