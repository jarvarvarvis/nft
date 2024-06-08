#ifndef _LIB_NFT_HEADER
#define _LIB_NFT_HEADER


// C++ compatibility
#ifdef __cplusplus
extern "C" {
#endif


/// Utility and configuration
#define NFT_STRINGIFY(v) #v

// Cross-platform way to extract the filename from the __FILE__ preprocessor macro.
// The issue with this implementation is the duplicate call to strrchr, but we
// can't use basename, as that one is a GNU function.
#define __NFT_GET_FILENAME(separator) (strrchr(__FILE__, separator) ? strrchr(__FILE__, separator) + 1 : __FILE__)

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
// This is probably a Windows system
#include <string.h>
#define NFT_FILENAME __NFT_GET_FILENAME('\\')

#elif defined(__linux__) || defined(linux) || defined(__linux)
// This is probably a Linux system
#include <string.h>
#define NFT_FILENAME __NFT_GET_FILENAME('/')

#elif defined(__APPLE__) || defined(__MACH__)
// This is probably an Apple system
#include <string.h>
#define NFT_FILENAME __NFT_GET_FILENAME('/')

#elif defined(__FreeBSD__)
// This is probably a FreeBSD system
#include <string.h>
#define NFT_FILENAME __NFT_GET_FILENAME('/')

#endif

// The NFT_NO_STL macro disables the usage of C standard library functions.
// This is mainly for compatibility reasons: If you don't have access to 
// the standard library, you can provide alternative definitions to the
// wrapper macros.
#ifndef NFT_NO_STL
#include <stdio.h>

#define NFT_PRINTF(fmt, ...)    printf(fmt, __VA_ARGS__)
#define NFT_PRINT(text)         NFT_PRINTF("%s", text)

#ifndef NFT_NO_STREQ
#include <string.h>
#define NFT_STRCMP(a, b) strcmp(a, b)
#endif /* NFT_NO_STREQ */

#endif /* NFT_NO_STL */

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
#endif /* NFT_NO_COLOR */


/// Tests
#define nft_assert(value, description) \
	__suite_data->tests += 1; \
	NFT_PRINT(" " description "... "); \
	if (value) \
	{ \
		NFT_PRINT(NFT_ANSI_COLORED("ok\n", GREEN)); \
	} \
	else \
	{ \
		NFT_PRINT(NFT_ANSI_COLORED("failed\n", RED)); \
		__suite_data->failed += 1; \
	}

#define nft_assert_eq(a, b, description)     nft_assert((a) == (b), description)
#define nft_assert_neq(a, b, description)    nft_assert((a) != (b), description)

#ifndef NFT_NO_STREQ
#define nft_assert_streq(a, b, description)  nft_assert(NFT_STRCMP((a), (b)) == 0, description)
#define nft_assert_strneq(a, b, description) nft_assert(NFT_STRCMP((a), (b)) != 0, description)
#endif /* NFT_NO_STREQ */


#define nft_assert_va(value, description, ...) \
	__suite_data->tests += 1; \
	NFT_PRINTF(" " description "... ", __VA_ARGS__); \
	if (value) \
	{ \
		NFT_PRINT(NFT_ANSI_COLORED("ok\n", GREEN)); \
	} \
	else \
	{ \
		NFT_PRINT(NFT_ANSI_COLORED("failed\n", RED)); \
		__suite_data->failed += 1; \
	}

#define nft_assert_eq_va(a, b, description, ...)     nft_assert_va((a) == (b), description, __VA_ARGS__)
#define nft_assert_neq_va(a, b, description, ...)    nft_assert_va((a) != (b), description, __VA_ARGS__)

#ifndef NFT_NO_STREQ
#define nft_assert_streq_va(a, b, description, ...)  nft_assert_va(NFT_STRCMP((a), (b)) == 0, description, __VA_ARGS__)
#define nft_assert_strneq_va(a, b, description, ...) nft_assert_va(NFT_STRCMP((a), (b)) != 0, description, __VA_ARGS__)
#endif /* NFT_NO_STREQ */

/// Test suites
struct nft_suite_data
{
	long tests;
	long failed;
};

// By default, nft will output the source file for every suite.
// The NFT_NO_SHOW_SUITE_FILE disables this feature.
#ifndef NFT_NO_SHOW_SUITE_FILE
#define nft_param_suite(name, ...) \
    char *__nft_suite_##name##_get_filename() { return NFT_FILENAME; } \
	void __nft_suite_##name(struct nft_suite_data *__suite_data, __VA_ARGS__)
#define nft_suite(name) \
    char *__nft_suite_##name##_get_filename() { return NFT_FILENAME; } \
	void __nft_suite_##name(struct nft_suite_data *__suite_data)
#else
#define nft_param_suite(name, ...) \
    char *__nft_suite_##name##_get_filename() { return NULL; } \
	void __nft_suite_##name(struct nft_suite_data *__suite_data, __VA_ARGS__)
#define nft_suite(name) \
    char *__nft_suite_##name##_get_filename() { return NULL; } \
	void __nft_suite_##name(struct nft_suite_data *__suite_data)
#endif /* NFT_NO_SHOW_SUITE_FILE */

void nft_log_results(struct nft_suite_data *data)
{
	if (data->failed == 0)
		NFT_PRINT("\nNo tests failed.\n\n");
	else
		NFT_PRINTF("\n%ld out of %ld test(s) failed.\n\n", data->failed, data->tests);
}

static struct nft_suite_data __nft_final_data = {0};

// The NFT_NO_FINAL_RESULTS macro disables the collection of all test results into a final
// result. If this macro is defined, the nft_log_final macro will also not be defined.
#ifdef NFT_NO_FINAL_RESULTS
#define nft_update_final_data(name)
#else
#define nft_update_final_data(name) \
    __nft_final_data.tests  += __nft_suite_##name##_data.tests; \
    __nft_final_data.failed += __nft_suite_##name##_data.failed;

#define nft_log_final() \
	NFT_PRINT("\n ====== Final ====== \n"); \
	nft_log_results(&__nft_final_data); \
	NFT_PRINT(" =================== \n")
#endif

#ifndef NFT_NO_SHOW_SUITE_FILE
#define nft_run_param_suite(name, ...) \
	struct nft_suite_data __nft_suite_##name##_data = {0}; \
	NFT_PRINTF("Running suite \"%s\" (defined in %s)...\n", NFT_STRINGIFY(name), __nft_suite_##name##_get_filename()); \
	__nft_suite_##name(&__nft_suite_##name##_data, __VA_ARGS__); \
	nft_log_results(&__nft_suite_##name##_data); \
    nft_update_final_data(name);
#define nft_run_suite(name) \
	struct nft_suite_data __nft_suite_##name##_data = {0}; \
	NFT_PRINTF("Running suite \"%s\" (defined in %s)...\n", NFT_STRINGIFY(name), __nft_suite_##name##_get_filename()); \
	__nft_suite_##name(&__nft_suite_##name##_data); \
	nft_log_results(&__nft_suite_##name##_data); \
    nft_update_final_data(name);
#else
#define nft_run_param_suite(name, ...) \
	struct nft_suite_data __nft_suite_##name##_data = {0}; \
	NFT_PRINTF("Running suite \"%s\"...\n", NFT_STRINGIFY(name)); \
	__nft_suite_##name(&__nft_suite_##name##_data, __VA_ARGS__); \
	nft_log_results(&__nft_suite_##name##_data); \
    nft_update_final_data(name);
#define nft_run_suite(name) \
	struct nft_suite_data __nft_suite_##name##_data = {0}; \
	NFT_PRINTF("Running suite \"%s\"...\n", NFT_STRINGIFY(name)); \
	__nft_suite_##name(&__nft_suite_##name##_data); \
	nft_log_results(&__nft_suite_##name##_data); \
    nft_update_final_data(name);
#endif /* NFT_NO_SHOW_SUITE_FILE */

// C++ compatibility
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LIB_NFT_HEADER */
