#include "XboxController.h"

#define DEADSPOT 10000
static unsigned int numofpads	=	0;
xboxcontroller::xboxcontroller()
{
	ZeroMemory( &xboxpad, sizeof(xboxpad) );
}

xboxcontroller::~xboxcontroller()
{

}

//void xboxcontroller::initialize()
//{
	//HWND hwindow	= FindWindow("MyWindowClass",NULL);
	//HINSTANCE hInst = (HINSTANCE) (LONG_PTR) GetWindowLongPtr( hwindow, GWLP_HINSTANCE ); 
	//DirectInput8Create(hInst, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&m_diObject, NULL);
	//initialize(m_diObject);
//}

void xboxcontroller::initialize(const LPDIRECTINPUTDEVICE8 inputdevice)
{ 
	padnum = numofpads++;
	InputDevice	=	inputdevice;
}

float	xboxcontroller::getpress(const action& keyID)
{
	if (keyID.keyname == "up" && ((xboxpad.Gamepad.wButtons & E_Up)==E_Up))
	{
		return 1.0f;
	}
	if (keyID.keyname == "down" && ((xboxpad.Gamepad.wButtons & E_down)==E_down))
	{
		return 1.0f;
	}
	if (keyID.keyname == "right" && ((xboxpad.Gamepad.wButtons & E_right)==E_right))
	{
		return 1.0f;
	}
	if (keyID.keyname == "left" && ((xboxpad.Gamepad.wButtons & E_left)==E_left))
	{
		return 1.0f;
	}
	if (keyID.keyname == "A" && ((xboxpad.Gamepad.wButtons & E_A)==E_A))
	{
		return 1.0f;
	}
	if (keyID.keyname == "B" && ((xboxpad.Gamepad.wButtons & E_B)==E_B))
	{
		return 1.0f;
	}
	if (keyID.keyname == "X" && ((xboxpad.Gamepad.wButtons & E_X)==E_X))
	{
		return 1.0f;
	}
	if (keyID.keyname == "Y" && ((xboxpad.Gamepad.wButtons & E_Y)==E_Y))
	{
		return 1.0f;
	}
	if (keyID.keyname == "start" && ((xboxpad.Gamepad.wButtons & E_start)==E_start))
	{
		return 1.0f;
	}
	if (keyID.keyname == "back" && ((xboxpad.Gamepad.wButtons & E_back)==E_back))
	{
		return 1.0f;
	}
	if (keyID.keyname == "back left" && ((xboxpad.Gamepad.wButtons & E_LB)==E_LB))
	{
		return 1.0f;
	}
	if (keyID.keyname == "back right" && ((xboxpad.Gamepad.wButtons & E_RB)==E_RB))
	{
		return 1.0f;
	}
	if (keyID.keyname == "left analog stick: left" )
	{
		if ( xboxpad.Gamepad.sThumbLX < -DEADSPOT )
		{
			return ( ( xboxpad.Gamepad.sThumbLX - DEADSPOT ) / ( -32768.0f - DEADSPOT ) );
		}
	}
	if (keyID.keyname == "left analog stick: right" )
	{
		if ( xboxpad.Gamepad.sThumbLX > DEADSPOT )
		{
			return ( ( xboxpad.Gamepad.sThumbLX - DEADSPOT )  / ( 32768.0f - DEADSPOT ) );
		}
	}
	if (keyID.keyname == "left analog stick: up" )
	{
		if ( xboxpad.Gamepad.sThumbLY < -DEADSPOT )
		{
			return ( ( xboxpad.Gamepad.sThumbLY - DEADSPOT ) / ( -32768.0f - DEADSPOT ) );
		}
	}
	if (keyID.keyname == "left analog stick: down" )
	{
		if ( xboxpad.Gamepad.sThumbLY > DEADSPOT )
		{
			return ( ( xboxpad.Gamepad.sThumbLY - DEADSPOT )  / ( 32768.0f - DEADSPOT ) );
		}
	}
	if (keyID.keyname == "right analog stick: left" )
	{
		if ( xboxpad.Gamepad.sThumbRX < -DEADSPOT )
		{
			return ( ( xboxpad.Gamepad.sThumbRX - DEADSPOT ) / ( -32768.0f - DEADSPOT ) );
		}
	}
	if (keyID.keyname == "right analog stick: right" )
	{
		if ( xboxpad.Gamepad.sThumbRX > DEADSPOT )
		{
			return ( ( xboxpad.Gamepad.sThumbRX - DEADSPOT )  / ( 32768.0f - DEADSPOT ) );
		}
	}
	if (keyID.keyname == "right analog stick: up" )
	{
		if ( xboxpad.Gamepad.sThumbRY < -DEADSPOT )
		{
			return ( ( xboxpad.Gamepad.sThumbRY - DEADSPOT ) / ( -32768.0f - DEADSPOT ) );
		}
	}
	if (keyID.keyname == "right analog stick: down" )
	{
		if ( xboxpad.Gamepad.sThumbRY > DEADSPOT )
		{
			return ( ( xboxpad.Gamepad.sThumbRY - DEADSPOT )  / ( 32768.0f - DEADSPOT ) );
		}
	}
	
	return 0.0f;
}

bool	xboxcontroller::capture()
{
	ZeroMemory( &xboxpad, sizeof(xboxpad) );
	XInputGetState(padnum, &xboxpad);
	//HRESULT h;
	//if ( FAILED( h=InputDevice->Poll() ) ) 
	//{
	//	while( h = InputDevice->Acquire() == DIERR_INPUTLOST) 
	//	{
	//	}
	//}

 //   if (FAILED( h = InputDevice->GetDeviceState(sizeof(XINPUT_STATE), &xboxpad) ) )
	//{
	//	return false;
	//}

	return true;
}
