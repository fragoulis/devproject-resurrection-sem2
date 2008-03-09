//*****************************************************************************
//**
//**   File:               LoadingController.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
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
