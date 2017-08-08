#ifndef material_h
#define material_h
#include "..\colour\Colour.h"

class Material
{
public:
	Material():Shininess(0.0f){};
	Colour	Emission;
	Colour	Ambient;
	Colour	Diffuse;
	Colour	Specular;
	float	Shininess;
};

#endif
