#ifndef inputinterface_h
#define inputinterface_h
#include "Input.h"
#include <vector>

class inputinterface
{
public:
	~inputinterface();
	unsigned int	loadkeys		(std::string filename);
	float			getaction		(std::string request , inputdevices * controllertype = NULL,unsigned int * controllerNum = NULL);
	void			Update			();

private:

	std::vector<action> actions;
	input				Input;

	//singleton
private:
	static inputinterface *		Instance;
	inputinterface();
public:
	static inputinterface *		I();

};

#endif