#ifndef controller_h
#define controller_h
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <XInput.h>
#include "..\Action.h"


class controller
{
public:
	controller()	{};
	~controller()	{};
	virtual void	initialize	(const LPDIRECTINPUTDEVICE8 inputdevice);
	virtual float	getpress	(const action& keyID)	=0;
	virtual bool	capture		()						=0;

protected:

	LPDIRECTINPUTDEVICE8 InputDevice;
};

#endif