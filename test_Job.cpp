#include <stdio.h>
#include <assert.h>

#include "Job.h"

//#define TEST(cond) printf("."), assert(cond);
#define TEST(cond) assert(cond);

int
main(int argc, char **argv)
{
	string job0_name = "01.-1", job1_name = "1.0";
	string attr0_name = "attr0", attr0_value = "123";
//	AttributeValueType attr0_value_type = INTEGER_TYPE;
	string attr1_name = "attr1", attr1_value = "1.23";
//	AttributeValueType attr1_value_type = FLOAT_TYPE;
	string attr2_name = "attr2", attr2_value = "-1.23";
//	AttributeValueType attr2_value_type = FLOAT_TYPE;
	string attr3_name = "attr3", attr3_value = "\"1 2 3\"";
//	AttributeValueType attr3_value_type = STRING_TYPE;
	string attr4_name = "attr4", attr4_value = "1+1";
//	AttributeValueType attr4_value_type = EXPR_TYPE;

	Job job0(job0_name, NULL);

	job0.Set(attr0_name, attr0_value);
	job0.Set(attr1_name, attr1_value);
	job0.Set(attr2_name, attr2_value);
	job0.Set(attr3_name, attr3_value);

	string attr;
	TEST(job0.Get(attr0_name, attr));
//	TEST(attr.GetType() == attr0_value_type);
//	TEST(attr.GetValue() == attr0_value);
	TEST(attr == attr0_value);
	TEST(job0.Get(attr1_name, attr));
//	TEST(attr.GetType() == attr1_value_type);
//	TEST(attr.GetValue() == attr1_value);
	TEST(attr == attr1_value);
	TEST(job0.Get(attr2_name, attr));
//	TEST(attr.GetType() == attr2_value_type);
//	TEST(attr.GetValue() == attr2_value);
	TEST(attr == attr2_value);
	TEST(job0.Get(attr3_name, attr));
//	TEST(attr.GetType() == attr3_value_type);
//	TEST(attr.GetValue() == attr3_value);
	TEST(attr == attr3_value);

	Job job1(job1_name, &job0);

	TEST(job1.Get(attr0_name, attr));
//	TEST(attr.GetType() == attr0_value_type);
//	TEST(attr.GetValue() == attr0_value);
	TEST(attr == attr0_value);
	TEST(job1.Get(attr1_name, attr));
//	TEST(attr.GetType() == attr1_value_type);
//	TEST(attr.GetValue() == attr1_value);
	TEST(attr == attr1_value);
	TEST(job1.Get(attr2_name, attr));
//	TEST(attr.GetType() == attr2_value_type);
//	TEST(attr.GetValue() == attr2_value);
	TEST(attr == attr2_value);
	TEST(job1.Get(attr3_name, attr));
//	TEST(attr.GetType() == attr3_value_type);
//	TEST(attr.GetValue() == attr3_value);
	TEST(attr == attr3_value);

	job1.Set(attr4_name, attr4_value);
	TEST(job1.Get(attr4_name, attr));
//	TEST(attr.GetType() == attr4_value_type);
//	TEST(attr.GetValue() == attr4_value);
	TEST(attr == attr4_value);

	job1.Set(attr1_name, attr4_value);
	TEST(job1.Get(attr1_name, attr));
//	TEST(attr.GetType() == attr4_value_type);
//	TEST(attr.GetValue() == attr4_value);
	TEST(attr == attr4_value);

	job0.Delete(attr2_name);
	TEST(!job0.Get(attr2_name, attr));
	TEST(!job1.Get(attr2_name, attr));

	return 0;
}
