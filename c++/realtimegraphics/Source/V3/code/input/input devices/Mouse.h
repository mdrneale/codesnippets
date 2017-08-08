#ifndef mouse_h
#define mouse_h
#include "Controller.h"

class mouseinfo
{
public:
	float	x,
			y,
			z;
	unsigned char buttons[8];	
};

class mouse: public controller
{
public:
	mouse();
	~mouse();
	virtual void	initialize	(const LPDIRECTINPUTDEVICE8 inputdevice);
	virtual float	getpress	(const action& keyID)	;
	virtual bool	capture		();
private:

	mouseinfo info;
};

#endif