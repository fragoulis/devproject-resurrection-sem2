
/**
 * Provides the main function
 * Sets up a window and calls methods on Application, namely:
 * init, destroy, update and render
 *
 */


//#define _CRTDBG_MAP_ALLOC
//#define _CRTDBG_MAP_ALLOC
//#include <cstdlib>
//#include <crtdbg.h>

//#include <vld.h>
#include <windows.h>
#include "Window.h"
#include "Win32Interface.h"
#include "../OSinterface/Application.h"
#include "../../rendering/Graphics.h"
#include "../../utility/Time.h"
#include "../../gfxutils/confparser/confparser.h"
#include <string>
#pragma warning( disable : 4996 )

#pragma warning( push )
#pragma warning( disable : 4005 )
#define _CRT_SECURE_NO_DEPRECATE
#pragma warning( pop )

#include <stdio.h>

using namespace std;

#ifndef ZeroMemory
	//for compilers that dont have ZeroMemory define it as memset
//#define ZeroMemory(a,b) (memset((a),0,(b)))
#endif

/**
 * Contains WinMain and main functions
 * Creates a window and runs the main loop.
 * Sets up and interfaces with Application object and Win32Interface
 */
int WINAPI WinMain(	HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	MSG winMsg;	//the window's message struct
	ZeroMemory(&winMsg,sizeof(MSG));	//fill with 0's

	// Create some variables
	Window GLWindow;
	Application Application;
	Time timer;
	Win32Interface Win32Interface;

	IOSInterface::setInstance(&Win32Interface);

	int scrw = 1280,scrh = 1024;
	bool fullscreen = false;

	ConfParser* cp = new ConfParser("./config/config.txt");
	if(cp->getSection("Screen"))
	{
		string hres = cp->getSection("Screen")->getVal("HorzRes"),
				vres = cp->getSection("Screen")->getVal("VerticalRes"),
				isfs = cp->getSection("Screen")->getVal("Fullscreen");
		if(hres != "")
			scrw = atoi(hres.c_str());
		if(vres != "")
			scrh = atoi(vres.c_str());
		if(isfs == "1")
			fullscreen = true;
	}
	delete cp;


	bool inLoop = true;

	//create the window
	if (!GLWindow.createWindow("Resurrection", scrw,scrh, fullscreen))
	{
		//if it fails, tell the user and exit
		MessageBox (NULL, "Error making window", "OpenGL", MB_OK);
		GLWindow.destroyWindow();
		return 0;
	}


	if (!Application.init()) return false;

	timer.reset();

	while (inLoop)
	{
		//see if there are any windows messages, and if so process and
		//remove them
		while (PeekMessage(&winMsg, NULL, 0, 0, PM_REMOVE))
		{
			switch (winMsg.message)
			{
				case WM_QUIT:	//quit if windows wants to quit
					inLoop = false;
					break;

				default:
					TranslateMessage(&winMsg);
					DispatchMessage(&winMsg);
					break;
			}
		}
        const float dt = (float)timer.getDeltatime();
		Application.update(dt); // bleh

		// Can add window data to g to allow things like printing text
		Graphics g;
		Application.render(g);

		SwapBuffers(GLWindow.getDrawingContext());
	}

	Application.destroy();
	GLWindow.destroyWindow();

	return 0;
}



//extern "C"
//{
//int PASCAL WinMain(HINSTANCE inst,HINSTANCE dumb,LPSTR param,int show);
//};

int main(int ac,char *av[])
{
    char buf[256];
    int i;
    HINSTANCE inst;

    inst=(HINSTANCE)GetModuleHandle(NULL);

    buf[0]=0;
    for(i=1; i<ac; i++)
    {
        strcat_s(buf,av[i]);
        strcat(buf," ");
    }

    return WinMain(inst,NULL,buf,SW_SHOWNORMAL);
}