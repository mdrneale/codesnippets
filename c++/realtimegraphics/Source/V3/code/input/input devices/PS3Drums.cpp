#include "PS3Drums.h"

#define DEADSPOT 3500

PS3Drums::PS3Drums()
{
	ZeroMemory( &PS3DrumsPad, sizeof(DIJOYSTATE2) );
}

PS3Drums::~PS3Drums()
{

}

void PS3Drums::initialize(const LPDIRECTINPUTDEVICE8 inputdevice)
{
	InputDevice	=	inputdevice;
}

float	PS3Drums::getpress(const action& keyID)
{
	if ( ( keyID.keyname == "square" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Square]		== PS3DrumButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "circle" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Circle]		== PS3DrumButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "triangle" )	&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Triangle]	== PS3DrumButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "cross" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Cross]		== PS3DrumButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "green" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Green]		== PS3DrumButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "red" )			&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Red]		== PS3DrumButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "yellow" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Yellow]		== PS3DrumButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "blue" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Blue]		== PS3DrumButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "orange" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Orange]		== PS3DrumButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "foot peddle" )	&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_FootPeddle]	== PS3DrumButton_IsPressed ) )
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "select" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Select]		== PS3DrumButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "start" )		&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_Start]		== PS3DrumButton_IsPressed ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "ps button" )	&& ( PS3DrumsPad.rgbButtons[PS3DrumsButtons_PSButton]	== PS3DrumButton_IsPressed ) )	
	{
		return 1.0f;
	}
		
	if ( ( keyID.keyname == "up" )			&& ( PS3DrumsPad.rgdwPOV[0] == PS3DrumsButtons_Up ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "down" )		&& ( PS3DrumsPad.rgdwPOV[0] == PS3DrumsButtons_Down ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "left" )		&& ( PS3DrumsPad.rgdwPOV[0] == PS3DrumsButtons_Left ) )	
	{
		return 1.0f;
	}
	if ( ( keyID.keyname == "right" )		&& ( PS3DrumsPad.rgdwPOV[0] == PS3DrumsButtons_Right ) )	
	{
		return 1.0f;
	}

	return 0.0f;
}

bool	PS3Drums::capture()
{
	if ( FAILED( InputDevice->Poll() ) ) 
	{
		while(InputDevice->Acquire() == DIERR_INPUTLOST) 
		{
		}
	}

    if (FAILED(InputDevice->GetDeviceState(sizeof(DIJOYSTATE2), &PS3DrumsPad) ) )
	{
		return false;
	}

	return true;
}
