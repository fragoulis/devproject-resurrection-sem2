//*****************************************************************************
//**
//**   File:               MenuRenderer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"

class MenuRenderer : public IRenderer
{
public:
	MenuRenderer();
	virtual ~MenuRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

	// add functions to change menu here
	// like "addMenuOption(string name, point2 location, float width, float height"
	// and "highlightMenuOption(int id)
	// you figure it out!
};
