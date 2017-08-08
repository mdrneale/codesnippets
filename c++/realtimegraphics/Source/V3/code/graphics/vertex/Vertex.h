#include <d3dx9.h>
#include "..\VisualInterface.h"

class Vertex
{
public:
	Vertex(){};
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR2 UVs;
};