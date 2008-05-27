#include "PS_EnemyEnergyViz.h"
#include "../gfxutils/Texture/Texture.h"
#include "../Math/Vector4.h"
#include "../utility/RandomGenerator.h"
#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/GameLogic.h"
#include "../Rendering/RenderEngine.h"
#include "../gfx/MatrixTransform.h"
#include "../gfxutils/texture/textureMgr.h"
#include "../gfxutils/VA/VATTable.h"
#include "../math/maths.h"

void PS_EnemyEnergyViz :: update(const float delta)
{
	static const float radiansPerSec = Math::PI / 3.0f;
	static const float tailsize = Math::PI * 2.0f;
	float radius = m_ship->getRadius() * 1.5f;
	m_particleSize = radius * 0.4f;

	m_currentTime += delta;

	//m_transform = m_ship->getCoordinateFrame();

	for (int i = 0; i < m_particleNum; i++) {
		//m_particles[i].m_position += m_particles[i].m_velocity;
		//float x = /*m_particles[i].m_position.getX()+*/m_ship->getRadius();
		//float z = /*m_particles[i].m_position.getZ()+*/m_ship->getRadius();
		//m_particles[i].m_position  

		float partFactor = float(i) / float(m_particleNum);
		float factor = m_currentTime * radiansPerSec + partFactor * tailsize;

		Vector3& pos = m_particles[i].m_position;
		pos.setX(cos(factor) * radius);
		pos.setZ(sin(factor) * radius);

		//m_particles[i].m_position.setX(cos((m_currentTime+i)*2)*x + sin((m_currentTime+i)*2)*z);
		//m_particles[i].m_position.setZ(cos((m_currentTime+i)*2)*z - sin((m_currentTime+i)*2)*x);
	}
}

// TEMPLATE CREATION CTOR
PS_EnemyEnergyViz :: PS_EnemyEnergyViz(const std::string& name,
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
		particle.m_position.set(0.0f, 0.0f, 0.0f);
		m_particles.push_back(particle);
	}
	//for (int i = 0; i < pnum; i++) {
	//	m_particles[i].m_position = Vector3(0,0,0);

	//	Vector3 vel = Vector3(RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1));
	//	m_particles[i].m_velocity = Vector3(pnum*10, 0, pnum*10);
	//	m_particles[i].m_age      = 0.0f;
	//	m_particles[i].m_lifeTime = 1.0f; // lives for 1 seconds
	//}
}

// TEMPLATE CREATION CTOR
PS_EnemyEnergyViz :: PS_EnemyEnergyViz(const std::string& name,
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
		particle.m_position.set(0.0f, 0.0f, 0.0f);
		m_particles.push_back(particle);
	}
	//for (int i = 0; i < pnum; i++) {
	//	m_particles[i].m_position = Vector3(0,0,0);

	//	Vector3 vel = Vector3(RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1));
	//	m_particles[i].m_velocity = vel*RandomGenerator::GET_RANDOM_FLOAT(5.0f, 25.0f);
	//	m_particles[i].m_age      = 0.0f;
	//	m_particles[i].m_lifeTime = 5.0f; 
	//}
}

PS_EnemyEnergyViz :: ~PS_EnemyEnergyViz() {
	//TextureMgr::instance().unloadPalette(m_flarePal);

	//delete[] m_particles;
}

PS_Base * PS_EnemyEnergyViz :: clone() const
{
	return new PS_EnemyEnergyViz(m_nameId,m_particleSize,m_systemLife,m_particleLife,m_particleNum);
}


void PS_EnemyEnergyViz :: render() const
{
	//RenderEngine::useColorChannelForAlpha(GX_CH_RED);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);	
	//GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVSRCCLR, GX_LO_CLEAR);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_CLEAR);
	//GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
	GXSetZMode(FALSE, GX_ALWAYS, FALSE);

	//Texture tex_pos(m_flarePal,0,"flare_pos");
	Texture *tex_pos = TextureMgr::instance().getTexture("flareWhite");
	tex_pos->bind();

	GXSetVtxDescv(VATTable::getVDL(7));

	static const GXColor enemyColors[3] =
		{
			{ 255, 80, 80, 255 },
			{ 235, 235, 50, 255 },
			{ 80, 80, 255, 255 }
		};

	GXColor color = enemyColors[(int) m_ship->getEnergyType()];
	RenderEngine::enableModulateTextureColor();
	RenderEngine::setModulateTextureColor(color);
	//RenderEngine::enableModulateTextureAlpha();

	GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_KONST, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
	GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K1_R);

	const Vector3 up(0, 0, -m_particleSize);
	const Vector3 ll(0,0,0);
	const Vector3 right(m_particleSize, 0, 0);
	const Vector3& originOffset = m_ship->getPosition().getVector();

	for (int i = 0; i < m_particleNum; i++)
	{
		//GXColor color = enemyColors[(int) m_ship->getEnergyType()];
		//float factor = min(1.0f, 3.0f * (1.0f - ()));
		float factor = min(1.0f, 1.0f * float(i) / float(m_particleNum) + 0.2f);
		//color.r = u8( color.r * factor);
		//color.g = u8( color.g * factor);
		//color.b = u8( color.b * factor);
		//color.a = u8( color.a * factor);
		//RenderEngine::setModulateTextureColor(color);
		RenderEngine::setModulateTextureAlpha(u8( factor * 255.0f ));


		RenderEngine::drawQuad(m_particles[i].m_position + originOffset, right, up);
	}

	GXSetBlendMode(GX_BM_NONE , GX_BL_SRCCLR, GX_BL_INVSRCCLR, GX_LO_CLEAR);
	GXSetZMode(TRUE, GX_LEQUAL, TRUE);
}

void PS_EnemyEnergyViz :: reset()
{
	m_currentTime = 0.0f;
}


//
//#include <iostream>
//
//
//// TEMPLATE CREATION CTOR
//PS_EnemyEnergyViz :: PS_EnemyEnergyViz(const std::string& name,
//			VBO * vbo,
//			const float psize,
//			const float syslife,
//			const float plife,
//			const unsigned pnum,
//			const int shindex,
//			const std::string& texture,
//			const float speed,
//			const float tailSize)
//:PS_Base(name,psize,syslife,plife,pnum,shindex,true),
//m_speed(speed),
//m_tailSize(tailSize)
//{
//	// Initialize
//	_generateData(vbo,TextureIO::instance()->getTexture(texture));
//	m_ship = NULL;
//}
//
//// TEMPLATE CREATION CTOR
//PS_EnemyEnergyViz :: PS_EnemyEnergyViz(const std::string& name,
//			Model * model,
//			const float psize,
//			const float syslife,
//			const float plife,
//			const unsigned pnum,
//			const int shindex,
//			const VAttribStatus& status,
//			const float speed,
//			const float tailSize)
//:PS_Base(name,psize,syslife,plife,pnum,shindex,false),
//m_speed(speed),
//m_tailSize(tailSize)
//{
//	m_quadArray = model;
//	m_usedAttribs = status;
//}
//
//
//PS_Base * PS_EnemyEnergyViz :: clone() const
//{
//	return new PS_EnemyEnergyViz(m_nameId,m_quadArray,m_particleSize,m_systemLife,m_particleLife,m_particleNum,
//					 m_shaderIndex,m_usedAttribs,m_speed,m_tailSize);
//}
//
//
//void  PS_EnemyEnergyViz :: _generateData(VBO * vbo,Texture * tex)
//{
//	const unsigned totalVData = m_particleNum*4;
//	const unsigned totalIData = m_particleNum*6;
//
//	// Get the VertexAttribute list
//	std::vector<const VertexAttribute *> vattrs;
//	vattrs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
//	vattrs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib1"));
//
//	// Set the attribute mask
//	m_usedAttribs._raw[0] = 0;
//	m_usedAttribs._raw[1] = 0;
//	const unsigned pos_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[0]);
//	const unsigned vel_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[1]);
//	m_usedAttribs.attrib[pos_index] = 0xFF;
//	m_usedAttribs.attrib[vel_index] = 0xFF;
//	//m_usedAttribs.attrib[off_index] = 0xFF;
//
//	// Allocate the memory for data & indices
//	std::vector<void *> data;
//	unsigned * indices = MemMgrRaw::instance()->allocate<unsigned>(totalIData);
//	Vector4 * positions = MemMgrRaw::instance()->allocate<Vector4>(totalVData);
//	Vector4 * velocities = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib1
//
//	// generate the vertex data.
//
//	for(unsigned i=0; i<totalVData ;i+=4)
//	{
//		float ioffset = i / (totalVData - 1.0f);
//
//		Vector3 out;
//		RandomGenerator::GET_RANDOM_VECTOR3(out,Vector3(-0.2f,-0.2f,-0.2f),Vector3(0.2f,0.2f,0.2f));
//	
//		velocities[i] = velocities[i+1] = velocities[i+2] = velocities[i+3] = Vector4(ioffset,
//																					  out.getX(),
//																					  out.getY(),
//																					  out.getZ());
//		positions[i] = Vector4(-1,1,0,1);
//		positions[i+1] = Vector4(-1,-1,0,1);
//		positions[i+2] = Vector4(1,-1,0,1);
//		positions[i+3] = Vector4(1,1,0,1);
//	}
//	// attach the data to the vector in the order of the vertex attributes
//	data.push_back(positions);
//	data.push_back(velocities);		
//
//	// generate the index data.
//	for(unsigned i=0;i<totalIData;i+=6)
//	{
//		const unsigned d_i = (4*i)/6;		// the data index
//		indices[i] = d_i;
//		indices[i+1] = d_i+1;
//		indices[i+2] = d_i+3;
//		indices[i+3] = d_i+3;
//		indices[i+4] = d_i+1;
//		indices[i+5] = d_i+2;
//	}
//
//	// Ok, now our vbo desc
//	VBODesc vdesc(vbo,vattrs,data,totalVData,indices,totalIData,GL_TRIANGLES);
//	// Our texture 'list'
//	std::vector<Texture *> texlist;
//	texlist.push_back(tex);
//
//	// & finally the model
//	m_quadArray = new Model(m_nameId,vbo);
//	m_quadArray->addMatGroup(MaterialGroup(Material(),texlist,vdesc,m_shaderIndex));
//
//	// Free our data
//	MemMgrRaw::instance()->free(positions);
//	MemMgrRaw::instance()->free(velocities);
//	MemMgrRaw::instance()->free(indices);
//	//MemMgrRaw::instance()->free(offsets);
//}
//
//void PS_EnemyEnergyViz :: update(const float delta)
//{
//	m_currentTime += delta;
//	m_transform = m_ship->getCoordinateFrame();
//}
//
//void PS_EnemyEnergyViz :: render() const
//{
//
//	// set uniforms, bind texture, transform & call VBO
//	CHECK_GL_ERROR();
//	VAttribStatus curstatus = VBOMgr::instance().getCurrentFlags();
//	VBOMgr::instance().setCurrentFlags(m_usedAttribs);
//
//	glPushMatrix();
//	const float * m = m_transform.getMatrix().cfp();
//	glMultMatrixf(m);
//	ShaderManager::instance()->begin(m_shaderIndex);
//	CHECK_GL_ERROR();
//	ShaderManager::instance()->setUniform1fv("currentTime",&m_currentTime);
//	CHECK_GL_ERROR();
//	ShaderManager::instance()->setUniform1fv("tailSize",&m_tailSize);
//	ShaderManager::instance()->setUniform1fv("radsPerSec",&m_speed);
//	Vector4 energycolor(RenderEngine::instance().getColorFromEnergyType(m_ship->getEnergyType()));
//	ShaderManager::instance()->setUniform4fv("energyColor",energycolor.cfp());
//	const float radius = m_ship->getRadius();
//	ShaderManager::instance()->setUniform1fv("shipRadius",&radius);
//	const float height = RenderEngine::instance().getLevelExtents().getY();
//	ShaderManager::instance()->setUniform1fv("height",&height);
//	CHECK_GL_ERROR();
//	m_quadArray->getMatGroup()[0].getTextureList()[0]->bind(0);
//	CHECK_GL_ERROR();
//	ShaderManager::instance()->setUniform1i("texmap0",0);
//	CHECK_GL_ERROR();
//	m_quadArray->getMatGroup()[0].getVboDesc().call();
//
//	glPopMatrix();
//	
//	VBOMgr::instance().setCurrentFlags(curstatus);
//	ShaderManager::instance()->end();
//	CHECK_GL_ERROR();
//}
//
//void PS_EnemyEnergyViz :: reset()
//{
//	m_currentTime = 0.0f;
//}
