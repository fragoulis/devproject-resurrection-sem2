#include "PS_Base.h"
#include "../gfx/Shaders/ShaderManager.h"

PS_Base :: PS_Base(const std::string& name,
			const Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const int shindex)			//Creates the ps with it's default parameters
:m_nameId(name),
m_quadArray(model),
m_particleSize(psize),
m_systemLife(syslife),
m_particleLife(plife),
m_shaderIndex(shindex)
{
	// TODO : constant parameters/uniforms need to be bound only once! So it can very well be done here.
}

void PS_Base :: render(const float delta)
{
	m_currentTime += delta;
}

void PS_Base :: reset()
{
	m_currentTime = 0.0f;
}

void PS_Base :: setTransform(const CoordinateFrame& transform)
{
	m_transform = transform;
}