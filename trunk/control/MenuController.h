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
class MenuRenderer;

class MenuController : public IController
{
public:
	MenuController();
	virtual ~MenuController();


	virtual void activate();
	virtual void deactivate();
	virtual void update(float dt);

private:
	MenuRenderer* m_renderer;
	// Menu data
};
