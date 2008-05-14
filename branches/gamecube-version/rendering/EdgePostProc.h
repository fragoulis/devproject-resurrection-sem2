#pragma once

#include "IPostProcessFX.h"

/*
	screen noise grain effect
*/

class EdgePostProc : public IPostProcessFX
{
	Texture * m_depth,
			* m_edges;

public :

	EdgePostProc(Texture * depth_buffer);
	~EdgePostProc();
	Texture * process(Texture * input, Texture * output,FramebufferObject& fbo,const float additive_time) const;
};