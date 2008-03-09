//*****************************************************************************
//**
//**   File:               IRenderer.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
class Graphics;


class IRenderer
{
public:
	virtual void render(Graphics& g) const = 0;
	virtual void update(float dt) = 0;
};