//*****************************************************************************
//**
//**   File:               LoadingRenderer.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"
class Texture;

class LoadingRenderer : public IRenderer
{
public:
	LoadingRenderer();
	virtual ~LoadingRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

private:
	Texture* m_texture;
};
