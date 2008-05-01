#pragma once

#include "IPostProcessFX.h"

/*
	screen noise grain effect
*/

class GrainPostProc : public IPostProcessFX
{
	Texture * m_noise;
	Texture * m_rand;

public :

	GrainPostProc();
	~GrainPostProc();
	Texture * process(Texture * input, Texture * output,FramebufferObject& fbo,const float additive_time) const;
};