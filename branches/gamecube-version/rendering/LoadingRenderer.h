#ifndef _RES_LOADING_RENDERER_H_
#define _RES_LOADING_RENDERER_H_
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
	//Texture* m_texture;
};


#endif