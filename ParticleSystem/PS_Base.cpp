#include "PS_Base.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/Model.h"

PS_Base :: PS_Base(const std::string& name,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const bool deleteModelOnDestruction)			//Creates the ps with it's default parameters
:m_nameId(name),
m_quadArray(0),
m_particleSize(psize),
m_systemLife(syslife),
m_particleLife(plife),
m_particleNum(pnum),
m_shaderIndex(shindex),
m_deleteModelOnDestruction(deleteModelOnDestruction),
m_currentTime(0.0f)
{
	// TODO : constant parameters/uniforms need to be bound only once! So it can very well be done here.
	ShaderManager::instance()->begin(shindex);
	ShaderManager::instance()->setUniform1fv("particleLife",&m_particleLife);
	ShaderManager::instance()->setUniform1fv("particleSize",&m_particleSize);
	ShaderManager::instance()->setUniform1fv("systemLife",&m_systemLife);
	ShaderManager::instance()->end();
}

PS_Base :: ~PS_Base()
{
	if(m_deleteModelOnDestruction)
	{
		delete m_quadArray;
		m_quadArray = 0;
	}
}

void PS_Base :: setTransform(const CoordinateFrame& transform)
{
	m_transform = transform;
}