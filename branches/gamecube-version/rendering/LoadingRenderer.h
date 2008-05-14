#pragma once
#include "IRenderer.h"
class Texture;

/**
 * Renders loading text.
 *
 */

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
