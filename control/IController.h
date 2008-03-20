//*****************************************************************************
//**
//**   File:               IController.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
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
