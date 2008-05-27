#ifndef _RES_LEVEL_COMPLETE_RENDERER_H_
#define _RES_LEVEL_COMPLETE_RENDERER_H_
#include "IRenderer.h"
class Texture;


/**
 * Renders Level complete text
 *
 */
class LevelCompleteRenderer : public IRenderer
{
public:
	LevelCompleteRenderer();
	virtual ~LevelCompleteRenderer();


	virtual void render(Graphics& g) const;
	virtual void update(float dt);

private:
	Texture* m_texture;

};


#endif