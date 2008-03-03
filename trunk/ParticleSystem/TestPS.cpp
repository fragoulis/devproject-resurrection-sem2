#include "TestPS.h"
#include "../gfx/Texture/TextureIO.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/Model.h"
#include "../Math/Vector4.h"
#include "../gfxutils/MemManager/MemMgr_RawData.h"
#include "../utility/RandomGenerator.h"


// TEMPLATE CREATION CTOR
TestPS :: TestPS(const std::string& name,
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
TestPS :: TestPS(const std::string& name,
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


PS_Base * TestPS :: clone() const
{
	return new TestPS(m_nameId,m_quadArray,m_particleSize,m_systemLife,m_particleLife,m_particleNum,
					 m_shaderIndex,m_usedAttribs);
}


void  TestPS :: _generateData(VBO * vbo,Texture * tex)
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

	// generate the vertex data.

	for(unsigned i=0; i<totalVData ;i+=4)
	{
		Vector3 vel;
		RandomGenerator::GET_RANDOM_VECTOR3(vel,Vector3(-1,-1,-1),Vector3(1,1,1));
		vel.normalize();
		velocities[i] = velocities[i+1] = velocities[i+2] = velocities[i+3] = Vector4(vel.getX(),
																					  vel.getY(),
																					  vel.getZ(),
																					  0.0f);

		positions[i] = Vector4(-1,1,0,1);
		positions[i+1] = Vector4(-1,-1,0,1);
		positions[i+2] = Vector4(1,-1,0,1);
		positions[i+3] = Vector4(1,1,0,1);
	}
	// attach the data to the vector in the order of the vertex attributes
	data.push_back(positions);
	data.push_back(velocities);		

	// generate the index data.
	for(unsigned i=0;i<totalIData;++i)
		indices[i] = i;

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

void TestPS :: update(const float delta)
{
	m_currentTime += delta;
}

void TestPS :: render() const
{
	// set uniforms, bind texture, transform & call VBO

	VAttribStatus curstatus = VBOMgr::instance().getCurrentFlags();
	VBOMgr::instance().setCurrentFlags(m_usedAttribs);

	glPushMatrix();
	glMultMatrixf(m_transform.getMatrix().cfp());
	ShaderManager::instance()->begin(m_shaderIndex);
	ShaderManager::instance()->setUniform1fv("currentTime",&m_currentTime);
	m_quadArray->getMatGroup()[0].getTextureList()[0]->bind(0);
	ShaderManager::instance()->setUniform1fv("particleTex",0);

	m_quadArray->getMatGroup()[0].getVboDesc().call();

	glPopMatrix();
	
	VBOMgr::instance().setCurrentFlags(curstatus);
}

void TestPS :: reset()
{
	m_currentTime = 0.0f;
}
