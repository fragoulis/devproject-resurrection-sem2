#ifndef _RES_GAME_OVER_RENDERER_H_
#define _RES_GAME_OVER_RENDERER_H_
#include "IRenderer.h"
class Texture;

/**
 * Renders the game over text
 *
 */
class GameOverRenderer : public IRenderer
{
public:
	GameOverRenderer();
	virtual ~GameOverRenderer();

	virtual void render(Graphics& g) const;
	virtual void update(float dt);

private:
	Texture* m_texture;

};


#endif