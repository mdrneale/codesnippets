#ifndef input_h
#define input_h
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <XInput.h>
#include "input devices/Controller.h"
#include <vector>
#include "..\helper code\Timer.h"

class input
{
public:
	 input									(														);
	~input									(														);
	bool			Initalise				(														);
	void			Update					(														);
	float			getvalue				( action& button , unsigned int * whopressedit = NULL	);


	BOOL CALLBACK	EnumCallback			( const DIDEVICEINSTANCE* instance , VOID* context		);
	BOOL CALLBACK	EnumObjectsCallback		( const DIDEVICEOBJECTINSTANCE* pdidoi , VOID* pContext );

private:

	Timer t;
	void			refreshall				(														);
	void			refreshdevice			( unsigned int inputdevice								);

	bool			InitaliseDInput			(														);
	bool			FindValidControllers	(														);
	bool			InitaliseController		(														);
	bool			Add360Controller		(														);
	bool			AddKeyboard				(														);
	bool			AddMouse				(														);
	bool			AddPS3Guitar			(														);
	bool			AddPS3Drums				(														);
	bool			AddPS3Controller		(														);
	std::vector<controller*>			controllers;

	unsigned int						NumberOfControllers;

	std::vector<LPDIRECTINPUTDEVICE8>	InputDevices;
	LPDIRECTINPUT8						m_diObject;
	LPDIRECTINPUTDEVICE8				CurrentJoyStick;
	DIDEVICEINSTANCE					CurrentJoyStickDevice;
	HWND								hwindow;
};
	BOOL CALLBACK EnumCallback			( const DIDEVICEINSTANCE* instance , VOID* context		);
	BOOL CALLBACK EnumObjectsCallback	( const DIDEVICEOBJECTINSTANCE* pdidoi , VOID* pContext	);
#endif