//*****************************************************************************
//**
//**   File:               LoadingController.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#include "LoadingController.h"
#include "../rendering/RenderEngine.h"

LoadingController :: LoadingController()
{
}

LoadingController :: ~LoadingController()
{
}


void LoadingController :: activate()
{
	// deactivate all user input, activate loading renderer
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateAllRenderers();
	re.activateRenderer("loading");
}

void LoadingController :: deactivate()
{
	RenderEngine& re = RenderEngine::instance();
	re.deactivateAllRenderers();
}

void LoadingController :: update(float dt)
{
	// animate the loading screen!
	// maybe do that in the renderer itself
	// that means IRenderer needs an update method
}
