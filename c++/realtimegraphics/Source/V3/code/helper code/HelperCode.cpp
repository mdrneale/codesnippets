#include "HelperCode.h"

char * CreateString( const char * GivenString )
{

	std::string test;
	if (GivenString)
	{
		test = GivenString;
	}
	else
	{
		test = "";
	}
	char * Value = new char[ test.size() + 1 ];
	strcpy_s ( Value, (test.size() + 1)*sizeof(char), test.c_str() );
	return Value;
}