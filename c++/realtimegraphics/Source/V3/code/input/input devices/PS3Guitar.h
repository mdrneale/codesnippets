#ifndef PS3Guitar_h
#define PS3Guitar_h
#include "Controller.h"

enum PS3GuitarButtons
{
	PS3GuitarButtons_Yellow		= 0,
	PS3GuitarButtons_Green		= 1,
	PS3GuitarButtons_Red		= 2,
	PS3GuitarButtons_Blue		= 3,
	PS3GuitarButtons_Orange		= 4,
	PS3GuitarButtons_Select		= 8,
	PS3GuitarButtons_Start		= 9,
	PS3GuitarButtons_PSButton	= 12,

	PS3GuitarButtons_Up			= 0,
	PS3GuitarButtons_Right		= 9000,
	PS3GuitarButtons_Down		= 18000,	
	PS3GuitarButtons_Left		= 27000,	

	PS3GuitarButton_IsPressed	= 128	
};

class PS3Guitar: public controller
{
public:
	PS3Guitar();
	~PS3Guitar();
	virtual void	initialize	(const LPDIRECTINPUTDEVICE8 inputdevice);
	virtual float	getpress	(const action& keyID);
	virtual bool	capture		();
private:

	DIJOYSTATE2 			PS3GuitarPad;
};

#endif