#include "PS3Guitar.h"

PS3Guitar::PS3Guitar()
{
	ZeroMemory( &PS3GuitarPad, sizeof(DIJOYSTATE2) );
}

PS3Guitar::~PS3Guitar()
{

}

void PS3Guitar::initialize(const LPDIRECTINPUTDEVICE8 inputdevice)
{
	InputDevice	=	inputdevice;
}

float	PS3Guitar::getpress(const action& keyID)
{
	if ( ( keyID.keyname == "green" )	&& ( PS3GuitarPad.rgbButtons[PS3GuitarButtons_Green]	== PS3GuitarButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "red" )		&& ( PS3GuitarPad.rgbButtons[PS3GuitarButtons_Red]		== PS3GuitarButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "yellow" )	&& ( PS3GuitarPad.rgbButtons[PS3GuitarButtons_Yellow]	== PS3GuitarButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "blue" )	&& ( PS3GuitarPad.rgbButtons[PS3GuitarButtons_Blue]		== PS3GuitarButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "orange" )	&& ( PS3GuitarPad.rgbButtons[PS3GuitarButtons_Orange]	== PS3GuitarButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "select" )	&& ( PS3GuitarPad.rgbButtons[PS3GuitarButtons_Select]	== PS3GuitarButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "start" )	&& ( PS3GuitarPad.rgbButtons[PS3GuitarButtons_Start]	== PS3GuitarButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "ps button" )	&& ( PS3GuitarPad.rgbButtons[PS3GuitarButtons_PSButton]	== PS3GuitarButton_IsPressed ) )	
	{
		return 1.0f;
	}
		
	if ( ( keyID.keyname == "up" )			&& ( PS3GuitarPad.rgdwPOV[0] == PS3GuitarButtons_Up ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "down" )		&& ( PS3GuitarPad.rgdwPOV[0] == PS3GuitarButtons_Down ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "left" )		&& ( PS3GuitarPad.rgdwPOV[0] == PS3GuitarButtons_Left ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "right" )		&& ( PS3GuitarPad.rgdwPOV[0] == PS3GuitarButtons_Right ) )	
	{
		return 1.0f;
	}
	if ( keyID.keyname == "wammy bar" )	
	{
		return (float) ( PS3GuitarPad.lZ / 10000.0f );
	}
	if ( keyID.keyname == "slide notes" )	
	{
		return (float) ( ( PS3GuitarPad.lRz + 10000.0f )  / 20000.0f );
	}
	

	return 0.0f;
}

bool	PS3Guitar::capture()
{
	if ( FAILED( InputDevice->Poll() ) ) 
	{
		while(InputDevice->Acquire() == DIERR_INPUTLOST) 
		{
		}
	}

    if (FAILED(InputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &PS3GuitarPad) ) )
	{
		return false;
	}

	return true;
}
