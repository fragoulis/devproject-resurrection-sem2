#include "PS_Manager.h"
#include "PS_Base.h"
#include "../gfx/VBO/VBO.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfxutils/ConfParser/ParserSection.h"

using namespace std;

PS_Manager :: PS_Manager()
:m_vbo(0)
{
}

PS_Manager :: ~PS_Manager()
{
}

void PS_Manager :: init(const ParserSection * parsec)
{
	// Parse the max number of quads
	const unsigned maxquads = FromString<unsigned>(parsec->getVal("MaxTotalParticles"));

	// Create our VBO with prespecified attribs, 3 generics for now, if more are needed, 
	// don't forget to specify it here
	std::vector<const VertexAttribute *> attribs;
	attribs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
	attribs.push_back(ShaderManager::instance()->vertexAttribute("Texcoord"));
	attribs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib1"));
	attribs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib2"));
	attribs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib3"));

	m_vbo = new VBO(attribs,
					maxquads*4,
					maxquads*6,
					GL_STATIC_DRAW,
					"ParticleVBO");

	// Now build all our particle systems;
}

void PS_Manager :: clear()
{
	for(vector<PS_Base *>::iterator it = m_psList.begin();
		it != m_psList.end();
		++it)
	{
		delete (*it);
		(*it) = 0;
	}
	m_psList.clear();
	if(m_vbo)
		delete m_vbo;
}