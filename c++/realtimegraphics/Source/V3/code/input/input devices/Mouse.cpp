#include "Mouse.h"
#include <math.h>

mouse::mouse()
{
	ZeroMemory( &info, sizeof(info) );
}

mouse::~mouse()
{
	if (InputDevice)
	{
		InputDevice->Unacquire();
		InputDevice->Release();
	}
}

void mouse::initialize(const LPDIRECTINPUTDEVICE8 inputdevice)
{
	InputDevice = inputdevice;
}

float	mouse::getpress(const action& keyID)
{
	if (keyID.keyname=="up")
	{
		if (info.y < 0.0f)
		{
			return fabs(info.y);
		}
		else
		{
			return 0.0f;
		}
	}
	else if (keyID.keyname=="down")
	{
		if (info.y > 0.0f)
		{
			return fabs(info.y);
		}
		else
		{
			return 0.0f;
		}
	}
	if (keyID.keyname=="left")
	{
		if (info.x < 0.0f)
		{
			return fabs(info.x);
		}
		else
		{
			return 0.0f;
		}
	}
	else if (keyID.keyname=="right")
	{
		if (info.x > 0.0f)
		{
			return fabs(info.x);
		}
		else
		{
			return 0.0f;
		}
	}

	return 0.0f;
}

bool	mouse::capture()
{
	DIMOUSESTATE2			m_mouseState;

	ZeroMemory( &m_mouseState, sizeof(m_mouseState) );
	ZeroMemory( &info, sizeof(info) );
	HRESULT hr;
	if (FAILED(InputDevice->GetDeviceState( sizeof(m_mouseState), &m_mouseState )))
	{
		hr=InputDevice->Acquire();
		while( hr == DIERR_INPUTLOST )
		{     
			hr = InputDevice->Acquire();
		}
		if (FAILED(hr))
		{
			return false;
		}
	}

	info.x		= ((float)m_mouseState.lX)*0.010f;
	info.y		= ((float)m_mouseState.lY)*0.010f;
	info.z		= 0.0f;
	_memccpy(info.buttons,m_mouseState.rgbButtons,8,sizeof(unsigned char));

	return true;
}
