//*****************************************************************************
//**
//**   File:               MenuController.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 01:41:43 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 203 $
//**
//*****************************************************************************

#include "MenuController.h"
#include "../rendering/RenderEngine.h"
#include "../rendering/MenuRenderer.h"
#include "ControllerManager.h"

MenuController::MenuController()
{
	m_renderer = dynamic_cast<MenuRenderer*>(RenderEngine::instance().getRenderer("menu"));
}

MenuController :: ~MenuController()
{
}


void MenuController :: activate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	//re.deactivateAllRenderers(); // can do this, or not :D
	re.activateRenderer("menu");
}

void MenuController :: deactivate()
{
	RenderEngine& re = RenderEngine::safeInstance();
	re.deactivateRenderer("menu");
}

void MenuController :: update(float dt)
{
	// check user input here
	// if something in menu needs to change, update data
	// and ask MenuRenderer to change the graphics.
	// if you want to exit the menu back to the game:
	// ControllerManager::instance().activateController("game")
	// GameController is currently hacked to load the TestLevel when it is activated
	// Later on we shall have to have communication between controllers hmm...


	// if you want the game to run: GameLogic::instance().update(dt)
	// if you want the graphics to update: RenderEngine::instance().update(dt)
	// problem: can't have some renderers update and others not ATM
	// this design sucks!
}
