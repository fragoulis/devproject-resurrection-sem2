//*****************************************************************************
//**
//**   File:               TerrainRenderer.cpp
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-19 20:39:19 +0000 (Wed, 19 Mar 2008) $
//**   Last Revision:      $Revision: 200 $
//**
//*****************************************************************************

#include <string>

#include "TerrainRenderer.h"
#include "../GameLogic/Objects/Terrain.h"
#include "../GameLogic/Objects/Crater.h"
#include "../GameLogic/GameEvents.h"
#include "../GameLogic/GameLogic.h"

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
#include "../gfxutils/Misc/Logger.h"

#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Objects/Ebomb.h"
#include "../GameLogic/Objects/Crater.h"
#include "../gfx/Camera.h"
#include "ShipRenderer.h"
#include "LaserRenderer.h"

using namespace std;

TerrainRenderer :: TerrainRenderer() :
	m_terrainModel(0),
	m_vbo(0),
	m_tformContribTex(0),
	m_heights(0),
	m_shadowTexture(0),
	m_lakeTimer(0.0f),
	m_heightTexture(0),
	m_lakeReflection(0)
{
	EventManager::instance().registerEventListener< Terrain_Changed >(this);
	EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Despawned >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);
	EventManager::instance().registerEventListener< Ebomb_Spawned >(this);
	EventManager::instance().registerEventListener< Ebomb_Despawned >(this);
	EventManager::instance().registerEventListener< Crater_Spawned >(this);
	EventManager::instance().registerEventListener< Crater_Despawned >(this);
	EventManager::instance().registerEventListener< Life_Restored >(this);

	EventManager::instance().registerEventListener<Key_GoingDown>(this); //DEBUG PURPOSES

	m_craterArrowTexture = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getCraterArrowTexture());
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
	{
		delete m_tformContribTex;
		m_tformContribTex = 0;
	}
	if(m_shadowTexture)
	{
		delete m_shadowTexture;
		m_shadowTexture = 0;
	}
	if(m_heightTexture)
	{
		delete m_heightTexture;
		m_heightTexture = 0;
	}
	if(m_lakeReflection)
	{
		delete m_lakeReflection;
		m_lakeReflection = 0;
	}
}


void TerrainRenderer :: render(Graphics& g) const
{
	_renderShadows();
	_updateTerraformContribution();

	/*
		fetch terrain texcoords & scaling
	*/
	Point3 pts[4];
	RenderEngine::instance().getWsScreenEdges(pts);
	Point3 ll(pts[0]);
	Point3 lr(pts[1]);
	Point3 tl(pts[3]);
	Point3 tr(pts[2]);
	Vector3 center = (pts[0].getVector() + pts[1].getVector() + pts[3].getVector() + pts[2].getVector())*0.25f;
	Vector2 extents;
	
	extents.setX(max(max(max(lr.getX() - center.getX(),
								  tr.getX() - center.getX()),
							  center.getX() - tl.getX()),
						  center.getX() - ll.getX()));
	extents.setY(max(max(max(lr.getZ() - center.getZ(),
								  ll.getZ() - center.getZ()),
							  center.getZ() - tr.getZ()),
						  center.getZ() - tl.getZ()));
	extents.add(abs(center.getX()),abs(center.getZ()));

	// compute the tex scale
	Vector2 terrain_tex_scale(0.5f*m_mapExtents.getX() / extents.getX(),
							  0.5f*m_mapExtents.getZ() / extents.getY());
	//compute the tex offset
	Vector2 terrain_tex_offset;
	{
		Vector2 mapExtents(m_mapExtents.getX(),m_mapExtents.getZ());
		Vector2 ll_mapEdge(-mapExtents * 0.5f);
		Vector2 ll_screenEdge(center.getX() - extents.getX(),
							  -center.getZ() - extents.getY());
		Vector2 offset_to_ur(ll_screenEdge - ll_mapEdge);
		terrain_tex_offset.setX(terrain_tex_scale.getX()*(offset_to_ur.getX()  / mapExtents.getX()));
		terrain_tex_offset.setY(terrain_tex_scale.getY()*(offset_to_ur.getY()  / mapExtents.getY()));
	}

	// Do some magic to render the terrain
	ShaderManager::instance()->begin("TerrainShader");
	m_terrainModel->matGroup(0).getTextureList()[0]->bind(14);
	ShaderManager::instance()->setUniform1i("texmap0",14);
	m_terrainModel->matGroup(0).getTextureList()[1]->bind(15);
	ShaderManager::instance()->setUniform1i("texmap1",15);
	m_tformContribTex->bind(13);
	ShaderManager::instance()->setUniform1i("contribMap",0);
	m_shadowTexture->bind(12);
	ShaderManager::instance()->setUniform1i("shadowTex",12);
	ShaderManager::instance()->setUniform2fv("terrain_tex_scale", terrain_tex_scale.cfp());
	ShaderManager::instance()->setUniform2fv("terrain_tex_offset", terrain_tex_offset.cfp());

	const float mapsize = float(m_terrain->getTerrainDim());
	ShaderManager::instance()->setUniform1fv("mapCellNum",&mapsize);

	m_terrainModel->matGroup(0).vboDesc().call();

	CHECK_GL_ERROR();

	
	// create the reflection Surface
	_drawLakeReflection(g);

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// Draw the lake
	
	ShaderManager::instance()->begin("lakeShader");
	m_lakeTexture->bind(11);
	ShaderManager::instance()->setUniform1i("noiseTex",11);
	m_shadowTexture->bind(12);
	ShaderManager::instance()->setUniform1i("shadowTex",12);
	m_lakeReflection->bind(10);
	ShaderManager::instance()->setUniform1i("reflTex",10);
	m_heightTexture->bind(9);
	ShaderManager::instance()->setUniform1i("heightTex",9);

	ShaderManager::instance()->setUniform1fv("timer",&m_lakeTimer);
	
	const float ws = RenderEngine::instance().getConstRenderSettings().getWaveSpeed();
	const float wcr = RenderEngine::instance().getConstRenderSettings().getWaveChangeRate();
	const float wr = RenderEngine::instance().getConstRenderSettings().getWaveRepeats(m_mapExtents.getX());

	ShaderManager::instance()->setUniform1fv("timer",&m_lakeTimer);
	ShaderManager::instance()->setUniform1fv("waveChangeRate",&wcr);
	ShaderManager::instance()->setUniform1fv("waveSpeed",&ws);
	ShaderManager::instance()->setUniform4fv("lightColor",m_lightColor.cfp());
	ShaderManager::instance()->setUniform4fv("waterColor",m_waterColor.cfp());
	Vector4 lightPos(m_lightDir.getX(),m_lightDir.getY(),m_lightDir.getZ(),0.0f);
	ShaderManager::instance()->setUniform4fv("lightPosition",lightPos.cfp());

	// compute the texcoord offsets & scale, based on the ship's center, max extents etc.
	Vector2 rto;
	Vector2 rts;
	float proj[4];
	m_cameraRef->getProjSettings(proj[0],proj[1],proj[2],proj[3]);
	float edge_x  = (RenderEngine::instance().getCameraHeightAbovePlane() + GameLogic::instance().getGamePlaneHeight())*proj[0] / proj[2];
	float edge_y  = (RenderEngine::instance().getCameraHeightAbovePlane() + GameLogic::instance().getGamePlaneHeight())*proj[1] / proj[2];

	// compute reflection texture scale
	rts.setX( 1.0f / (edge_x * 2.0f / m_mapExtents.getX()));
	rts.setY( 1.0f / (edge_y * 2.0f / m_mapExtents.getZ()));

	float off[2] = {m_cameraRef->getEye().getX(),m_cameraRef->getEye().getZ()};
	
	// compute reflection texture offset
	
	Vector2 mapExtents(m_mapExtents.getX(),m_mapExtents.getZ());
	Vector2 ll_mapEdge(-mapExtents * 0.5f);
	Vector2 curpos(off[0],-off[1]);
	Vector2 ll_screenEdge(curpos - Vector2(edge_x,edge_y));
	Vector2 offset_to_ur(ll_screenEdge - ll_mapEdge);
	rto.setX(rts.getX()*(offset_to_ur.getX()  / mapExtents.getX()));
	rto.setY(rts.getY()*(offset_to_ur.getY()  / mapExtents.getY()));

	//ShaderManager::instance()->setUniform2fv("reflectionTexCoordOffsets",rto.cfp());
	//ShaderManager::instance()->setUniform2fv("reflectionTexCoordScale",rts.cfp());
	ShaderManager::instance()->setUniform2fv("terrain_tex_scale", terrain_tex_scale.cfp());
	ShaderManager::instance()->setUniform2fv("terrain_tex_offset", terrain_tex_offset.cfp());

	ShaderManager::instance()->setUniform1fv("waveRepeats",&wr);

	Vector3 qll(-m_mapExtents.getX()*0.5f,0.0f,m_mapExtents.getX()*0.5f);
	Vector3 qright(m_mapExtents.getX(),0,0);
	Vector3 qup(0,0,-m_mapExtents.getX());
	RenderEngine::drawTexturedQuad(qll,qright,qup,Vector2(0,0),Vector2(1.0f,1.0f));

	
	// Draw the crater arrows

	const float cr_aty = GameLogic::instance().getGamePlaneHeight();
	const float crwidth = 50.0f;
	const float crheight = 100.0f;

	ShaderManager::instance()->begin("craterArrowShader");
	m_craterArrowTexture->bind(0);
	ShaderManager::instance()->setUniform1i("tex",0);

	for(std::vector<CraterInfo_t>::const_iterator it = m_craterList.begin();
		it != m_craterList.end();
		++it)
	{
		ShaderManager::instance()->setUniform4fv("color",it->color.cfp());
		Vector3 ll(it->crater->getPosition().getVector());
		ll.setY(cr_aty);
		RenderEngine::drawTexturedQuad(ll - Vector3(crwidth * 0.5f,crheight * 0.5f,0.0f),
									   Vector3(crwidth,0.0f,0.0f),
									   Vector3(0.0f,crheight,0.0f),
									   Vector2(0.0f,0.0f),
									   Vector2(1.0f,1.0f));
	}
	glDisable(GL_BLEND);

	// FIXME : DRAW A THICKLINE QUAD AT THE EDGES FOR DEBUG
/*
	float proj[4];
	m_cameraRef->getProjSettings(proj[0],proj[1],proj[2],proj[3]);
	glLineWidth(13.0f);
	float edge_x  = 0.55f*(1500.0f + GameLogic::instance().getGamePlaneHeight())*proj[0] / proj[2];
	float edge_y  = 0.55f*(1500.0f + GameLogic::instance().getGamePlaneHeight())*proj[1] / proj[2];
	float off[2] = {m_cameraRef->getEye().getX(),m_cameraRef->getEye().getZ()};
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_LINE_LOOP);
		glVertex3f(-edge_x+off[0],0.0f,edge_y+off[1]);
		glVertex3f(edge_x+off[0],0.0f,edge_y+off[1]);
		glVertex3f(edge_x+off[0],0.0f,-edge_y+off[1]);
		glVertex3f(-edge_x+off[0],0.0f,-edge_y+off[1]);
	glEnd();
	glEnable(GL_DEPTH_TEST);
*/
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
	RenderEngine::instance().setLevelLight(ldir);

	m_lightColor = FromString<Vector4>(parser.getSection("Misc")->getVal("LightAmbDiff"));
	RenderEngine::instance().setLevelLightColor(m_lightColor);

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

	vector<MipmapLevel> mlh;
	mlh.push_back(MipmapLevel(m_heights,0));
	m_heightTexture = new Texture2D(dimension,dimension,GL_LUMINANCE16F_ARB,GL_LUMINANCE,GL_FLOAT,mlh,
									GL_TEXTURE_2D,"Heights",false,false);
	m_heightTexture->setParam(GL_TEXTURE_WRAP_S,GL_CLAMP);
	m_heightTexture->setParam(GL_TEXTURE_WRAP_T,GL_CLAMP);
	

	// Free our data
	MemMgrRaw::instance()->free(vertexData);
	MemMgrRaw::instance()->free(texcoordData);
	MemMgrRaw::instance()->free(indexData);



	// TERRAFORM STUFF

	if(m_tformContribTex)
		delete m_tformContribTex;
	std::vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel(0,0));
	m_tformContribTex = new Texture2D(dimension,dimension,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE,
									  ml,GL_TEXTURE_RECTANGLE_ARB,"Terraform contribution",false,true);
	m_tformFBO.Bind();
	m_tformFBO.UnattachAll();
	m_tformFBO.AttachTexture(m_tformContribTex->getTarget(),m_tformContribTex->getId(),GL_COLOR_ATTACHMENT0_EXT);
	bool res = m_tformFBO.IsValid();
	assert(res);
	FramebufferObject::Disable();


	// LAKE STUFF
	
	m_lakeTexture = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getLakeTexture());

	// get the reflection tex size
	int vp[4];
	RenderEngine::instance().getViewport(vp);
	const float reflDimRatio = RenderEngine::instance().getConstRenderSettings().getReflectionTextureScreenRatio();
	unsigned reflTexSize[2] = {unsigned(reflDimRatio*vp[2]),
							   unsigned(reflDimRatio*vp[3])};
	// be sure that it is a mult of 2
	reflTexSize[0] += reflTexSize[0]%2;
	reflTexSize[1] += reflTexSize[1]%2;

	// FIXME : get the REAL dims of the screen - or get a lesser one dep on opts
	m_lakeReflection = new Texture2D(reflTexSize[0],reflTexSize[1],GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"lake reflection",false,false);
	m_lakeReflection->setParam(GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	m_lakeReflection->setParam(GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	m_reflectionDepthBuffer.Bind();
	m_reflectionDepthBuffer.Set(GL_DEPTH_COMPONENT32,reflTexSize[0],reflTexSize[1]);
	m_reflectionFBO.Bind();
	m_reflectionFBO.AttachTexture(GL_TEXTURE_2D,m_lakeReflection->getId(),GL_COLOR_ATTACHMENT0_EXT);
	m_reflectionFBO.AttachRenderBuffer(m_reflectionDepthBuffer.GetId(),GL_DEPTH_ATTACHMENT_EXT);
	m_reflectionFBO.IsValid();
	FramebufferObject::Disable();

	// Get water color
	m_waterColor = FromString<Vector4>(parser.getSection("Misc")->getVal("WaterColor"));

	// SHADOW STUFF
	_initShadows(ldir);


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

void TerrainRenderer :: _addShadowCaster(const CoordinateModel& model)
{
	m_shadowCasters.push_back(model);
}

void TerrainRenderer :: _removeShadowCaster(const CoordinateFrame * cf)
{
	std::vector<CoordinateModel>::iterator it = m_shadowCasters.begin();
	while(it != m_shadowCasters.end())
	{
		if(it->coordframe == cf)
		{
			m_shadowCasters.erase(it);
			break;
		}
		++it;
	}
}


void TerrainRenderer :: _updateTerraform(const float dt)
{
	const float tpu = RenderEngine::instance().getConstRenderSettings().getTerraformingTimePerUnit();
	// For every active terraform process, add the dt
	for(size_t i=0; i< m_tformInfo.size();++i)
	{
		m_tformInfo[i].currentTimeOffset += dt;
		if(m_tformInfo[i].currentTimeOffset > (tpu*m_tformInfo[i].radius))
		{
			m_tformInfo.erase(m_tformInfo.begin() + i);
			--i;
		}
	}
	//CKLOG(string("ActiveTerraforms : ") + ToString<unsigned>(unsigned(m_tformInfo.size())),2);

}

void TerrainRenderer :: _updateTerraformContribution() const
{
	// save settings
	// bind the FBO
	// make settings
	// bind shader & texture
	// for every active terraform , run the shader in the appropriate quad ( for starters the whole map)
	// the shader makes the terraformed cells white
	// restore settings , unbind FBO

	// save draw buffer * viewport
	int curdrawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&curdrawbuf);
	int vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	const unsigned dim = m_terrain->getTerrainDim();
	
	// Change viewport - bind fbo - set draw buffer
	glViewport(0,0,dim,dim);
	m_tformFBO.Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	// push & alter transforms
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-m_mapExtents.getX()*0.5f,
			m_mapExtents.getX()*0.5f,
			-m_mapExtents.getZ()*0.5f,
			m_mapExtents.getZ()*0.5f,
			-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// bind shader & uniforms & runs the shader
	Vector3 ll(-m_mapExtents.getX()*0.5f,-m_mapExtents.getZ()*0.5f,0.0f);
	Vector3 up(0.0f,m_mapExtents.getZ(),0.0f);
	Vector3 right(m_mapExtents.getX(),0.0f,0.0f);

	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	ShaderManager::instance()->begin("terraformShader");

	const float tpu = RenderEngine::instance().getConstRenderSettings().getTerraformingTimePerUnit();

	for(size_t i=0;i< m_tformInfo.size();++i)
	{
		const float timepcent = m_tformInfo[i].currentTimeOffset / (tpu*m_tformInfo[0].radius);
		Vector2 v(m_tformInfo[i].center.getX(),-m_tformInfo[i].center.getZ());
		ShaderManager::instance()->setUniform1fv("timePercent", &timepcent);
		ShaderManager::instance()->setUniform2fv("center",v.cfp());
		const float sqradius = m_tformInfo[i].radius*m_tformInfo[i].radius;
		ShaderManager::instance()->setUniform1fv("sqradius",&sqradius);

		RenderEngine::drawQuad(ll,right,up);
	}

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	// disable fbo - restore buffer & viewport
	FramebufferObject::Disable();
	glDrawBuffer(curdrawbuf);
	glViewport(vp[0],vp[1],vp[2],vp[3]);

	// restore matrix states
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void TerrainRenderer :: onEvent(Player_Spawned& evt)
{
	// Get the const render settings of the ship
	const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	Model * model = ModelMgr::instance().getModel(settings.modelName);
	_addShadowCaster(CoordinateModel(model,cf));
}

void TerrainRenderer :: onEvent(Enemy_Spawned& evt)
{
	// Get the const render settings of the ship
	const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	Model * model = ModelMgr::instance().getModel(settings.modelName);
	_addShadowCaster(CoordinateModel(model,cf));
}

void TerrainRenderer :: onEvent(Enemy_Despawned& evt)
{
	// Fetch the enemy & remove
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	_removeShadowCaster(cf);
}

void TerrainRenderer :: onEvent(Player_Despawned& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	_removeShadowCaster(cf);
}

void TerrainRenderer :: onEvent(Ebomb_Spawned& evt)
{
	// Get the const render settings of the ship
	const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	Model * model = ModelMgr::instance().getModel(settings.modelName);
	_addShadowCaster(CoordinateModel(model,cf));
}

void TerrainRenderer :: onEvent(Ebomb_Despawned& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	_removeShadowCaster(cf);
}


void TerrainRenderer :: _renderShadows() const
{
	int drawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&drawbuf);
	int vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	const unsigned ts = RenderEngine::instance().getConstRenderSettings().getShadowTextureSize();
	glViewport(0,0,ts,ts);
	VAttribStatus vstatus = VBOMgr::instance().getCurrentFlags();
	m_shadowFBO.Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	/*
		Get the info from renderEngine : 
	*/
	
	Point3 pts[4];
	RenderEngine::instance().getWsScreenEdges(pts);
	Point3 ll(pts[0]);
	Point3 lr(pts[1]);
	Point3 tl(pts[3]);
	Point3 tr(pts[2]);

	Vector3 center = (ll.getVector() + lr.getVector() + tl.getVector() + tr.getVector())*0.25f;
	Vector2 extents;
	
	extents.setX(max(max(max(lr.getX() - center.getX(),
								  tr.getX() - center.getX()),
							  center.getX() - tl.getX()),
						  center.getX() - ll.getX()));
	extents.setY(max(max(max(lr.getZ() - center.getZ(),
								  ll.getZ() - center.getZ()),
							  center.getZ() - tr.getZ()),
						  center.getZ() - tl.getZ()));
	extents.add(abs(center.getX()),abs(center.getZ()));

	float proj[4];
	m_cameraRef->getProjSettings(proj[0],proj[1],proj[2],proj[3]);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-extents.getX(),
			extents.getX(),
			-extents.getY(),
			extents.getY(),
			proj[2],
			proj[3]);		// set our proj to look at just the extents we need
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(center.getX(),
			  center.getY() + RenderEngine::instance().getCameraHeightAbovePlane(),
			  center.getZ(),
			  center.getX(),
			  center.getY(),
			  center.getZ(),
			  0.0f,
			  0.0f,
			  -1.0f);	// set mview as a top down view,from a really high point, at the center

	glClear(GL_COLOR_BUFFER_BIT);
	ShaderManager::instance()->begin("ShadowWrite2");
	const Vector3 ldir(m_lightDir);

	float flat[16] = {1 , 0,0,0,
					-ldir.getX() / ldir.getY(), 1, -ldir.getZ() / ldir.getY(), 0.0, 
					0.0,0.0,1.0 , 0.0, 
					0.0,0.0,0.0,1.0};

	glMultMatrixf(flat);

	VAttribStatus vonly;
	const VertexAttribute * vattr = ShaderManager::instance()->vertexAttribute("Vertex");
	vonly.attrib[ShaderManager::instance()->vertexAttributeIndex(vattr)] = 0xFF;
	VBOMgr::instance().setCurrentFlags(vonly);

	
	for(std::vector<CoordinateModel>::const_iterator it = m_shadowCasters.begin();
		it != m_shadowCasters.end();
		++it)
	{
		glPushMatrix();
		glMultMatrixf(it->coordframe->getMatrix().cfp());
		for(size_t i=0;i<it->model->getMatGroup().size();++i)
			it->model->getMatGroup()[i].getVboDesc().call();
		glPopMatrix();
	}

	VBOMgr::instance().setCurrentFlags(vstatus);

	// Restore stuff

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	FramebufferObject::Disable();
	glDrawBuffer(drawbuf);
	glViewport(vp[0],vp[1],vp[2],vp[3]);
}

void TerrainRenderer :: _initShadows(const Vector4& lightdir)
{
	m_lightDir.set(-lightdir.getX(),-lightdir.getY(),-lightdir.getZ());
	// initialize the texture & FBO
	const unsigned ts = RenderEngine::instance().getConstRenderSettings().getShadowTextureSize();
	std::vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel(0,0));
	m_shadowFBO.Bind();
	m_shadowTexture = new Texture2D(ts,ts,GL_RGB,GL_RGB,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"Shadow",false,false);
	//glGenerateMipmapEXT(GL_TEXTURE_2D);
	m_shadowTexture->setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	m_shadowTexture->setParam(GL_TEXTURE_WRAP_S,GL_CLAMP);
	m_shadowTexture->setParam(GL_TEXTURE_WRAP_T,GL_CLAMP);
	m_shadowFBO.AttachTexture(GL_TEXTURE_2D,m_shadowTexture->getId(),GL_COLOR_ATTACHMENT0_EXT);
	m_shadowFBO.IsValid();
	FramebufferObject::Disable();
}

void TerrainRenderer :: update(const float dt)
{
	// Also update trees & contrib tex
	_updateTerraform(dt);
	m_lakeTimer += dt;
}

void TerrainRenderer::onEvent(Key_GoingDown &key) 
{
	int keyPressed = key.getValue();

	switch (keyPressed) {
		case 'M':
			{
			TerraformInfo_t tfi;
			tfi.center = Vector3(0.0f,0.0f,0.0f);
			tfi.radius = 1024;
			tfi.currentTimeOffset = 0.0f;
			m_tformInfo.push_back(tfi);
			}
			break;
		case 'N':
			{
			TerraformInfo_t tfi;
			tfi.center = Vector3(-150.0f,0.0f,-150.0f);
			tfi.radius = 500;
			tfi.currentTimeOffset = 0.0f;
			m_tformInfo.push_back(tfi);
			}
			break;
	}
}

void TerrainRenderer::_drawLakeReflection(Graphics& g) const
{
	// get the reflection tex size
	int vp[4];
	RenderEngine::instance().getViewport(vp);
	const float reflDimRatio = RenderEngine::instance().getConstRenderSettings().getReflectionTextureScreenRatio();
	unsigned reflTexSize[2] = {unsigned(reflDimRatio*vp[2]),
							   unsigned(reflDimRatio*vp[3])};
	// be sure that it is a mult of 2
	reflTexSize[0] += reflTexSize[0]%2;
	reflTexSize[1] += reflTexSize[1]%2;

	// save state
	int drawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&drawbuf);
	glViewport(0,0,reflTexSize[0],reflTexSize[1]);

	// bind & set draw buffer
	m_reflectionFBO.Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float gph = GameLogic::instance().getGamePlaneHeight();

	// compute the screen quad

	Point3 pts[4];
	RenderEngine::instance().getWsScreenEdges(pts);
	Point3 ll(pts[0]);
	Point3 lr(pts[1]);
	Point3 tl(pts[3]);
	Point3 tr(pts[2]);

	Vector3 center = (ll.getVector() + lr.getVector() + tl.getVector() + tr.getVector())*0.25f;
	Vector2 extents;
	
	extents.setX(max(max(max(lr.getX() - center.getX(),
								  tr.getX() - center.getX()),
							  center.getX() - tl.getX()),
						  center.getX() - ll.getX()));
	extents.setY(max(max(max(lr.getZ() - center.getZ(),
								  ll.getZ() - center.getZ()),
							  center.getZ() - tr.getZ()),
						  center.getZ() - tl.getZ()));
	extents.add(abs(center.getX()),abs(center.getZ()));

	// Fix the camera

	float proj[4];
	m_cameraRef->getProjSettings(proj[0],proj[1],proj[2],proj[3]);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(-extents.getX(),
			extents.getX(),
			-extents.getY(),
			extents.getY(),
			proj[2],
			proj[3]);		// set our proj to look at just the extents we need
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(center.getX(),
			  center.getY() + RenderEngine::instance().getCameraHeightAbovePlane(),
			  center.getZ(),
			  center.getX(),
			  center.getY(),
			  center.getZ(),
			  0.0f,
			  0.0f,
			  -1.0f);	// set mview as a top down view,from a really high point, at the center

	
	m_laserRendererRef->render(g);

	glScalef(1.0f,-1.0f,1.0f);

	//RenderEngine::instance().setLevelLight(-RenderEngine::instance().getLevelLight());

	// draw ships
	m_shipRendererRef->render(g);

	//RenderEngine::instance().setLevelLight(-RenderEngine::instance().getLevelLight());

	
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	//restore settings
	FramebufferObject::Disable();
	glDrawBuffer(drawbuf);
	glViewport(vp[0],vp[1],vp[2],vp[3]);
	
}


void TerrainRenderer::onEvent(Crater_Spawned& evt)
{
	Vector4 cratercolor;
	switch(evt.getValue()->getEbombType())
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
	m_craterList.push_back(CraterInfo_t(evt.getValue(),cratercolor));
}

void TerrainRenderer::onEvent(Crater_Despawned& evt)
{
	for(std::vector<CraterInfo_t>::iterator it = m_craterList.begin();
		it != m_craterList.end();
		++it)
	{
		if(it->crater == evt.getValue())
		{
			m_craterList.erase(it);
			break;
		}
	}
}

void TerrainRenderer::onEvent(Life_Restored& evt)
{
	Crater* crater = evt.getValue();

	for(std::vector<CraterInfo_t>::iterator it = m_craterList.begin();
		it != m_craterList.end();
		++it)
	{
		if (it->crater == crater)
		{
			m_craterList.erase(it);
			break;
		}
	}

	TerraformInfo_t tfi;
	tfi.center = crater->getPosition().getVector();
	tfi.radius = crater->getAffectedAreaRadius();
	tfi.currentTimeOffset = 0.0f;
	m_tformInfo.push_back(tfi);
}