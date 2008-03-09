//*****************************************************************************
//**
//**   File:               IController.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************


#pragma once


/**
 * Interface for controllers
 */
class IController
{
public:
	virtual void activate() = 0;
	virtual void deactivate() = 0;
	virtual void update(float dt) = 0;
};