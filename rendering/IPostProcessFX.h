#pragma once


class Texture;
class FramebufferObject;

/*
	Base class for post - process FX

	Watch : 
	fbo will have nothing attached, & upon return , also everything should be unattached
	Viewport by default is the full one, projection is ortho from 0 to 1
*/

class IPostProcessFX
{
public :
	virtual ~IPostProcessFX(){}
	virtual Texture * process(Texture * input,Texture * output, FramebufferObject& fbo,const float additive_time) const = 0;
};