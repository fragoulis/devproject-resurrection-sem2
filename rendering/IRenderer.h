//*****************************************************************************
//**
//**   File:               IRenderer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
class Graphics;


class IRenderer
{
public:
	virtual ~IRenderer(){}
	virtual void render(Graphics& g) const = 0;
	virtual void update(float dt) = 0;
};
