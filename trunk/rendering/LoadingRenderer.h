//*****************************************************************************
//**
//**   File:               LoadingRenderer.h
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#pragma once
#include "IRenderer.h"

class LoadingRenderer : public IRenderer
{
public:
	LoadingRenderer();
	virtual ~LoadingRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);
};
