#pragma once
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
