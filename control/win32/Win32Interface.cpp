#include "Win32Interface.h"
#include <windows.h>

Win32Interface :: Win32Interface()
{
}

Win32Interface :: ~Win32Interface()
{
}

void Win32Interface :: quit()
{
	PostQuitMessage(0);
}