#ifndef _RES_IRENDERER_H_
#define _RES_IRENDERER_H_
class Graphics;


/**
 * Interface for renderers
 *
 */
class IRenderer
{
public:
	virtual ~IRenderer(){}
	virtual void render(Graphics& g) const = 0;
	virtual void update(float dt) = 0;
};


#endif
