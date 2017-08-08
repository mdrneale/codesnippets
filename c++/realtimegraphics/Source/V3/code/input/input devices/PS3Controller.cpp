#include "PS3Controller.h"

#define DEADSPOT 500

PS3Controller::PS3Controller()
{
	ZeroMemory( &PS3ControllerPad, sizeof(DIJOYSTATE2) );
}

PS3Controller::~PS3Controller()
{

}

void PS3Controller::initialize(const LPDIRECTINPUTDEVICE8 inputdevice)
{
	InputDevice	=	inputdevice;
}

float	PS3Controller::getpress(const action& keyID)
{
	if ( ( keyID.keyname == "select" )		&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_select]		== PS3ControllerButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "start" )		&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_start]		== PS3ControllerButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "up" )			&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_Up]			== PS3ControllerButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "right" )		&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_right]		== PS3ControllerButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "down" )		&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_down]			== PS3ControllerButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "left" )		&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_left]			== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "square" )		&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_Square]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "circle" )		&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_Circle]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "triangle" )	&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_Triangle]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "cross" )		&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_Cross]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "ps button" )	&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_PSButton]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "L1" )			&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_L1]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "L2" )			&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_L2]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "R1" )			&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_R1]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "R2" )			&& ( PS3ControllerPad.rgbButtons[PS3ControllerButtons_R2]		== PS3ControllerButton_IsPressed ) )	
	{
		return 1.0f;
	}

	if ( ( keyID.keyname == "left analog stick: up" ) )	
	{
		if ( PS3ControllerPad.lY < -DEADSPOT )
		{
			return (PS3ControllerPad.lY + DEADSPOT) / -( 10000.0f - DEADSPOT );
		}
	}
	if ( ( keyID.keyname == "left analog stick: down" ) )	
	{
		if ( PS3ControllerPad.lY > DEADSPOT )
		{
			return (PS3ControllerPad.lY - DEADSPOT) / ( 10000.0f - DEADSPOT );
		}
	}
	if ( ( keyID.keyname == "left analog stick: left" ) )	
	{
		if ( PS3ControllerPad.lX < -DEADSPOT )
		{
			return (PS3ControllerPad.lX + DEADSPOT) / -( 10000.0f - DEADSPOT );
		}
	}
	if ( ( keyID.keyname == "left analog stick: right" ) )	
	{
		if ( PS3ControllerPad.lX > DEADSPOT )
		{
			return (PS3ControllerPad.lX - DEADSPOT) / ( 10000.0f - DEADSPOT );
		}
	}
	if ( ( keyID.keyname == "right analog stick: up" ) )	
	{
		if ( PS3ControllerPad.lRz < -DEADSPOT )
		{
			return (PS3ControllerPad.lRz + DEADSPOT) / -( 10000.0f - DEADSPOT );
		}
	}
	if ( ( keyID.keyname == "right analog stick: down" ) )	
	{
		if ( PS3ControllerPad.lRz > DEADSPOT )
		{
			return (PS3ControllerPad.lRz - DEADSPOT) / ( 10000.0f - DEADSPOT );
		}
	}
	if ( ( keyID.keyname == "right analog stick: left" ) )	
	{
		if ( PS3ControllerPad.lZ < -DEADSPOT )
		{
			return (PS3ControllerPad.lZ + DEADSPOT) / -( 10000.0f - DEADSPOT );
		}
	}
	if ( ( keyID.keyname == "right analog stick: right" ) )	
	{
		if ( PS3ControllerPad.lZ > DEADSPOT )
		{
			return (PS3ControllerPad.lZ - DEADSPOT) / ( 10000.0f - DEADSPOT );
		}
	}
	return 0.0f;
		
	//if ( ( keyID.keyname == "up" )			&& ( PS3GuitarPad.rgdwPOV[0] == PS3GuitarButtons_Up ) )	
	//{
	//	return 1.0f;
	//}
	//if ( ( keyID.keyname == "down" )		&& ( PS3GuitarPad.rgdwPOV[0] == PS3GuitarButtons_Down ) )	
	//{
	//	return 1.0f;
	//}
	//if ( ( keyID.keyname == "left" )		&& ( PS3GuitarPad.rgdwPOV[0] == PS3GuitarButtons_Left ) )	
	//{
	//	return 1.0f;
	//}
	//if ( ( keyID.keyname == "right" )		&& ( PS3GuitarPad.rgdwPOV[0] == PS3GuitarButtons_Right ) )	
	//{
	//	return 1.0f;
	//}
	//if ( keyID.keyname == "wammy bar" )	
	//{
	//	return (float) ( PS3GuitarPad.lZ / 10000.0f );
	//}
	//if ( keyID.keyname == "slide notes" )	
	//{
	//	return (float) ( ( PS3GuitarPad.lRz + 10000.0f )  / 20000.0f );
	//}
}

bool	PS3Controller::capture()
{
	if ( FAILED( InputDevice->Poll() ) ) 
	{
		while(InputDevice->Acquire() == DIERR_INPUTLOST) 
		{
		}
	}

    if (FAILED(InputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &PS3ControllerPad) ) )
	{
		return false;
	}

	return true;
}
