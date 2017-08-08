#ifndef PS3Drums_h
#define PS3Drums_h
#include "Controller.h"

enum PS3DrumsButtons
{
	PS3DrumsButtons_Square		= 0,
	PS3DrumsButtons_Blue		= 0,
	PS3DrumsButtons_Cross		= 1,
	PS3DrumsButtons_Green		= 1,
	PS3DrumsButtons_Circle		= 2,
	PS3DrumsButtons_Red			= 2,
	PS3DrumsButtons_Triangle	= 3,
	PS3DrumsButtons_Yellow		= 3,
	PS3DrumsButtons_FootPeddle	= 4,
	PS3DrumsButtons_Orange		= 5,
	PS3DrumsButtons_Select		= 8,
	PS3DrumsButtons_Start		= 9,
	PS3DrumsButtons_PSButton	= 12,

	PS3DrumsButtons_Up			= 0,
	PS3DrumsButtons_Right		= 9000,
	PS3DrumsButtons_Down		= 18000,	
	PS3DrumsButtons_Left		= 27000,

	PS3DrumButton_IsPressed		= 128
};

class PS3Drums: public controller
{
public:
	PS3Drums();
	~PS3Drums();
	virtual void	initialize	(const LPDIRECTINPUTDEVICE8 inputdevice);
	virtual float	getpress	(const action& keyID);
	virtual bool	capture		();
private:

	DIJOYSTATE2 			PS3DrumsPad;
};

#endif