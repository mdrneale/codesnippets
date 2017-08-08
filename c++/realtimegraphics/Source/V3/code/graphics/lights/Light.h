#ifndef light_h
#define light_h
#include "..\colour\Colour.h"
#include "..\camera\Camera.h"
#include <d3dx9.h>

class Light: public Camera
{
public:
	Light(): InnerFOV(D3DX_PI / 8.0f),Attenuation(0.25f,0.12f,0.07f),LightBox(-1){};
	Colour	Col;
	D3DXVECTOR3	 Attenuation;
	float	InnerFOV;
	int		LightBox;
};

#endif
