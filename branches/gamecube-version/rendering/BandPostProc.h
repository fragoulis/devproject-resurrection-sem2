#pragma once

#include "IPostProcessFX.h"

class BandPostProc : public IPostProcessFX
{
public :

	BandPostProc();
	~BandPostProc();
	
	virtual Texture * process(Texture * input,Texture * output, FramebufferObject& fbo,const float additive_time) const;

};