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
	vector<const ParserSection *> parsed_ps = parsec->getChildren();
	for(vector<const ParserSection *>::iterator it = parsed_ps.begin();
		it != parsed_ps.end();
		++it)
	{
		// Get type & variations & fill the appropriate struct
		const unsigned variations = FromString<unsigned>((*it)->getVal("Variations"));
		m_psDescList.push_back(PSNameVariations((*it)->getName(),variations,unsigned(m_psList.size())));

		for(unsigned i=0;i<variations;++i)
		{
			PS_Base * created_ps(0);
			// fetch the standard PS_Base parameters
			// fetch the type in order to fetch the rest required parameters
			// generate the random starting data / model
			// push the appropriate ps to m_psList
			
			m_psList.push_back(created_ps);
		}
	}
}

void PS_Manager :: clear()
{
	for(vector<PS_Base *>::iterator it = m_psList.begin();
		it != m_psList.end();
		++it)
	{
		if(*it)	// FIXME : just for now, that I fill with NULL pointers
		{
			delete (*it);
			(*it) = 0;
		}
	}
	m_psList.clear();
	if(m_vbo)
		delete m_vbo;
}

PS_Base * PS_Manager :: fetchNewPS(const std::string& name)
{
	std::vector<PSNameVariations>::iterator queriedPS = m_psDescList.begin();
	for(;
		queriedPS != m_psDescList.end();
		++queriedPS)
	{
		if(queriedPS->name == name)
			break;
	}
	assert(queriedPS != m_psDescList.end());			// assert we asked for an existing particle system name

	// create a new one based one the template.
	PS_Base * ret = m_psList[queriedPS->posInList]->clone();
	return ret;
}