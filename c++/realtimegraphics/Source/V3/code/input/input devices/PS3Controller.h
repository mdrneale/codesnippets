#ifndef PS3Controller_h
#define PS3Controller_h
#include "Controller.h"

enum PS3ControllerButtons
{
	PS3ControllerButtons_select		= 0,
	PS3ControllerButtons_LA			= 1,
	PS3ControllerButtons_RA			= 2,
	PS3ControllerButtons_start		= 3,
	PS3ControllerButtons_Up			= 4,
	PS3ControllerButtons_right		= 5,
	PS3ControllerButtons_down		= 6,
	PS3ControllerButtons_left		= 7,
	PS3ControllerButtons_L2			= 8,
	PS3ControllerButtons_R2			= 9,
	PS3ControllerButtons_L1			= 10,
	PS3ControllerButtons_R1			= 11,
	PS3ControllerButtons_Triangle	= 12,
	PS3ControllerButtons_Circle		= 13,
	PS3ControllerButtons_Cross		= 14,
	PS3ControllerButtons_Square		= 15,
	PS3ControllerButtons_PSButton	= 16,

	PS3ControllerButton_IsPressed	= 128
};

class PS3Controller: public controller
{
public:
	PS3Controller();
	~PS3Controller();
	virtual void	initialize	(const LPDIRECTINPUTDEVICE8 inputdevice);
	virtual float	getpress	(const action& keyID);
	virtual bool	capture		();
private:

	DIJOYSTATE2 			PS3ControllerPad;
};

#endif