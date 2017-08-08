#ifndef inputdata_h
#define inputdata_h
#include <vector>
#include "Action.h"

class inputdata
{
public:

	inputdata					(														);
	~inputdata					(														);
	inputdevices	loadfromxml	( const char * const file,std::vector<action>& tofill	);
	char			AsciiToDIK	( char ascii											);
protected:
private:
};

#endif