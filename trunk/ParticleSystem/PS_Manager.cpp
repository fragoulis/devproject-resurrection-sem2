//*****************************************************************************
//**
//**   File:               PS_Manager.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "PS_Manager.h"
#include "PS_EnergyLoss.h"
#include "PS_Explosion.h"
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

	// PACK TIGHTLY!!! Less Attributes -> More Particles

	std::vector<const VertexAttribute *> attribs;
	attribs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
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
			const float particleLife = FromString<float>((*it)->getVal("ParticleLife"));
			const float systemLife = FromString<float>((*it)->getVal("SystemLife"));
			const float particleSize = FromString<float>((*it)->getVal("ParticleSize"));
			const unsigned particleNum = FromString<unsigned>((*it)->getVal("ParticleNum"));
			const int shaderIndex = ShaderManager::instance()->getShaderIndex((*it)->getVal("Shader"));

			// fetch the name in order to fetch the rest required parameters
			const std::string pstype = (*it)->getName();
			if(pstype == "PS_RedEnergyLoss" || pstype == "PS_YellowEnergyLoss" || pstype == "PS_BlueEnergyLoss")
			{
				const int particleColor = FromString<int>((*it)->getVal("ParticleColor"));
				const string texname = (*it)->getVal("Texture");
				created_ps = new PS_EnergyLoss((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,particleColor,texname);
			}
			else if(pstype == "PS_Explosion")
			{
				// The PS_SomeOther class, derived from PS_Base, might need extra variables.
				// If so,parse them & create the particle system

				const string texname = (*it)->getVal("Texture");
				created_ps = new PS_Explosion((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname);

			}
			else if(pstype == "PS_ColouredExplosion")
			{
				// The PS_SomeOther class, derived from PS_Base, might need extra variables.
				// If so,parse them & create the particle system

				const string texname = (*it)->getVal("Texture");
				created_ps = new PS_Explosion((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname);

			}
			else if(pstype == "PS_RedEnemyExplosion" || pstype == "PS_YellowEnemyExplosion" || pstype == "PS_BlueEnemyExplosion")
			{
				// The PS_SomeOther class, derived from PS_Base, might need extra variables.
				// If so,parse them & create the particle system

				const string texname = (*it)->getVal("Texture");
				created_ps = new PS_Explosion((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname);

			}
			else if(pstype == "PS_Fountain")
			{
				// The PS_SomeOther class, derived from PS_Base, might need extra variables.
				// If so,parse them & create the particle system

				const string texname = (*it)->getVal("Texture");
				created_ps = new PS_Explosion((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname);

			}
			
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
			//delete (*it)->model() ?? because here we hold the templates??
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