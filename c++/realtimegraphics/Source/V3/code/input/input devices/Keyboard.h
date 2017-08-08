#ifndef keyboard_h
#define keyboard_h
#include "Controller.h"

class keyboard: public controller
{
public:
	keyboard();
	~keyboard();
	virtual void	initialize	(const LPDIRECTINPUTDEVICE8 inputdevice);
	virtual float	getpress	(const action& keyID);
	virtual bool	capture		();
private:

	BYTE					keys[256];
};

#endif