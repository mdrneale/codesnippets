#include "InputInterface.h"
#include <string>
#include "InputData.h"

//singleton
inputinterface *	inputinterface::Instance = NULL;

inputinterface *	inputinterface::I()
{
	if ( !Instance )
	{
		Instance = new inputinterface();
	}
	return Instance;
}


inputinterface::inputinterface(void)
{
	Input.Initalise();
}

inputinterface::~inputinterface(void)
{
	if ( Instance )
	{
		Instance=NULL;
	}
}

void inputinterface::Update()
{
	Input.Update();
}

unsigned int inputinterface::loadkeys(std::string filename)
{
	inputdata test;
	test.loadfromxml(filename.c_str(),actions);

	return 1;
}

float inputinterface::getaction(std::string request,inputdevices * controllertype,unsigned int * controllerNum )
{
	float keyvalue=0.0f;
	for (unsigned int a=0; a<actions.size(); ++a)
	{
		if (actions[a].message==request)
		{
			keyvalue = Input.getvalue( actions[a] , controllerNum );
			if (keyvalue)
			{
				if (controllertype)
				{
					*controllertype = actions[a].actionfor;
				}
				break;
			}
		}
	}
	return keyvalue;
}