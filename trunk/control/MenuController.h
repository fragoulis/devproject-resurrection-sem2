//*****************************************************************************
//**
//**   File:               MenuController.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-20 01:41:43 +0000 (Thu, 20 Mar 2008) $
//**   Last Revision:      $Revision: 203 $
//**
//*****************************************************************************

#pragma once
#include "IController.h"
#include "../utility/Singleton.h"
class MenuRenderer;




/**
 * Sets up the main menu and handles user input for it
 *
 */
class MenuController :
	public IController,
	public Singleton< MenuController >
{
public:
	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);


	// Helper to load menu
	void load();
	void loadPlanetSelection();


private:
	MenuRenderer* m_renderer;
	// Menu data
    
    // ambient sound id
    unsigned m_ambientSound;

	void _newGame();
	void _exit();

	// Callback for LoadingController
	// New game simply loads the very first level
	void _loadFirstLevel();


	// Singleton stuffz
	friend Singleton< MenuController >;
	MenuController();
	virtual ~MenuController();
};