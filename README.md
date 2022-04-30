# nft - The nicer framework for testing

nft is a header-only library for creating automated tests in C/C++ projects.

The library provides a few useful macros:

### Suites

```
nft_suite(name)
  
	Define a new suite. 

nft_run_suite(name)
  
	Run the suite.
	By default, nft outputs the file which the suite was defined in.
	You can disable this feature by defining the NFT_NO_SHOW_SUITE_FILE macro.

nft_param_suite(name, args)

	Define a new suite and a list of arguments that can be passed to it when it 
	is run using the nft_run_param_suite macro.

nft_run_param_suite(name, args)

	Run the suite with the provided arguments from the suite definition.
	By default, nft outputs the file which the suite was defined in.
	You can disable this feature by defining the NFT_NO_SHOW_SUITE_FILE macro.

```

### Tests

```
nft_assert(expression, description)

	Run a test. If the expression evaluates to a non-zero value, the test is successful. 
	Otherwise, the test fails.
	The description is shown in the output.


nft_assert_eq(a, b, description)

	Run a test that checks if the values a and b are equal.

nft_assert_neq(a, b, description)

	Run a test that checks if the values a and b are not equal.


nft_assert_streq(a, b, description)

	Run a test that checks if the two given strings a and b are equal, according to the given
	definition of NFT_STRCMP. (See the Compatibility section for more information)
	Per default, NFT_STRCMP uses the C standard library function strcmp from the string.h
	header.
	You can disable the nft_assert_streq and nft_assert_strneq macros altogether by adding
		#define NFT_NO_STREQ
	before including nft.h.

nft_assert_strneq(a, b, description)

	Run a test that checks if the two given strings a and b are not equal.

```

### Results

```
nft_log_final()

	Per default, the results of all tests are collected into one final result. Insert
	this macro at the end of the main function to output those final results.
	If you want to disable this feature, insert
		#define NFT_NO_FINAL_RESULTS
	before including the nft.h header in your test files.

```

### Compatibility

If, for some reason, you don't have access to the C standard library headers (stdio.h and string.h),
you should insert
```
#define NFT_NO_STL
```
before including the nft.h header in your test files.

This assumes that you have provided custom definitions for the NFT\_PRINTF and NFT\_STRCMP macros.
Otherwise, your compiler will not be happy.


If your terminal doesn't support ANSI escape sequences, colored output will not work correctly.
You should insert
```
#define NFT_NO_COLOR
```
to disable the use of these sequences.
