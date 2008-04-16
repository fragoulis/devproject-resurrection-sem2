//*****************************************************************************
//**
//**   File:               PS_Manager.cpp
//**   Author:             $Author: manuele.bonanno $
//**   Last Change:        $Date: 2008-03-19 11:53:22 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 191 $
//**
//*****************************************************************************

#include "PS_Manager.h"
#include "PS_EnergyLoss.h"
#include "PS_Explosion.h"
#include "PS_MultiTexExplosion.h"
#include "PS_Fountain.h"
#include "PS_Clouds.h"
#include "PS_RotatingFlare.h"
#include "PS_EnemyEnergyViz.h"
#include "PS_Jet.h"
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

				const string texname = (*it)->getVal("Texture");
				created_ps = new PS_Explosion((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname);

			}
			else if(pstype == "PS_ColouredExplosion")
			{

				const string texname = (*it)->getVal("Texture");
				const string shapetexname = (*it)->getVal("ShapeTexture");
				created_ps = new PS_MultiTexExplosion((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname,shapetexname);

			}
			else if(pstype == "PS_RedEnemyExplosion" || pstype == "PS_YellowEnemyExplosion" || pstype == "PS_BlueEnemyExplosion")
			{

				const string texname = (*it)->getVal("Texture");
				const string shapetexname = (*it)->getVal("ShapeTexture");
				created_ps = new PS_MultiTexExplosion((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname,shapetexname);

			}
			else if(pstype == "PS_Jet")
			{

				const string texname = (*it)->getVal("Texture");
				created_ps = new PS_Jet((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname);

			}
			else if(pstype == "PS_Fountain")
			{
				//const int particleColor = FromString<int>((*it)->getVal("ParticleColor"));
				const string texname = (*it)->getVal("Texture");
				created_ps = new PS_Fountain((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,PS_Fountain::FOUNTAIN_TYPE_FADE_OUT,texname);
			}
			else if(pstype == "PS_Clouds")
			{
				const string texname = (*it)->getVal("Texture");
				const string noisetexname = (*it)->getVal("NoiseTexture");
				created_ps = new PS_Clouds((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname,noisetexname);
			}
			else if(pstype == "PS_RotatingFlare")
			{
				const string texname = (*it)->getVal("Texture");
				const float tailSize = FromString<float>((*it)->getVal("TailSize"));
				const float speed = FromString<float>((*it)->getVal("Speed"));
				created_ps = new PS_RotatingFlare((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname,speed,tailSize);
			}
			else if(pstype == "PS_EnemyEnergyViz")
			{
				const string texname = (*it)->getVal("Texture");
				const float tailSize = FromString<float>((*it)->getVal("TailSize"));
				const float speed = FromString<float>((*it)->getVal("Speed"));
				created_ps = new PS_EnemyEnergyViz((*it)->getName(),m_vbo,particleSize,systemLife,particleLife,particleNum,shaderIndex,texname,speed,tailSize);
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