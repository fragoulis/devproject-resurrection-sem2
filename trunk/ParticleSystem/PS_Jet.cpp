//*****************************************************************************
//**
//**   File:               PS_Jet.cpp
//**   Author:             $Author: Manuele.Bonanno $
//**   Last Change:        $Date: 2008-03-16 19:42:46 +0000 (dom, 16 mar 2008) $
//**   Last Revision:      $Revision: 176 $
//**
//*****************************************************************************

#include "PS_Jet.h"
#include "../gfx/Texture/TextureIO.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/Model.h"
#include "../Math/Vector4.h"
#include "../gfxutils/MemManager/MemMgr_RawData.h"
#include "../utility/RandomGenerator.h"
#include "../GameLogic/Objects/Playership.h"

#include <iostream>


// TEMPLATE CREATION CTOR
PS_Jet :: PS_Jet(const std::string& name,
			VBO * vbo,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const std::string& texture)
:PS_Base(name,psize,syslife,plife,pnum,shindex,true), m_jetSpeed(0.0f)
{
	// Initialize
	_generateData(vbo,TextureIO::instance()->getTexture(texture));

	m_emitterShip = NULL;
}

// TEMPLATE CREATION CTOR
PS_Jet :: PS_Jet(const std::string& name,
			Model * model,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const VAttribStatus& status)
:PS_Base(name,psize,syslife,plife,pnum,shindex,false), m_jetSpeed(0.0f)
{
	m_quadArray = model;
	m_usedAttribs = status;

	m_emitterShip = NULL;
}


PS_Base * PS_Jet :: clone() const
{
	return new PS_Jet(m_nameId,m_quadArray,m_particleSize,m_systemLife,m_particleLife,m_particleNum,
					 m_shaderIndex,m_usedAttribs);
}


void  PS_Jet :: _generateData(VBO * vbo,Texture * tex)
{
	const unsigned totalVData = m_particleNum*4;
	const unsigned totalIData = m_particleNum*6;

	// Get the VertexAttribute list
	std::vector<const VertexAttribute *> vattrs;
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib1"));
	//vattrs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib2"));

	// Set the attribute mask
	m_usedAttribs._raw[0] = 0;
	m_usedAttribs._raw[1] = 0;
	const unsigned pos_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[0]);
	const unsigned vel_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[1]);
	//const unsigned off_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[2]);
	m_usedAttribs.attrib[pos_index] = 0xFF;
	m_usedAttribs.attrib[vel_index] = 0xFF;
	//m_usedAttribs.attrib[off_index] = 0xFF;

	// Allocate the memory for data & indices
	std::vector<void *> data;
	unsigned * indices = MemMgrRaw::instance()->allocate<unsigned>(totalIData);
	Vector4 * positions = MemMgrRaw::instance()->allocate<Vector4>(totalVData);
	Vector4 * velocities = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib1
	//Vector4 * offsets = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib2

	// generate the vertex data.

	for(unsigned i=0; i<totalVData ;i+=4)
	{
		Vector3 vel;
		RandomGenerator::GET_RANDOM_VECTOR3(vel,Vector3(-0.4f,-0.4f,1.0f),Vector3(0.4f,0.4f,1.0f));
		vel.normalize();
		vel.multiply(5.0f + RandomGenerator::GET_RANDOM_FLOAT(1.0f,2.5f));
		//velocities[i] = velocities[i+1] = velocities[i+2] = velocities[i+3] = Vector4(0.0f,
		//																			  0.0f,
		//																			  0.0f,
		//																			  float(i)*m_particleLife*0.25f/m_particleNum);
		velocities[i] = velocities[i+1] = velocities[i+2] = velocities[i+3] = Vector4(vel.getX(),
																					  vel.getY(),
																					  vel.getZ(),
																					  float(i)*m_particleLife*0.25f/m_particleNum);

		/*offsets[i] = offsets[i+1] = offsets[i+2] = offsets[i+3] = Vector4(vel.getX()*10,
																					  vel.getY()*10,
																					  vel.getZ()*10,
																					  0.0f);*/

		//std::cout<<velocities[i].getW()<<std::endl;
		positions[i] = Vector4(-1,1,0,1);
		positions[i+1] = Vector4(-1,-1,0,1);
		positions[i+2] = Vector4(1,-1,0,1);
		positions[i+3] = Vector4(1,1,0,1);
	}
	// attach the data to the vector in the order of the vertex attributes
	data.push_back(positions);
	data.push_back(velocities);		
	//data.push_back(offsets);	

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

void PS_Jet :: update(const float delta)
{
	m_currentTime += delta;

	//moving the jet with the player ship
	if (m_emitterShip != NULL) {
		CoordinateFrame cf = m_emitterShip->getCoordinateFrame();
		setTransform(cf); 
	} 

	float shipSpeed = 0.0f;
	if (m_emitterShip != NULL)  {
		shipSpeed = __max(abs(m_emitterShip->getVelocity().getZ()), abs(m_emitterShip->getVelocity().getX()));
		
		if (m_jetSpeed < shipSpeed)
			m_jetSpeed += 25;
		else 
			m_jetSpeed -= 25;

		if (m_jetSpeed < 0)
			m_jetSpeed = 0;
	}
}

void PS_Jet :: render() const
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
	ShaderManager::instance()->setUniform1fv("shipSpeed",&m_jetSpeed);
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

void PS_Jet :: reset()
{
	m_currentTime = 0.0f;
}