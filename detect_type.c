#include <stdio.h>
#include <assert.h>

#include "Utils.h"

int
main(int argc, char **argv)
{
	assert(DetectType("123") == INTEGER_TYPE);
	assert(DetectType("-123") == INTEGER_TYPE);
	assert(DetectType("1.23") == FLOAT_TYPE);
	assert(DetectType("-1.23") == FLOAT_TYPE);
	assert(DetectType("1+2") == EXPR_TYPE);
	assert(DetectType("1-2") == EXPR_TYPE);
	assert(DetectType("1 - 2") == EXPR_TYPE);
	assert(DetectType("abs(-1)") == EXPR_TYPE);
	assert(DetectType("-Abc") == EXPR_TYPE);
	assert(DetectType("-") == EXPR_TYPE);
	assert(DetectType("\"Hello\"") == STRING_TYPE);
	assert(DetectType("\"123\"") == STRING_TYPE);
	assert(DetectType("\"1.23\"") == STRING_TYPE);

	const char *NAMES[5] = {"EXPR_TYPE", "INTEGER_TYPE", "FLOAT_TYPE", "STRING_TYPE", "UNKNOWN_TYPE"};
	if (argv[1]) {
		printf("%s\n", NAMES[DetectType(argv[1])]);
	}

	return 0;
}
