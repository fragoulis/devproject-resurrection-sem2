#include "PS_Explosion2.h"
#include "../gfx/Texture/TextureIO.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/Model.h"
#include "../Math/Vector4.h"
#include "../gfxutils/MemManager/MemMgr_RawData.h"
#include "../utility/RandomGenerator.h"
#include "../Rendering/RenderEngine.h"


#include <iostream>

static const unsigned VELNUM = 16;


// TEMPLATE CREATION CTOR
PS_Explosion2 :: PS_Explosion2(const std::string& name,
			VBO * vbo,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const std::string& texture)
:PS_Base(name,psize,syslife,plife,pnum,shindex,true)
{
	// Initialize
	_generateData(vbo,TextureIO::instance()->getTexture(texture));
}

// TEMPLATE CREATION CTOR
PS_Explosion2 :: PS_Explosion2(const std::string& name,
			Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const VAttribStatus& status)
:PS_Base(name,psize,syslife,plife,pnum,shindex,false)
{
	m_quadArray = model;
	m_usedAttribs = status;
}


PS_Base * PS_Explosion2 :: clone() const
{
	return new PS_Explosion2(m_nameId,m_quadArray,m_particleSize,m_systemLife,m_particleLife,m_particleNum,
					 m_shaderIndex,m_usedAttribs);
}


void  PS_Explosion2 :: _generateData(VBO * vbo,Texture * tex)
{
	const unsigned totalVData = m_particleNum*4;
	const unsigned totalIData = m_particleNum*6;

	// Get the VertexAttribute list
	std::vector<const VertexAttribute *> vattrs;
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib1"));

	// Set the attribute mask
	m_usedAttribs._raw[0] = 0;
	m_usedAttribs._raw[1] = 0;
	const unsigned pos_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[0]);
	const unsigned vel_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[1]);
	m_usedAttribs.attrib[pos_index] = 0xFF;
	m_usedAttribs.attrib[vel_index] = 0xFF;

	// Allocate the memory for data & indices
	std::vector<void *> data;
	unsigned * indices = MemMgrRaw::instance()->allocate<unsigned>(totalIData);
	Vector4 * positions = MemMgrRaw::instance()->allocate<Vector4>(totalVData);
	Vector4 * velocities = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib1


	// Generate VELNUM velocities
	Vector3 velvecs[VELNUM];
	unsigned used[VELNUM];
	memset(used,0,sizeof(unsigned)*VELNUM);
	for(unsigned i=0; i<VELNUM ;++i)
	{
		RandomGenerator::GET_RANDOM_VECTOR3(velvecs[i],Vector3(-1.0f,0.001f,-1.0f),Vector3(1,1,1));
		velvecs[i].normalize();
	}


	// generate the vertex data.

	for(unsigned i=0; i<totalVData ;i+=4)
	{
		// select a velocity vector
		const unsigned velindex = (i/4)%VELNUM;
		Vector3 vel = velvecs[velindex] + 0.13f*velvecs[(velindex + (used[velindex]+1))%VELNUM];
		vel.normalize();
		velocities[i] = velocities[i+1] = velocities[i+2] = velocities[i+3] = Vector4(vel.getX(),
																					  vel.getY(),
																					  vel.getZ(),
																					  float(used[velindex])*m_particleLife*0.33f*VELNUM/m_particleNum);

		positions[i] = Vector4(-1,1,0,1);
		positions[i+1] = Vector4(-1,-1,0,1);
		positions[i+2] = Vector4(1,-1,0,1);
		positions[i+3] = Vector4(1,1,0,1);
		++used[velindex];
	}
	// attach the data to the vector in the order of the vertex attributes
	data.push_back(positions);
	data.push_back(velocities);	

	// generate the index data.
	for(unsigned i=0;i<totalIData;i+=6)
	{
		const unsigned d_i = (4*i)/6;		// the data index
		indices[i] = d_i;
		indices[i+1] = d_i+1;
		indices[i+2] = d_i+3;
		indices[i+3] = d_i+3;
		indices[i+4] = d_i+1;
		indices[i+5] = d_i+2;
	}

	// Ok, now our vbo desc
	VBODesc vdesc(vbo,vattrs,data,totalVData,indices,totalIData,GL_TRIANGLES);
	// Our texture 'list'
	std::vector<Texture *> texlist;
	texlist.push_back(tex);

	// & finally the model
	m_quadArray = new Model(m_nameId,vbo);
	m_quadArray->addMatGroup(MaterialGroup(Material(),texlist,vdesc,m_shaderIndex));

	// Free our data
	MemMgrRaw::instance()->free(positions);
	MemMgrRaw::instance()->free(velocities);
	MemMgrRaw::instance()->free(indices);
}

void PS_Explosion2 :: update(const float delta)
{
	m_currentTime += delta;
}

void PS_Explosion2 :: render() const
{
	// set uniforms, bind texture, transform & call VBO
	CHECK_GL_ERROR();
	VAttribStatus curstatus = VBOMgr::instance().getCurrentFlags();
	VBOMgr::instance().setCurrentFlags(m_usedAttribs);

	glPushMatrix();
	const float * m = m_transform.getMatrix().cfp();
	glMultMatrixf(m);
	ShaderManager::instance()->begin(m_shaderIndex);
	CHECK_GL_ERROR();
	ShaderManager::instance()->setUniform1fv("currentTime",&m_currentTime);
	CHECK_GL_ERROR();
	m_quadArray->getMatGroup()[0].getTextureList()[0]->bind(0);
	CHECK_GL_ERROR();
	ShaderManager::instance()->setUniform1i("particleTex",0);
	CHECK_GL_ERROR();

	TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getNoiseTexture())->bind(1);
	ShaderManager::instance()->setUniform1i("particleNoise",1);

	ShaderManager::instance()->setUniform1f("sizeModifier",m_sizeModifier);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	m_quadArray->getMatGroup()[0].getVboDesc().call();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);

	glPopMatrix();
	
	VBOMgr::instance().setCurrentFlags(curstatus);
	ShaderManager::instance()->end();
	CHECK_GL_ERROR();
}

void PS_Explosion2 :: reset()
{
	m_currentTime = 0.0f;
}
