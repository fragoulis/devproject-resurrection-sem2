#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
//#include <gl\glext.h>
#include "Window.h"
#include "../OSinterface/Input.h"

#pragma comment(lib, "opengl32.lib")

//window class name for.. windows
#define WINDOW_CLASS "Resurrection"

#ifndef ZeroMemory
	//for compilers that dont have ZeroMemory define it as memset
#define ZeroMemory(src,size) (memset((a),0,(b)))
#endif


//some compilers dont define CDS_FULLSCREEN

#ifndef CDS_FULLSCREEN
#define CDS_FULLSCREEN 4
#endif


bool active;		//if the window is active, and should be draw to
//bool keys[256];		//array of keys

//declearation for wndproc, ill get onto this later
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


bool Window::createWindow(const char* title,int width,int height,bool fullscreen)
{
	// make sure input singleton exists
	Input::safeInstance();

	Window::fullscreen = fullscreen;	//set the window's fullscreen flag
	Window::hinstance = GetModuleHandle(NULL);	//get an instance for the window

	WNDCLASS wc;	//descriptor for the window class


	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		// Redraw On Move, And Own DC For Window
	wc.lpfnWndProc		= (WNDPROC) WndProc;						// WndProc Handles  windows Messages
	wc.cbClsExtra		= 0;										// No Extra Window Data
	wc.cbWndExtra		= 0;										// No Extra Window Data
	wc.hInstance		= Window::hinstance;						// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);				// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);				// Load The Arrow Pointer
	wc.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);;		// Black Background
	wc.lpszMenuName		= NULL;										// We Don't Want A Menu
	wc.lpszClassName	= WINDOW_CLASS;								// Set The Class Name


	// register the window class, if anything gos worng, return false
	if (!RegisterClass(&wc))
		return false;
	// if the game is in fullscreen, change the screen resoulution to match
	if (fullscreen)
	{
		DEVMODE screenmode;	//a device mode for the screen
		ZeroMemory(&screenmode,sizeof(DEVMODE));
		screenmode.dmSize = sizeof(DEVMODE);

		screenmode.dmBitsPerPel	=	32;		// 32 bits per pixel
		screenmode.dmPelsWidth	=	width;	// width of the window
		screenmode.dmPelsHeight	=	height;	// and the height

		screenmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
		// what fields were used

		// if that screen mode is not available exit
		if (ChangeDisplaySettings(&screenmode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			return false;

		Window::fullscreen=fullscreen;	//re-do the class' fullscreen flag
	}


	// set window-modes for fullscreen/windowed and such

	DWORD ExStyle,Style;	//window extended style and style
	

	if (fullscreen)
	{
		ExStyle = WS_EX_APPWINDOW;	//create a normal window
		Style = WS_POPUP;			// sets the window to be on-top and have no borders/title bar etc 

		ShowCursor(false);	//its better to hide the cursor for fullscreen
	}
	else
	{
		ExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;	//normal window which has an edge on it
		Style = WS_CAPTION | WS_SYSMENU;	//a normal window's window
	}

	RECT windowrect;	//a rectangle that holds the size of the window
	windowrect.top		= 0;
	windowrect.bottom	= (long) height;
	windowrect.left		= 0;
	windowrect.right	= (long) width;

	//this function adjust's the size of out window,
	//so that the intrior of the window is what we want it to be,
	//not the entire window

	if (Window::fullscreen) AdjustWindowRectEx(&windowrect,Style,false,ExStyle);


	//this creates the window
	if (!(Window::hWnd=CreateWindowEx(ExStyle,WINDOW_CLASS,title,Style,	//the extended style, window class name, title bar and style for the window

										windowrect.left,windowrect.top,	//window's left and top position
										windowrect.right-windowrect.left,	//window's width
										windowrect.bottom-windowrect.top,	//window's height

										NULL,NULL,Window::hinstance,NULL)))	//parent window, menu and hinstance for window
		return false;	//if creation fails, return false


	//get the hDc, or rendering context, for the window
	if (!(Window::hDc = GetDC(Window::hWnd)))
		return false;










	//if it's all done, move on to OpenGL start up

	static PIXELFORMATDESCRIPTOR pixelformat;
	ZeroMemory(&pixelformat,sizeof(PIXELFORMATDESCRIPTOR));
	// this describes how to set up the surface for use with openGL,
	// it is declared static as openGL may need it after this function is complete

	pixelformat.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelformat.nVersion = 1;	//the version of the data struct, should always be set to 1
	
	pixelformat.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
	//say we want to draw to a window, have double buffering and support openGL

	pixelformat.iPixelType = PFD_TYPE_RGBA;
	//determins how the pixels are stored in memory

	pixelformat.cColorBits = 32;	// 32 bpp
	pixelformat.cAlphaBits = 8;		// 8 bits for transparency
	pixelformat.cDepthBits = 24;	// a 24 bit depth buffer
	pixelformat.iLayerType = PFD_MAIN_PLANE;

	GLuint pxl; //for holding pixel format
	
	// check for a matching pixel format
	if (!(pxl = ChoosePixelFormat(Window::hDc,&pixelformat)))
		return false;

	// set the pixel format for the window
	if (!SetPixelFormat(Window::hDc,pxl,&pixelformat))
		return false;


	// create a opengl rendering context
	if (!(Window::hRc = wglCreateContext(Window::hDc)))
		return false;

	// and set it as the current context
	if (!wglMakeCurrent(Window::hDc,Window::hRc))
		return false;



	ShowWindow(Window::hWnd,SW_SHOW);		// Show The Window
	SetForegroundWindow(Window::hWnd);
	SetFocus(Window::hWnd);					// Sets Keyboard Focus To The Window

	//if it all went successfully, return true
	return true;

}


void Window::destroyWindow()
{
	//un-registers everything made with the window

	//destroy fullscreen if made
	if (Window::fullscreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(true);
	}

	//release hRc
	if (Window::hRc)
	{
        wglMakeCurrent(NULL,NULL);		// unslect as current context
		wglDeleteContext(Window::hRc);	// delete context
		Window::hRc = NULL;				// make sure hRc becomes nothing
	}

	//release hDc if made
	if (Window::hDc)
	{
		ReleaseDC(Window::hWnd,Window::hDc);
		Window::hDc = NULL;
	}

	//destroy window
	if (Window::hWnd)
	{
		DestroyWindow(Window::hWnd);
		Window::hWnd = NULL;
	}

	UnregisterClass(WINDOW_CLASS, Window::hinstance);

	Input::destroy();
}

LRESULT	CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_ACTIVATE:
		//window activation message, the hiword of wparam is true of false
		//depending on it's state
		active = (!HIWORD(wParam));
		return 0;

	case WM_CLOSE:
		//windows wants to close the window, tell the main loop to quit
		PostQuitMessage(0);
		return 0;

	case WM_KEYDOWN:
		//a key has been pressed down, the key is the wParam
		//keys[wParam] = true;
		Input::instance().onKeyDown(int(wParam));
		return 0;

	case WM_KEYUP:
		//the opposite of keydown
		//keys[wParam] = false;
		Input::instance().onKeyUp(int(wParam));
		return 0;

	case WM_MOUSEMOVE :
		Input::instance().onMouseMove(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_LBUTTONDOWN :
		Input::instance().onMouseDown(1);
		return 0;

	case WM_LBUTTONUP :
		Input::instance().onMouseUp(1);
		return 0;

	case WM_RBUTTONDOWN :
		Input::instance().onMouseDown(2);
		return 0;

	case WM_RBUTTONUP :
		Input::instance().onMouseUp(2);
		return 0;

	}

	// ignore all unhandled message
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}