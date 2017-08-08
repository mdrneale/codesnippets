#ifndef model_h
#define model_h
#include <d3d9.h>
#include <d3dx9.h>
#include "..\mesh\mesh.h"

class Model
{
public:
	Model():	MeshID(-1), 
				MaterialID(-1), 
				diffuseMapID(-1), 
				normalMapID(-1),
				SpecularHeightMapID(-1), 
				EffectID(-1),
				Position(0,0,0),
				Rotation(0,0,0),
				Scaling(1.0f,1.0f,1.0f),
				SpecPow(1)	
	{
		BaseCol[0]=1;
		BaseCol[1]=1;
		BaseCol[2]=1;

		SpecCol[0]=1;
		SpecCol[1]=1;
		SpecCol[2]=1;
	};
	int		MeshID;
	int		MaterialID;
	int		diffuseMapID;
	int		normalMapID;
	int		SpecularHeightMapID;
	int		EffectID;
	float	BaseCol[3];
	float	SpecCol[3];
	float	SpecPow;


	D3DXVECTOR3 Rotation;
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Scaling;
};

#endif
