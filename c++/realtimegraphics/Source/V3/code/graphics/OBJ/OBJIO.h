#ifndef OBJIO_H
#define OBJIO_H
#include "..\mesh\mesh.h"

class OBJIO
{
public:
	void load(const char* file,Mesh* meshtofill,LPDIRECT3DDEVICE9		gD3dDevice);
};

#endif