#pragma once
#include "IRenderer.h"

class PauseRenderer : public IRenderer
{
public:
	PauseRenderer();
	virtual ~PauseRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

};
