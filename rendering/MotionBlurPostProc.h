#pragma once

#include "IPostProcessFX.h"

#define MB_STORED 20

class Texture;
class MotionBlurPostProc : public IPostProcessFX
{
	Texture * m_stored[MB_STORED];
	unsigned m_filled;
	unsigned m_index;

public :

	MotionBlurPostProc();
	~MotionBlurPostProc();

	Texture * process(Texture * input,Texture * output, FramebufferObject& fbo,const float additive_time) const;

};