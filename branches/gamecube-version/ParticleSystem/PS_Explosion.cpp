#include "PS_Explosion.h"
//#include "../gfx/Texture/TextureIO.h"
#include "../gfxutils/Texture/Texture.h"
//#include "../gfx/Shaders/ShaderManager.h"
//#include "../gfx/Model/Model.h"
#include "../Math/Vector4.h"
//#include "../gfxutils/MemManager/MemMgr_RawData.h"
#include "../utility/RandomGenerator.h"
#include "../rendering/RenderEngine.h"
#include "../gfx/MatrixTransform.h"
#include "../gfxutils/texture/textureMgr.h"
#include "../gfxutils/VA/VATTable.h"




#include <iostream>


// TEMPLATE CREATION CTOR
PS_Explosion :: PS_Explosion(const std::string& name,
			/*VBO * vbo,*/
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			//const int shindex,
			const std::string& texture)
:PS_Base(name,psize,syslife,plife,pnum)
{
	// Initialize
	//_generateData(vbo,TextureIO::instance()->getTexture(texture));
	m_flarePal = TextureMgr::instance().loadPalette("flareWhite.tpl","flareWhiteTPL.txt");
	//m_particles = new Particle[pnum];
	for (int i = 0; i < pnum; i++) {
		Particle particle;
		m_particles.push_back(particle);
	}
	for (int i = 0; i < pnum; i++) {
		m_particles[i].m_position = Vector3(0,0,0);

		Vector3 vel = Vector3(RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1));
		vel.normalize();
		m_particles[i].m_velocity = vel * RandomGenerator::GET_RANDOM_FLOAT(5.0f, 10.0f);
		m_particles[i].m_age      = 0.0f;
		m_particles[i].m_lifeTime = 1.0f; // lives for 1 seconds
	}
}

// TEMPLATE CREATION CTOR
PS_Explosion :: PS_Explosion(const std::string& name,
			/*Model * model,*/
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum/*,
			const int shindex,*/
			/*const VAttribStatus& status*/)
:PS_Base(name,psize,syslife,plife,pnum)
{
	//m_quadArray = model;
	//m_usedAttribs = status;
	m_flarePal = TextureMgr::instance().loadPalette("flareWhite.tpl","flareWhiteTPL.txt");
	for (int i = 0; i < pnum; i++) {
		Particle particle;
		m_particles.push_back(particle);
	}
	for (int i = 0; i < pnum; i++) {
		m_particles[i].m_position = Vector3(0,0,0);

		Vector3 vel = Vector3(RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1));
		vel.normalize();
		m_particles[i].m_velocity = vel*RandomGenerator::GET_RANDOM_FLOAT(5.0f, 10.0f);
		m_particles[i].m_age      = 0.0f;
		m_particles[i].m_lifeTime = 5.0f; 
	}
}

PS_Explosion :: ~PS_Explosion() {
	//TextureMgr::instance().unloadPalette(m_flarePal);

	//delete[] m_particles;
}

PS_Base * PS_Explosion :: clone() const
{
	return new PS_Explosion(m_nameId,m_particleSize,m_systemLife,m_particleLife,m_particleNum);
}

void PS_Explosion :: setVelocity(const Vector3& v)
{
	for (int i = 0; i < m_particleNum; i++) {
		Particle& part = m_particles[i];
		part.m_velocity += v;
	}
}

void PS_Explosion :: update(const float delta)
{
	m_currentTime += delta;

	for (int i = 0; i < m_particleNum; i++) {
		Particle& part = m_particles[i];
		part.m_position += part.m_velocity;
		//part.m_age      += delta;
	}
}

void PS_Explosion :: render() const
{
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);	
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVSRCCLR, GX_LO_CLEAR);
	GXSetZMode(FALSE, GX_ALWAYS, FALSE);

	//Texture tex_pos(m_flarePal,0,"flare_pos");
	Texture *tex_pos = TextureMgr::instance().getTexture("flareWhite");
	tex_pos->bind();

	GXSetVtxDescv(VATTable::getVDL(7));

	//RenderEngine::useColorChannelForAlpha(GX_CH_RED);
	GXColor color = explosionColor[0];
	float factor = min(1.0f, 2.0f * (1.0f - (m_currentTime / m_systemLife)));
	color.r = u8( color.r * factor);
	color.g = u8( color.g * factor);
	color.b = u8( color.b * factor);
	color.a = u8( color.a * factor);

	RenderEngine::enableModulateTextureColor();
	RenderEngine::setModulateTextureColor(color);
	//RenderEngine::enableModulateTextureAlpha();
	//RenderEngine::setModulateTextureAlpha(u8(m_currentTime / m_systemLife * 255.0f));

	const Vector3 up(0, 0, -m_particleSize);
	const Vector3 ll(0,0,0);
	const Vector3 right(m_particleSize, 0, 0);
	const Vector3& originOffset = m_transform.getOrigin().getVector();

	for (int i = 0; i < m_particleNum; i++) {
		//MatrixTransform::PushMatrix();
		//MatrixTransform::MulMatrix(m_transform.getMatrix());
		//MatrixTransform::Translate(m_particles[i].m_position.getX(), m_particles[i].m_position.getY(), m_particles[i].m_position.getZ());

		RenderEngine::drawQuad(m_particles[i].m_position + originOffset, right, up);
		//MatrixTransform::PopMatrix();
	}

	GXSetBlendMode(GX_BM_NONE , GX_BL_SRCALPHA, GX_BL_INVSRCCLR, GX_LO_CLEAR);
	GXSetZMode(TRUE, GX_LEQUAL, TRUE);
}

void PS_Explosion :: reset()
{
	m_currentTime = 0.0f;
}
