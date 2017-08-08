#include "VisualInterface.h"


VisualInterface::~VisualInterface()
{
	if (isinitialised)
	{
		CloseDownDirect3D();
	}
}

void VisualInterface::CloseDownDirect3D()
{
	for (unsigned int a=0; a<drawMethods.size();a++)
	{
		delete drawMethods.at(a);
		drawMethods.at(a)=NULL;
	}

	if (gD3dDevice)
	{
		gD3dDevice->Release();
		gD3dDevice=NULL;
	}
	if (gD3dObject)
	{
		gD3dObject->Release();
		gD3dObject=NULL;
	}
}










