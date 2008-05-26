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
	m_flarePal = TextureMgr::instance().loadPalette("flare2.tpl","flareTPL.txt");
	//m_particles = new Particle[pnum];
	for (int i = 0; i < pnum; i++) {
		Particle particle;
		m_particles.push_back(particle);
	}
	for (int i = 0; i < pnum; i++) {
		m_particles[i].m_position = Vector3(0,0,0);

		Vector3 vel = Vector3(RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1));
		m_particles[i].m_velocity = vel*RandomGenerator::GET_RANDOM_FLOAT(50.0f, 25.0f);
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
	m_flarePal = TextureMgr::instance().loadPalette("flare2.tpl","flareTPL.txt");
	for (int i = 0; i < pnum; i++) {
		Particle particle;
		m_particles.push_back(particle);
	}
	for (int i = 0; i < pnum; i++) {
		m_particles[i].m_position = Vector3(0,0,0);

		Vector3 vel = Vector3(RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1), RandomGenerator::GET_RANDOM_FLOAT(-1,1));
		m_particles[i].m_velocity = vel*RandomGenerator::GET_RANDOM_FLOAT(5.0f, 25.0f);
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


//void  PS_Explosion :: _generateData(VBO * vbo,Texture * tex)
//{
//	const unsigned totalVData = m_particleNum*4;
//	const unsigned totalIData = m_particleNum*6;
//
//	// Get the VertexAttribute list
//	std::vector<const VertexAttribute *> vattrs;
//	vattrs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
//	vattrs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib1"));
//	//vattrs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib2"));
//
//	// Set the attribute mask
//	m_usedAttribs._raw[0] = 0;
//	m_usedAttribs._raw[1] = 0;
//	const unsigned pos_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[0]);
//	const unsigned vel_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[1]);
//	//const unsigned off_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[2]);
//	m_usedAttribs.attrib[pos_index] = 0xFF;
//	m_usedAttribs.attrib[vel_index] = 0xFF;
//	//m_usedAttribs.attrib[off_index] = 0xFF;
//
//	// Allocate the memory for data & indices
//	std::vector<void *> data;
//	unsigned * indices = MemMgrRaw::instance()->allocate<unsigned>(totalIData);
//	Vector4 * positions = MemMgrRaw::instance()->allocate<Vector4>(totalVData);
//	Vector4 * velocities = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib1
//	//Vector4 * offsets = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib2
//
//	// generate the vertex data.
//
//	for(unsigned i=0; i<totalVData ;i+=4)
//	{
//		Vector3 vel;
//		RandomGenerator::GET_RANDOM_VECTOR3(vel,Vector3(-1,-1,-1),Vector3(1,1,1));
//		vel.normalize();
//		vel.multiply(5.0f + RandomGenerator::GET_RANDOM_FLOAT(1.0f,2.5f));
//		velocities[i] = velocities[i+1] = velocities[i+2] = velocities[i+3] = Vector4(vel.getX(),
//																					  vel.getY(),
//																					  vel.getZ(),
//																					  float(i)*m_particleLife*0.25f/m_particleNum);
//
//		/*offsets[i] = offsets[i+1] = offsets[i+2] = offsets[i+3] = Vector4(vel.getX()*10,
//																					  vel.getY()*10,
//																					  vel.getZ()*10,
//																					  0.0f);*/
//
//		//std::cout<<velocities[i].getW()<<std::endl;
//		positions[i] = Vector4(-1,1,0,1);
//		positions[i+1] = Vector4(-1,-1,0,1);
//		positions[i+2] = Vector4(1,-1,0,1);
//		positions[i+3] = Vector4(1,1,0,1);
//	}
//	// attach the data to the vector in the order of the vertex attributes
//	data.push_back(positions);
//	data.push_back(velocities);		
//	//data.push_back(offsets);	
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

void PS_Explosion :: update(const float delta)
{
	m_currentTime += delta;

	for (int i = 0; i < 500; i++) {
		m_particles[i].m_position += m_particles[i].m_velocity;;
		m_particles[i].m_age      += delta;
	}
}

void PS_Explosion :: render() const
{
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);	
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCCLR, GX_BL_ONE, GX_LO_CLEAR);
	GXSetZMode(FALSE, GX_ALWAYS, FALSE);

	Texture tex_pos(m_flarePal,0,"flare_pos");
	tex_pos.bind();

	GXSetVtxDescv(VATTable::getVDL(7));

	const Vector3 up(0, 0, -50);
	const Vector3 ll(0,0,0);
	const Vector3 right(50, 0, 0);

	for (int i = 0; i < 500; i++) {
		//MatrixTransform::PushMatrix();
		//MatrixTransform::MulMatrix(m_transform.getMatrix());
		//MatrixTransform::Translate(m_particles[i].m_position.getX(), m_particles[i].m_position.getY(), m_particles[i].m_position.getZ());

		Vector3 ll(m_particles[i].m_position + m_transform.getOrigin().getVector() );
		
		RenderEngine::drawQuad(ll,right,up);
		//MatrixTransform::PopMatrix();
	}

	GXSetBlendMode(GX_BM_NONE , GX_BL_SRCCLR, GX_BL_INVSRCCLR, GX_LO_CLEAR);
	GXSetZMode(TRUE, GX_LEQUAL, TRUE);
}

void PS_Explosion :: reset()
{
	m_currentTime = 0.0f;
}
