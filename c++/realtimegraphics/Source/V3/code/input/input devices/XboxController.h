#ifndef xboxcontroller_h
#define xboxcontroller_h
#include "Controller.h"

enum xboxbuttons
{
	E_Up		= 1,
	E_down		= 2,
	E_left		= 4,
	E_right		= 8,
	E_start		= 16,
	E_back		= 32,
	E_LA		= 64,
	E_RA		= 128,
	E_LB		= 256,
	E_RB		= 512,
	E_A			= 4096,
	E_B			= 8192,
	E_X			= 16384,
	E_Y			= 32768
};

class xboxcontroller: public controller
{
public:
	xboxcontroller();
	~xboxcontroller();
	virtual void	initialize	(const LPDIRECTINPUTDEVICE8 inputdevice);
	virtual float	getpress	(const action& keyID);
	virtual bool	capture		();
private:

	XINPUT_STATE			xboxpad;
	unsigned int			padnum;
};

#endif