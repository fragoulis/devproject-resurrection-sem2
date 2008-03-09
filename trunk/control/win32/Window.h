//*****************************************************************************
//**
//**   File:               Window.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once
#include <windows.h>

/**
 * Basic window class.
 * Does not really do anything special.
 * What else is there to say?
 */
class Window
{
public:


	//makes a window, returning it's rendering context on success,
	//and throwing an exception on failure
	bool createWindow(const char* title, int width, int height, bool fullscreen);
	void destroyWindow();

	const HDC& getDrawingContext() const { return hDc; };

	//~Window() { destroyWindow(); }//destory window on class temination

private:
	bool fullscreen;
	HINSTANCE hinstance;	//an instance for the window
	HWND hWnd;				//window handle
	HDC hDc;				//the drawing context for the window
	HGLRC hRc;				//rendering context
};
