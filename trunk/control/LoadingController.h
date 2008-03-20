//*****************************************************************************
//**
//**   File:               LoadingController.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************


#pragma once
#include "IController.h"

/**
 * Shows a loading screen, disables all user input.
 *
 * TODO: add some callback function to do some actual loading?
 */
class LoadingController : public IController
{
public:
	LoadingController();
	virtual ~LoadingController();

	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:
};
