#include "input.h"
#include <dinput.h>
#include <windows.h>
#include "Inputtypes.h"

//////////////////////////////////////////////////////////////////////////
input::input(): m_diObject(NULL) , CurrentJoyStick(NULL), NumberOfControllers(0)	
{
	t.Duration = ( 1.0f / 30.0f );
}

bool input::InitaliseDInput					(														)
{
	hwindow			= FindWindow("MyWindowClass",NULL);
	HINSTANCE hInst = (HINSTANCE) (LONG_PTR) GetWindowLongPtr( hwindow, GWLP_HINSTANCE ); 

	if (FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION,IID_IDirectInput8, (void**)&m_diObject, NULL)))
	{
		return false;
	}

	return true;
}

bool input::InitaliseController				(														)
{
	if ( FAILED( CurrentJoyStick->SetDataFormat( &c_dfDIJoystick2 ) ) )
	{
		return false;
	}

	if ( FAILED( CurrentJoyStick->SetCooperativeLevel( hwindow , DISCL_EXCLUSIVE | DISCL_FOREGROUND ) ) )
	{
		return false;
	}

	DIDEVCAPS capabilities;
	capabilities.dwSize = sizeof(DIDEVCAPS);
	if (FAILED( CurrentJoyStick->GetCapabilities( &capabilities ) ) )
	{
		return false;
	}

	if( FAILED( CurrentJoyStick->EnumObjects( ::EnumObjectsCallback , this , DIDFT_ALL ) ) )
	{
		return false;
	}

	return true;
}


bool input::Add360Controller				(														)
{
	if ( !InitaliseController() )
	{
		return false;
	}

	return true;
}

bool input::AddMouse						(														)
{
	if ( FAILED( CurrentJoyStick->SetDataFormat(&c_dfDIMouse2)))
	{
		return false;
	}

	if ( FAILED( CurrentJoyStick->SetCooperativeLevel( hwindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
	{
		return false;
	}

	if ( FAILED( CurrentJoyStick->Acquire() ) )
	{
		return false;
	}

	return true;
}

bool input::AddKeyboard						(														)
{
	if (FAILED(CurrentJoyStick->SetDataFormat( &c_dfDIKeyboard )))
	{
		return false;
	}

	if ( FAILED( CurrentJoyStick->SetCooperativeLevel( hwindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE ) ) )
	{
		return false;
	}

	if (FAILED(CurrentJoyStick->Acquire()))
	{
		return false;
	}

	return true;
}

bool input::AddPS3Guitar					(														)
{
	if ( !InitaliseController() )
	{
		return false;
	}

	return true;
}

bool input::AddPS3Drums						(														)
{
	if ( !InitaliseController() )
	{
		return false;
	}

	return true;
}

bool input::AddPS3Controller				(														)
{
	if ( !InitaliseController() )
	{
		return false;
	}

	return true;
}

bool input::FindValidControllers			(														)
{
	if ( FAILED( m_diObject->EnumDevices( DI8DEVCLASS_ALL , ::EnumCallback , (LPVOID)this , DIEDFL_ATTACHEDONLY ) ) )
	{
		return false;
	}

	if ( CurrentJoyStick == NULL )
	{
		return false;
	}

	for (unsigned int a = 0; a < InputDevices.size(); a++)
	{
		CurrentJoyStick = InputDevices[a];
		controller* add = NULL;
		ZeroMemory(&CurrentJoyStickDevice, sizeof(CurrentJoyStickDevice));
		CurrentJoyStickDevice.dwSize = sizeof(CurrentJoyStickDevice);
		if (FAILED(CurrentJoyStick->GetDeviceInfo(&CurrentJoyStickDevice))) 
		{
			return false;
		}
		std::string ControllerName = CurrentJoyStickDevice.tszProductName;
		if ( ControllerName.compare( "XBOX 360 For Windows (Controller)" ) == 0 ||
			 ControllerName.compare( "Controller (XBOX 360 For Windows)" ) == 0 )
		{
			Add360Controller();
			add = new xboxcontroller();
			add->initialize( CurrentJoyStick );
			controllers.push_back(add);
		}
		else if ( ControllerName.compare( "Keyboard" ) == 0 )
		{
			AddKeyboard();
			add = new keyboard();
			add->initialize( CurrentJoyStick );
			controllers.push_back(add);
		}
		else if ( ControllerName.compare( "Mouse" ) == 0 )
		{
			AddMouse();
			add = new mouse();
			add->initialize( CurrentJoyStick );
			controllers.push_back(add);
		}
		else if ( ControllerName.compare( "Guitar Hero3 for PlayStation (R) 3" ) == 0 )
		{
			AddPS3Guitar();
			add = new PS3Guitar();
			add->initialize( CurrentJoyStick );
			controllers.push_back(add);
		}
		else if ( ControllerName.compare( "Guitar Hero4 for PlayStation (R) 3" ) == 0 )
		{
			AddPS3Drums();
			add = new PS3Drums();
			add->initialize( CurrentJoyStick );
			controllers.push_back(add);
		}
		else if ( ControllerName.compare( "DUALSHOCK3 Controller HID Minidriver (USB) Beta" ) == 0 )
		{
			AddPS3Controller();
			add = new PS3Controller();
			add->initialize( CurrentJoyStick );
			controllers.push_back(add);
		}
		else
		{
			InputDevices[a]->Release();
			InputDevices[a] = NULL;
			InputDevices.erase(InputDevices.begin()+a);
			--a;
		}
	}

	NumberOfControllers = (unsigned int) InputDevices.size();
	return true;
}


bool input::Initalise()
{
	if ( !InitaliseDInput() )
	{
		return false;
	}

	return FindValidControllers();
}
//////////////////////////////////////////////////////////////////////////
// Dirty messy Direct Input stuff
BOOL CALLBACK EnumCallback					( const DIDEVICEINSTANCE* instance, VOID* context		)
{
    if (context != NULL) {
        return ((input *)context)->EnumCallback(instance, context);
    } else {
        return DIENUM_STOP;
    }
}

BOOL CALLBACK input::EnumCallback			( const DIDEVICEINSTANCE* instance, VOID* context		)
{
    // Obtain an interface to the enumerated joystick.
    HRESULT hr = m_diObject->CreateDevice(instance->guidInstance, &CurrentJoyStick, NULL);
	if (FAILED(hr)) 
	{ 
		return DIENUM_STOP;
	}

	InputDevices.push_back(CurrentJoyStick);
    return DIENUM_CONTINUE;
}

BOOL CALLBACK EnumObjectsCallback			( const DIDEVICEOBJECTINSTANCE* pdidoi , VOID* pContext	)
{
	if (pContext != NULL) 
	{
		return ((input *)pContext)->EnumObjectsCallback(pdidoi, pContext);
	} 
	else 
	{
		return DIENUM_STOP;
	}
}

BOOL CALLBACK input::EnumObjectsCallback	( const DIDEVICEOBJECTINSTANCE* pdidoi , VOID* pContext	)
{
	if( pdidoi->dwType & DIDFT_AXIS )
	{
		DIPROPRANGE diprg;
		diprg.diph.dwSize		= sizeof( DIPROPRANGE );
		diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );
		diprg.diph.dwHow		= DIPH_BYID;
		diprg.diph.dwObj		= pdidoi->dwType; // Specify the enumerated axis
		diprg.lMin = -10000;
		diprg.lMax = +10000;

		// Set the range for the axis
		if( FAILED( CurrentJoyStick->SetProperty( DIPROP_RANGE, &diprg.diph ) ) )
		{
			return DIENUM_STOP;
		}
	}

	return DIENUM_CONTINUE;
}
//////////////////////////////////////////////////////////////////////////
input::~input								(														)
{
	for (unsigned int a=0; a<controllers.size(); ++a)
	{
		delete controllers[a];
		controllers[a]	=	NULL;
	}

	for (unsigned int a=0; a<InputDevices.size(); ++a)
	{
		InputDevices[a]->Release();
		InputDevices[a]	=	NULL;
	}

	if (m_diObject)
	{
		m_diObject->Release();
	}
}
//////////////////////////////////////////////////////////////////////////
void input::Update							(														)
{
	if ( t.Check() )
	{
		refreshall();
	}
}

void input::refreshall						(														)
{
	for (unsigned int a=0; a<controllers.size(); ++a)
	{
		controllers[a]->capture();
	}

}

void input::refreshdevice					( unsigned int inputdevice								)
{
	if (inputdevice<controllers.size())
	{
		controllers[inputdevice]->capture();
	}
}

float input::getvalue						( action& button , unsigned int * whopressedit			)
{
	float value = 0.0f;
	for (unsigned int a=0; a<controllers.size(); a++)
	{		
		float currentvalue = controllers[a]->getpress(button);
		if (currentvalue < 0.0f)
		{
			currentvalue = 0.0f;
		}
		value += currentvalue;
		if ( currentvalue && whopressedit)
		{
			*whopressedit |= ( 1 << ( a ) );
		}
	}
	return value;
}