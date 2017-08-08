#ifndef mesh_h
#define mesh_h
#include <d3d9.h>
#include <d3dx9.h>
//#include "..\..\xml\tsf\include\tsf\tsf.hpp"

struct Mesh
{
	Mesh();
	~Mesh();
//	void Initialise(tsf::mesh &TyronesMesh);
	void triangle(LPDIRECT3DDEVICE9		gD3dDevice);
	IDirect3DVertexBuffer9		*_vb; 
	IDirect3DIndexBuffer9		*_ib; 
	int NumofVerts;
	int PrimitiveCount;
};

#endif
