#ifndef action_h
#define action_h
#include <string>

enum inputdevices
{
	e_nodevice,
	e_anydevice,
	e_keyboard,
	e_mouse,
	e_xboxcontroller,
	e_ps3controller,
	e_ps3guitar,
	e_ps3drums,
	e_numberofinputdevices
};

class action
{
public:
	std::string		message,
					keyname;
	bool			isascii;
	inputdevices	actionfor;
	int				keyname2;
};

#endif