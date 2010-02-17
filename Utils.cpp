#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "AttributeValueType.h"

AttributeValueType
DetectType(const char *val)
{
	assert(val);

	if ('"' == val[0]) return STRING_TYPE;
	for (const char *tmp = val; '\0' != *tmp; tmp++) {
		if ('-' == *tmp && (tmp != val || '\0' == tmp[1])) return EXPR_TYPE;
		if (!isdigit(*tmp) && '-' != *tmp && '.' != *tmp) return EXPR_TYPE;
	}
	if (isdigit(val[0]) || '-' == val[0]) {
		if (strchr(val, '.')) {
			return FLOAT_TYPE;
		} else {
			return INTEGER_TYPE;
		}
	}

	return UNKNOWN_TYPE;
}
