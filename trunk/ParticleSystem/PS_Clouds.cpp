#include "PS_Clouds.h"
#include "../gfx/Texture/TextureIO.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/Model.h"
#include "../Math/Vector4.h"
#include "../gfxutils/MemManager/MemMgr_RawData.h"
#include "../utility/RandomGenerator.h"
#include "../Rendering/RenderEngine.h"
#include "../GameLogic/GameLogic.h"


#include <iostream>


// TEMPLATE CREATION CTOR
PS_Clouds :: PS_Clouds(const std::string& name,
			VBO * vbo,
			const float psize,
			const float syslife,
			const float plife,
			const unsigned pnum,
			const int shindex,
			const std::string& texture,
			const std::string& noisetex)
:PS_Base(name,psize,syslife,plife,pnum,shindex,true)
{
	// Initialize
	_generateData(vbo,TextureIO::instance()->getTexture(texture),
					  TextureIO::instance()->getTexture(noisetex));
}

// TEMPLATE CREATION CTOR
PS_Clouds :: PS_Clouds(const std::string& name,
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


PS_Base * PS_Clouds :: clone() const
{
	return new PS_Clouds(m_nameId,m_quadArray,m_particleSize,m_systemLife,m_particleLife,m_particleNum,
					 m_shaderIndex,m_usedAttribs);
}


void  PS_Clouds :: _generateData(VBO * vbo,Texture * tex,Texture * tex2)
{
	const unsigned totalVData = m_particleNum*4;
	const unsigned totalIData = m_particleNum*6;

	// Get the VertexAttribute list
	std::vector<const VertexAttribute *> vattrs;
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib1"));
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("GenAttrib2"));

	// Set the attribute mask
	m_usedAttribs._raw[0] = 0;
	m_usedAttribs._raw[1] = 0;
	const unsigned pos_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[0]);
	const unsigned off_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[1]);
	const unsigned gen2_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[2]);
	m_usedAttribs.attrib[pos_index] = 0xFF;
	m_usedAttribs.attrib[off_index] = 0xFF;
	m_usedAttribs.attrib[gen2_index] = 0xFF;

	// Allocate the memory for data & indices
	std::vector<void *> data;
	unsigned * indices = MemMgrRaw::instance()->allocate<unsigned>(totalIData);
	Vector4 * positions = MemMgrRaw::instance()->allocate<Vector4>(totalVData);
	Vector4 * offsets = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib1
	Vector4 * gen2data = MemMgrRaw::instance()->allocate<Vector4>(totalVData);	// as GenAttrib2

	// generate the vertex data.

	for(unsigned i=0; i<totalVData ;i+=4)
	{
		
		float x = RandomGenerator::GET_RANDOM_FLOAT(-1.0f,1.0f),
			  y = RandomGenerator::GET_RANDOM_FLOAT(-1.0f,1.0f),
			  wind_eff = RandomGenerator::GET_RANDOM_FLOAT(0.1f,1.0f),
			  wind_dev = RandomGenerator::GET_RANDOM_FLOAT(-0.2f,0.2f);

		offsets[i] = offsets[i+1] = offsets[i+2] = offsets[i+3] = Vector4(x,
																		  wind_dev,
																		  y,
																		  wind_eff);

		float y_offset = (i / float(totalVData - 1)) - 0.5f;
		gen2data[i] = gen2data[i+1] = gen2data[i+2] = gen2data[i+3] = Vector4(y_offset,
																			  0.0f,
																			  0.0f,
																			  0.0f);

		//std::cout<<velocities[i].getW()<<std::endl;
		positions[i] = Vector4(-1,1,0,1);
		positions[i+1] = Vector4(-1,-1,0,1);
		positions[i+2] = Vector4(1,-1,0,1);
		positions[i+3] = Vector4(1,1,0,1);
	}
	// attach the data to the vector in the order of the vertex attributes
	data.push_back(positions);	
	data.push_back(offsets);	
	data.push_back(gen2data);	

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
	texlist.push_back(tex2);

	// & finally the model
	m_quadArray = new Model(m_nameId,vbo);
	m_quadArray->addMatGroup(MaterialGroup(Material(),texlist,vdesc,m_shaderIndex));

	// Free our data
	MemMgrRaw::instance()->free(positions);
	MemMgrRaw::instance()->free(indices);
	MemMgrRaw::instance()->free(offsets);
	MemMgrRaw::instance()->free(gen2data);
}

void PS_Clouds :: update(const float delta)
{
	m_currentTime += delta;

}

void PS_Clouds :: render() const
{
	// set uniforms, bind texture, transform & call VBO
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	CHECK_GL_ERROR();
	VAttribStatus curstatus = VBOMgr::instance().getCurrentFlags();
	VBOMgr::instance().setCurrentFlags(m_usedAttribs);

	glPushMatrix();
	const float * m = m_transform.getMatrix().cfp();
	glMultMatrixf(m);
	ShaderManager::instance()->begin(m_shaderIndex);
	ShaderManager::instance()->setUniform1fv("currentTime",&m_currentTime);
	m_quadArray->getMatGroup()[0].getTextureList()[0]->bind(0);
	ShaderManager::instance()->setUniform1i("particleTex",0);
	m_quadArray->getMatGroup()[0].getTextureList()[1]->bind(1);
	ShaderManager::instance()->setUniform1i("noiseTex",1);

	float windf[4];
	const Vector3 lvlwind(RenderEngine::instance().getLevelWind());
	const Vector3 yaxis(0.0f,1.0f,0.0f);
	Vector3 wind(lvlwind.getX(),0.0f,lvlwind.getY());
	windf[0] = wind.getX();
	windf[1] = wind.getY();
	windf[2] = wind.getZ();
	windf[3] = lvlwind.getZ();
	Vector3 wind_tan(wind);
	wind_tan.cross(yaxis);
	wind_tan.normalize();

	ShaderManager::instance()->setUniform4fv("wind",windf);
	ShaderManager::instance()->setUniform3fv("wind_tan",wind_tan.cfp());
	const float map_size = RenderEngine::instance().getLevelExtents().getX()*0.5f;
	ShaderManager::instance()->setUniform1fv("map_width",&map_size);
	const float plane_height = GameLogic::instance().getGamePlaneHeight();
	ShaderManager::instance()->setUniform1fv("plane_height",&plane_height);

	const float height_scale = 60.0f;
	ShaderManager::instance()->setUniform1fv("height_scale",&height_scale);

	Vector4 lightpos = RenderEngine::instance().getLevelLight();
	Vector4 lightcol = RenderEngine::instance().getLevelLightColor();

	ShaderManager::instance()->setUniform4fv("lightPosition",lightpos.cfp());
	ShaderManager::instance()->setUniform4fv("lightColor",lightcol.cfp());

	m_quadArray->getMatGroup()[0].getVboDesc().call();

	glPopMatrix();
	
	VBOMgr::instance().setCurrentFlags(curstatus);
	ShaderManager::instance()->end();
	CHECK_GL_ERROR();

	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
}

void PS_Clouds :: reset()
{
	m_currentTime = 0.0f;
}
