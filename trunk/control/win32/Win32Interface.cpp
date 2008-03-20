//*****************************************************************************
//**
//**   File:               Win32Interface.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


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