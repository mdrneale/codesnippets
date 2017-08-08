#include "keyboard.h"

keyboard::keyboard()
{
	ZeroMemory(keys, sizeof(keys) );
}

keyboard::~keyboard()
{
	if (InputDevice)
	{
		InputDevice->Unacquire();
		InputDevice->Release();
	}
}

//void keyboard::initialize()
//{
	//HWND hwindow	= FindWindow("MyWindowClass",NULL);
	//HINSTANCE hInst = (HINSTANCE) (LONG_PTR) GetWindowLongPtr( hwindow, GWLP_HINSTANCE ); 
	//DirectInput8Create(hInst, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&m_diObject, NULL);
	//initialize(m_diObject);
//}

void keyboard::initialize(const LPDIRECTINPUTDEVICE8 inputdevice)
{
	InputDevice = inputdevice;
}

float	keyboard::getpress(const action& keyID)
{
//	if ( keyID.actionfor == e_keyboard )
	{
		if (keyID.isascii)
		{
			if (keys[keyID.keyname2] & 0x80 )
			{
				return 1.0f;
			}
			else
			{
				return 0.0f;
			}
		}
		else
		{
			if (keyID.keyname=="up")
			{
				if (keys[DIK_UP] & 0x80)
				{
					return 1.0f;
				}
				else
				{
					return -0.0f;
				}
			}
			else if (keyID.keyname=="down")
			{
				if (keys[DIK_DOWN] & 0x80)
				{
					return 1.0f;
				}
				else
				{
					return 0.0f;
				}
			}
			else if (keyID.keyname=="left")
			{
				if (keys[DIK_LEFT] & 0x80)
				{
					return 1.0f;
				}
				else
				{
					return 0.0f;
				}
			}
			else if (keyID.keyname=="right")
			{
				if (keys[DIK_RIGHT] & 0x80)
				{
					return 1.0f;
				}
				else
				{
					return 0.0f;
				}
			}
			else if (keyID.keyname=="escape")
			{
				if (keys[DIK_ESCAPE] & 0x80)
				{
					return 1.0f;
				}
				else
				{
					return 0.0f;
				}
			}
			else if (keyID.keyname=="space")
			{
				if (keys[DIK_SPACE] & 0x80)
				{
					return 1.0f;
				}
				else
				{
					return 0.0f;
				}
			}
		}
		return -100.0f;
	}
	return 0.0f;
}

bool	keyboard::capture()
{
	ZeroMemory(keys, sizeof(keys) );
	if (FAILED(InputDevice->GetDeviceState( sizeof(keys), keys )))
	{
		// If input is lost then acquire and keep trying until we get it back
		HRESULT hr	=	InputDevice->Acquire();
		while( hr == DIERR_INPUTLOST )
		{     
			hr = InputDevice->Acquire();
		}
		// Could be we failed for some other reason
		if (FAILED(hr))
		{
			return false;
		}
	}
	return true;
}
