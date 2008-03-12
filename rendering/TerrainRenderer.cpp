//*****************************************************************************
//**
//**   File:               TerrainRenderer.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include <string>

#include "TerrainRenderer.h"
#include "../GameLogic/Objects/Terrain.h"
#include "../GameLogic/GameEvents.h"

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
#include "../gfx/Texture/Texture2D.h"
#include "../gfx/vbo/vbo.h"
#include "../Math/Vector4.h"
#include "../Math/Vector2.h"

using namespace std;

TerrainRenderer :: TerrainRenderer() :
	m_terrainModel(0),
	m_vbo(0),
	m_tformContribTex(0),
	m_heights(0)
{
	EventManager::instance().registerEventListener< Terrain_Changed >(this);
	EventManager::instance().registerEventListener< Level_Load >(this);
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
	if(m_heights)
	{
		MemMgrRaw::instance()->free(m_heights);
		m_heights = 0;
	}
	if(m_tformContribTex)
		delete m_tformContribTex;
}


void TerrainRenderer :: render(Graphics& g) const
{
	// Do some magic to render the terrain
	ShaderManager::instance()->begin("TerrainShader");
	m_terrainModel->matGroup(0).getTextureList()[0]->bind(0);
	ShaderManager::instance()->setUniform1i("texmap0",0);
	m_terrainModel->matGroup(0).getTextureList()[1]->bind(1);
	ShaderManager::instance()->setUniform1i("texmap1",1);
	m_tformContribTex->bind(2);
	ShaderManager::instance()->setUniform1i("contribMap",2);

	// FIXME : fetch it from m_terrain
	const float mapsize = 129.0f;
	ShaderManager::instance()->setUniform1fv("mapCellNum",&mapsize);

	m_terrainModel->matGroup(0).vboDesc().call();
	TextureMgr::instance()->setTextureUnit(2);
	TextureMgr::instance()->setBoundTexture(0,2);
	TextureMgr::instance()->setTextureUnit(1);
	TextureMgr::instance()->setBoundTexture(0,1);

	CHECK_GL_ERROR();

	// Draw the lake
	// FIXME : do it appropriately
	
	ShaderManager::instance()->end();
	Vector3 ll(-m_mapExtents.getX()*0.5f,0.0f,m_mapExtents.getX()*0.5f);
	Vector3 right(m_mapExtents.getX(),0,0);
	Vector3 up(0,0,-m_mapExtents.getX());
	m_lakeTexture->bind();
	RenderEngine::drawTexturedQuad(ll,right,up,Vector2(0,0),Vector2(10,10));
}


void TerrainRenderer :: onEvent(Level_Load& evt)
{
	const ParserSection* psRoot = evt.getValue1();
	const ParserSection* psGraphics = psRoot->getSection("Graphics");

	std::string gfxlevelfile = psGraphics->getVal("file");

	_clearResources();

	_loadResources(evt.getValue2(), gfxlevelfile);
}

void TerrainRenderer :: _loadResources(const std::string& id,
									   const std::string& gfxlevelfile)
{
	// Fetch the parser sections

	ConfParser parser(std::string("config/levels/") + gfxlevelfile);

	unsigned dimension;
	unsigned dataSize,indexSize;
	Vector4 ldir;

	// Open the file
	string filepath = ModelMgr::instance().getModelDir() + parser.getSection("DataFiles")->getVal("TerrainData");
	FILE * fp = fopen(filepath.c_str(),"rb");

	// Read the dimension & light direction
	fread(&dimension,sizeof(unsigned),1,fp);
	//dimension = FromString<unsigned>(parser.getSection("Misc")->getVal("MapCellDim"));
	ldir = FromString<Vector4>(parser.getSection("Misc")->getVal("LightDir"));

	// Set the GL Light 0
	glPushAttrib(GL_MATRIX_MODE);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glLightfv(GL_LIGHT0,GL_POSITION,ldir.cfp());
	glPopMatrix();
	glPopAttrib();

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
	fclose(fp);

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
	texvector.push_back(TextureIO::instance()->loadImage(parser.getSection("DataFiles")->getVal("BarrenTexture")));
	texvector.push_back(TextureIO::instance()->loadImage(parser.getSection("DataFiles")->getVal("TerraformTexture")));
	m_terrainModel->addMatGroup(MaterialGroup(Material(),
									 texvector,
									 VBODesc(m_vbo,vattrs,attribData,dataSize,indexData,indexSize,GL_TRIANGLE_STRIP),
									 -1));	

	// Get the heights & stuff for Terrain *, at the moment assume the scale is 1 & ymax = 100
	m_mapExtents = FromString<Vector3>(parser.getSection("Misc")->getVal("MapExtent"));
	m_heights = MemMgrRaw::instance()->allocate<float>(dimension*dimension);
	for(unsigned i=0;i<dataSize;++i)
		m_heights[i] = vertexData[i].getY();
	m_terrainDimension = dimension;
	

	// Free our data
	MemMgrRaw::instance()->free(vertexData);
	MemMgrRaw::instance()->free(texcoordData);
	MemMgrRaw::instance()->free(indexData);



	// TERRAFORM STUFF

	if(m_tformContribTex)
		delete m_tformContribTex;
	std::vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel(0,0));
	m_tformContribTex = new Texture2D(dimension,dimension,GL_RGB,GL_RGB,GL_UNSIGNED_BYTE,
									  ml,GL_TEXTURE_RECTANGLE_ARB,"Terraform contribution",false,true);
	m_tformFBO.Bind();
	m_tformFBO.UnattachAll();
	m_tformFBO.AttachTexture(m_tformContribTex->getTarget(),m_tformContribTex->getId(),GL_COLOR_ATTACHMENT0_EXT);
	bool res = m_tformFBO.IsValid();
	assert(res);
	FramebufferObject::Disable();


	// LAKE STUFF
	
	m_lakeTexture = TextureIO::instance()->getTexture("LakeTexture.dds");

	// TREE STUFF
	const string treefile = string("config/levels/") + parser.getSection("Misc")->getVal("TreePositions");
	const vector<string> treeModelNames = parser.getSection("Misc")->getValVector("TreeModels");
	fp = fopen(treefile.c_str(),"rb");
	unsigned treenum;
	fread(&treenum,sizeof(unsigned),1,fp);
	Vector4 * treepos = MemMgrRaw::instance()->allocate<Vector4>(treenum);
	fread(treepos,sizeof(Vector4),treenum,fp);
	fclose(fp);
	for(size_t i=0;i<treeModelNames.size();++i)
	{
		Model * m1 = ModelMgr::instance().getModel(treeModelNames[i] + string("_geom.obj"));
		Model * m2 = ModelMgr::instance().getModel(treeModelNames[i] + string("_tex.obj"));
		m_trees.push_back(ForestInfo_t(m1,m2));
	}
	for(unsigned i=0;i<treenum;++i)
	{
		const unsigned random_i = rand() % unsigned(m_trees.size());
		m_trees[random_i].trees.push_back(TreeInfo_t(Vector3(treepos[i].cfp())));
	}
	MemMgrRaw::instance()->free(treepos);


}

void TerrainRenderer :: onEvent(Terrain_Changed& evt)
{
	m_terrain = evt.getValue();
	m_terrain->fillData(m_heights,
						m_mapExtents.getX(),
						m_mapExtents.getY(),
						m_terrainDimension);	
	// Not allowed to do file loading here!
	// Keep that restricted to Level_Load please
	// If not possible at all: tell Joep :)
}

void TerrainRenderer :: addShadowCaster(const CoordinateModel& model)
{
	m_shadowCasters.push_back(model);
}

void TerrainRenderer :: removeShadowCaster(const CoordinateModel& model)
{
	std::vector<CoordinateModel>::iterator it = m_shadowCasters.begin();
	while(it != m_shadowCasters.end())
	{
		if(it->coordframe == model.coordframe)
			if(it->model == model.model)
			{
				m_shadowCasters.erase(it);
				break;
			}
	}
}