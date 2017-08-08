#ifndef HelperCode_h
#define HelperCode_h
#include <sstream>

#define Min(a,b) a<b?a:b
#define Max(a,b) a>b?a:b
#define Limit(num,small,big) Min(Max(num,small),big)
#define	Lerp(a,b,ratio)	(a+((b-a)*ratio))

template <class T>
std::string ToString(T val)
{
	std::ostringstream sval;
	sval << val; 
	return sval.str();   
}

char * CreateString( const char * GivenString );

#endif