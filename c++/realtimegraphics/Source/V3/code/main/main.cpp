#define VC_EXTRALEAN	// takes out all the crap

#include <windows.h>	// header for windows
#include <time.h>

#include "..\game code\Game.h"
//#define CALLBACK __clrcall



// wndproc function to catch my messages
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
HWND	CreateGameWindow(WNDCLASS &wcex,HINSTANCE &hInstance,const char* caption,const char* classname,const int width,const int height);
void	CreateGameWindow(HINSTANCE &hInstance,int nCmdShow,const char* caption,const char* classname,const int width,const int height);
int		externalGameLoop();
void	checkfps(game &mygame);
// Callback message handler function for our window
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY:
             PostQuitMessage(0);
        break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

HWND	CreateGameWindow(WNDCLASS &wcex,HINSTANCE &hInstance,const char* caption,const char* classname,const int width,const int height)
{
// clear the ramdom memory of the wndclass
	ZeroMemory(&wcex,sizeof(WNDCLASS));
// give the function windows calls with its messages	
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance		= hInstance;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszClassName	= classname;
// register my new window class with the Windows API
	RegisterClass(&wcex);
// Create the window

	return CreateWindow(classname, caption, WS_OVERLAPPEDWINDOW, 0, 0, width, height, NULL, NULL, hInstance, NULL);
}

void	CreateGameWindow(HINSTANCE &hInstance,int nCmdShow,const char* caption,const char* classname,const int width,const int height)
{
	WNDCLASS wcex;
	HWND hWnd =	CreateGameWindow(wcex,hInstance,caption,classname,width,height);
	//show the window
	ShowWindow(hWnd, nCmdShow);
	ShowCursor(0);
	UpdateWindow(hWnd);
} 

int		externalGameLoop()
{
	game bbox;							//create my game object

	if (!bbox.Initalise())
	{
		return -1;
	}

	MSG msg;
	ZeroMemory( &msg, sizeof(msg) );	//message structure memory set to zero
	while( msg.message!=WM_QUIT )		//window loop
	{
		if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			unsigned char gamemsg = bbox.GetMessages();
			if ( (gamemsg & E_Exit) == E_Exit)
			{
				break;
			}
			checkfps(bbox);				//another frame has passed 
			bbox.Update();				//update my game!!!
		}
	}
	return (int)msg.wParam;				//exit program
}

void	checkfps(game &mygame)
{
	static unsigned int fps	= 0;		//frames per second counter
	static long starttimer	= clock();	//timer updated every second
	long endtimer			= clock();	//timer updated every game loop
	if (endtimer - starttimer >= 1000)	//if a second has passed
	{
		mygame.SetFps(fps);				//set the frame rate	
		fps					=	0;		//reset the frame rate counter
		starttimer			=	clock();//reset the timer
	}
	else
	{
		++fps;							//add a frame to frame rate counter
	}
}

// The entry point of my program
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CreateGameWindow(hInstance,nCmdShow,"its working!","MyWindowClass",WIDTH,HEIGHT);		// create my game window
//	CreateGameWindow(hInstance,nCmdShow,"its working2!","MyWindowClass2",WIDTH,HEIGHT);		// create my game window
	return	externalGameLoop();												// begin the game loop
}

