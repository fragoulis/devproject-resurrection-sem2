#include <string>

#include "TerrainRenderer.h"
#include "../GameLogic/Terrain.h"

#include "../rendering/RenderEngine.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../gfxutils/MemManager/MemMgr_RawData.h"
#include "../gfx/Model/ModelMgr.h"
#include "../gfx/Texture/TextureIO.h"
#include "../gfx/Texture/Texture.h"
#include "../gfx/Shaders/VertexAttrib.h"
#include "../gfx/Shaders/ShaderManager.h"
#include "../gfx/Model/Model.h"
#include "../gfx/Model/VBODesc.h"
#include "../gfx/vbo/vbo.h"
#include "../Math/Vector4.h"
#include "../Math/Vector2.h"

using namespace std;

TerrainRenderer :: TerrainRenderer()
:m_terrainModel(0),
m_vbo(0)
{
	EventManager::instance().registerEventListener(this);
}

TerrainRenderer :: ~TerrainRenderer()
{
	_clearResources();
}

void TerrainRenderer :: _clearResources()
{
	if(m_terrainModel)
	{
		const std::string texname =  m_terrainModel->matGroup(0).getTextureList()[0]->getName();
		TextureIO::instance()->deleteTexture(texname);
		delete m_terrainModel;
		m_terrainModel = 0;
	}
	if(m_vbo)
	{
		delete m_vbo;
		m_vbo = 0;
	}
}


void TerrainRenderer :: render(Graphics& g) const
{
	// Do some magic to render the terrain
	m_terrainModel->matGroup(0).getTextureList()[0]->bind();
	m_terrainModel->matGroup(0).vboDesc().call();
	CHECK_GL_ERROR();
}

void TerrainRenderer :: onEvent(Terrain_Changed& tc)
{
	m_terrain = tc.getData();
	const std::string& id = m_terrain->getID();

	// Erase the old data

	_clearResources();

	// Fetch the parser sections

	const ParserSection * parsecLevel = RenderEngine::instance().getParserSection(string("Level:") + id);
	const ParserSection * parsecLevelTerrain = parsecLevel->getChild("Terrain");

	unsigned dimension;
	unsigned dataSize,indexSize;
	Vector4 ldir;

	// Open the file
	string filepath = ModelMgr::instance().getModelDir() + parsecLevelTerrain->getVal("Data");
	FILE * fp = fopen(filepath.c_str(),"rb");

	// Read the dimension
	fread(&dimension,sizeof(unsigned),1,fp);

	// Read the light direction
	fread(&ldir,sizeof(Vector4),1,fp);

	// Compute the index - data sizes
	dataSize = dimension * dimension;
	indexSize = 2*(dimension + 1)*(dimension - 1);		// tristrip index formula

	// Read the rest data ( vertex - texcoord - indices )
	Vector4 * vertexData = MemMgrRaw::instance()->allocate<Vector4>(dataSize);
	Vector2 * texcoordData = MemMgrRaw::instance()->allocate<Vector2>(dataSize);
	unsigned * indexData = MemMgrRaw::instance()->allocate<unsigned>(indexSize);

	fread(vertexData,sizeof(Vector4),dataSize,fp);
	fread(texcoordData,sizeof(Vector2),dataSize,fp);
	fread(indexData,sizeof(unsigned),indexSize,fp);

	// Check index integrity (we want all in range of 0-indexSize)
	/*
	unsigned max=0;
	for(unsigned i=0;i<indexSize;++i)
		max = MAX(indexData[i],max);
	assert(max < dataSize);
	*/

	std::vector<void *> attribData;
	attribData.push_back((void *)vertexData);
	attribData.push_back((void *)texcoordData);
	
	// - We need only "Vertex" & "Texcoord" Attributes
	std::vector<const VertexAttribute *> vattrs;
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
	vattrs.push_back(ShaderManager::instance()->vertexAttribute("Texcoord"));

	// Create the vbo
	m_vbo = new VBO(vattrs,				// Specify attribs
					dataSize,			// now we don't need any more space for vertex data in this vbo
					indexSize,		// now we don't need any more space for index data in this vbo
					GL_STATIC_DRAW,		// usage hint, we won't change VBO contents
					"UselessString");	// This is a crappy classification attempt & will probably be removed

	m_terrainModel = new Model(string("Terrain_")+id,m_vbo);

	std::vector<Texture *> texvector;
	texvector.push_back(TextureIO::instance()->loadImage(parsecLevelTerrain->getVal("Texture")));
	m_terrainModel->addMatGroup(MaterialGroup(Material(),
									 texvector,
									 VBODesc(m_vbo,vattrs,attribData,dataSize,indexData,indexSize,GL_TRIANGLE_STRIP),
									 -1));	

	// Get the heights & stuff for Terrain *, at the moment assume the scale is 1 & ymax = 100
	float * heights = MemMgrRaw::instance()->allocate<float>(dimension*dimension);
	for(unsigned i=0;i<dataSize;++i)
		heights[i] = vertexData[i].getY();
	m_terrain->fillData(heights,
						1.0f,
						100.0f,
						dimension);	

	// Free our data
	MemMgrRaw::instance()->free(vertexData);
	MemMgrRaw::instance()->free(texcoordData);
	MemMgrRaw::instance()->free(indexData);


	// set a camera at the center of the terrain looking down
}