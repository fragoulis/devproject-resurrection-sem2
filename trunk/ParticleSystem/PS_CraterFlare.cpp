#include "PS_CraterFlare.h"
#include "../gfx/Texture/TextureIO.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/Model.h"
#include "../Math/Vector4.h"
#include "../gfxutils/MemManager/MemMgr_RawData.h"
#include "../utility/RandomGenerator.h"
#include "../GameLogic/Objects/Crater.h"
#include "../GameLogic/GameLogic.h"

#include <iostream>


// TEMPLATE CREATION CTOR
PS_CraterFlare :: PS_CraterFlare(const std::string& name,
			VBO * vbo,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const std::string& texture,
			const float speed,
			const float tailSize)
:PS_Base(name,psize,syslife,plife,pnum,shindex,true),
m_speed(speed),
m_tailSize(tailSize)
{
	// Initialize
	_generateData(vbo,TextureIO::instance()->getTexture(texture));
	m_crater = NULL;
}

// TEMPLATE CREATION CTOR
PS_CraterFlare :: PS_CraterFlare(const std::string& name,
			Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const VAttribStatus& status,
			const float speed,
			const float tailSize)
:PS_Base(name,psize,syslife,plife,pnum,shindex,false),
m_speed(speed),
m_tailSize(tailSize)
{
	m_quadArray = model;
	m_usedAttribs = status;
}


PS_Base * PS_CraterFlare :: clone() const
{
	return new PS_CraterFlare(m_nameId,m_quadArray,m_particleSize,m_systemLife,m_particleLife,m_particleNum,
					 m_shaderIndex,m_usedAttribs,m_speed,m_tailSize);
}


void  PS_CraterFlare :: _generateData(VBO * vbo,Texture * tex)
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
	//m_usedAttribs.attrib[off_index] = 0xFF;

	// Allocate the memory for data & indices
	std::vector<void *> data;
	unsigned * indices = MemMgrRaw::instance()->allocate<unsigned>(totalIData);
	Vector4 * positions = MemMgrRaw::instance()->allocate<Vector4>(totalVData);
	Vector4 * velocities = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib1

	// generate the vertex data.

	for(unsigned i=0; i<totalVData ;i+=4)
	{
		float ioffset = i / (totalVData - 1.0f);

		Vector3 out;
		RandomGenerator::GET_RANDOM_VECTOR3(out,Vector3(-0.2,-0.2,-0.2),Vector3(0.2,0.2,0.2));
	
		velocities[i] = velocities[i+1] = velocities[i+2] = velocities[i+3] = Vector4(ioffset,
																					  out.getX(),
																					  out.getY(),
																					  out.getZ());
		positions[i] = Vector4(-1,1,0,1);
		positions[i+1] = Vector4(-1,-1,0,1);
		positions[i+2] = Vector4(1,-1,0,1);
		positions[i+3] = Vector4(1,1,0,1);
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
	//MemMgrRaw::instance()->free(offsets);
}

void PS_CraterFlare :: update(const float delta)
{
	m_currentTime += delta;
}

void PS_CraterFlare :: render() const
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
	ShaderManager::instance()->setUniform1fv("tailSize",&m_tailSize);
	ShaderManager::instance()->setUniform1fv("radsPerSec",&m_speed);
	Vector4 cratercolor;
	switch(m_crater->getEbombType())
	{
		case EBOMB_TYPE_RED : 
			cratercolor.set(1.0f,0.0f,0.0f,1.0f);
			break;
		case EBOMB_TYPE_YELLOW : 
			cratercolor.set(1.0f,1.0f,0.0f,1.0f);
			break;
		case EBOMB_TYPE_BLUE : 
			cratercolor.set(0.0f,0.0f,1.0f,1.0f);
			break;
		case EBOMB_TYPE_ORANGE: 
			cratercolor.set(1.0f,0.5f,0.0f,1.0f);
			break;
		case EBOMB_TYPE_GREEN: 
			cratercolor.set(0.0f,1.0f,0.0f,1.0f);
			break;
		case EBOMB_TYPE_PURPLE: 
			cratercolor.set(1.0f,0.0f,1.0f,1.0f);
			break;
	}
	ShaderManager::instance()->setUniform4fv("craterColor",cratercolor.cfp());
	const float radius = m_crater->getRadius();
	ShaderManager::instance()->setUniform1fv("craterRadius",&radius);
	const float height = GameLogic::instance().getGamePlaneHeight();
	ShaderManager::instance()->setUniform1fv("height",&height);
	CHECK_GL_ERROR();
	m_quadArray->getMatGroup()[0].getTextureList()[0]->bind(0);
	CHECK_GL_ERROR();
	ShaderManager::instance()->setUniform1i("particleTex",0);
	CHECK_GL_ERROR();
	m_quadArray->getMatGroup()[0].getVboDesc().call();

	glPopMatrix();
	
	VBOMgr::instance().setCurrentFlags(curstatus);
	ShaderManager::instance()->end();
	CHECK_GL_ERROR();
}

void PS_CraterFlare :: reset()
{
	m_currentTime = 0.0f;
}
