#ifndef colour_h
#define colour_h

class Colour
{
public:
	Colour():R(1.0f),G(1.0f),B(1.0f){};
	Colour(float r,float g,float b):R(r),G(g),B(b){};
	float R,G,B;
};

#endif
