#include <string>

#include "TerrainRenderer.h"
#include "../GameLogic/Objects/Terrain.h"
#include "../GameLogic/Objects/Crater.h"
#include "../GameLogic/GameEvents.h"
#include "../GameLogic/GameLogic.h"
#include "../utility/EventManager.h"

#include "../rendering/RenderEngine.h"
#include "../gfxutils/ConfParser/ParserSection.h"
#include "../Math/Vector4.h"
#include "../Math/Vector2.h"
#include "../gfxutils/Misc/Logger.h"
#include "../gfxutils/Misc/utils.h"

#include "../GameLogic/Enemies/Enemyship.h"
#include "../GameLogic/Objects/Playership.h"
#include "../GameLogic/Objects/Ebomb.h"
#include "../GameLogic/Objects/Crater.h"
//#include "../ParticleSystem/PS_Base.h"
//#include "../ParticleSystem/PS_Manager.h"
#include "../gfx/Camera.h"
//#include "ShipRenderer.h"
//#include "LaserRenderer.h"

#include "../control/Gamecube/gc_new_delete.h"
#include "Terrain/TerrainLoader.h"
#include "Terrain/TerrainStruct.h"
#include "../gfxutils/Texture/TextureMgr.h"
#include "../gfxutils/Texture/TplPalette.h"
#include "../gfxutils/Texture/Texture.h"
#include "../gfxutils/Model/ModelMgr.h"
#include "Lake.h"

using namespace std;

TerrainRenderer :: TerrainRenderer() :
	//m_terrainModel(0),
	//m_vbo(0),
	//m_tformContribTex(0),
	m_heights(0),
	m_terrainStruct(0)
	//m_shadowTexture(0),
	//m_lakeTimer(0.0f),
	//m_heightTexture(0),
	//m_lakeReflection(0),
	//m_cloudPS(0)
{
	EventManager::instance().registerEventListener< Terrain_Changed >(this);
	EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Despawned >(this);
	EventManager::instance().registerEventListener< Player_Destroyed >(this);
	EventManager::instance().registerEventListener< Player_Respawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);
	EventManager::instance().registerEventListener< Enemy_Destroyed >(this);
	EventManager::instance().registerEventListener< Ebomb_Spawned >(this);
	EventManager::instance().registerEventListener< Ebomb_Despawned >(this);
	EventManager::instance().registerEventListener< Crater_Spawned >(this);
	EventManager::instance().registerEventListener< Crater_Despawned >(this);
	EventManager::instance().registerEventListener< Life_Restored >(this);
	EventManager::instance().registerEventListener< Level_Unload >(this);
	EventManager::instance().registerEventListener< Level_Complete >(this);

	//EventManager::instance().registerEventListener<Key_GoingDown>(this); //DEBUG PURPOSES

	//m_craterArrowTexture = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getCraterArrowTexture());

	m_terrainTextures[0] = m_terrainTextures[1] = 0;
	m_terrainStruct = 0;
	m_lake = 0;
}

TerrainRenderer :: ~TerrainRenderer()
{
	EventManager::instance().unRegisterEventListener< Terrain_Changed >(this);
	EventManager::instance().unRegisterEventListener< Level_Load >(this);
	EventManager::instance().unRegisterEventListener< Player_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Player_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Player_Destroyed >(this);
	EventManager::instance().unRegisterEventListener< Player_Respawned >(this);
	EventManager::instance().unRegisterEventListener< Enemy_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Enemy_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Enemy_Destroyed >(this);
	EventManager::instance().unRegisterEventListener< Ebomb_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Ebomb_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Crater_Spawned >(this);
	EventManager::instance().unRegisterEventListener< Crater_Despawned >(this);
	EventManager::instance().unRegisterEventListener< Life_Restored >(this);
	EventManager::instance().unRegisterEventListener< Level_Unload >(this);
	EventManager::instance().unRegisterEventListener< Level_Complete >(this);


	//EventManager::instance().unRegisterEventListener<Key_GoingDown>(this); //DEBUG PURPOSES

	_clearResources();
}

void TerrainRenderer :: onEvent(Level_Unload& evt)
{
	_clearResources();
}

void TerrainRenderer :: _clearResources()
{
	//if(m_terrainModel)
	//{
	//	const std::string texname =  m_terrainModel->matGroup(0).getTextureList()[0]->getName();
	//	TextureIO::instance()->deleteTexture(texname);
	//	delete m_terrainModel;
	//	m_terrainModel = 0;
	//}
	//if(m_vbo)
	//{
	//	delete m_vbo;
	//	m_vbo = 0;
	//}
	if(m_heights)
	{
		//MemMgrRaw::instance()->free(m_heights);
		m_heights = 0;
	}
	if(m_terrainTextures[0])
	{
		TextureMgr::instance().unloadPalette(m_terrainTextures[0]->parentPalette());
		m_terrainTextures[0] = m_terrainTextures[1] = 0;
	}
	if(m_terrainStruct)
	{
		delete m_terrainStruct;
		m_terrainStruct = 0;
	}
	if(m_lake)
	{
		delete m_lake;
		m_lake = 0;
	}
	//if(m_tformContribTex)
	//{
	//	delete m_tformContribTex;
	//	m_tformContribTex = 0;
	//}
	//if(m_shadowTexture)
	//{
	//	delete m_shadowTexture;
	//	m_shadowTexture = 0;
	//}
	//if(m_heightTexture)
	//{
	//	delete m_heightTexture;
	//	m_heightTexture = 0;
	//}
	//if(m_lakeReflection)
	//{
	//	delete m_lakeReflection;
	//	m_lakeReflection = 0;
	//}
	//if(m_cloudPS)
	//{
	//	delete m_cloudPS;
	//	m_cloudPS = 0;
	//}

	//m_craterList.clear();
	//m_shadowCasters.clear();
	//m_tformInfo.clear();
}


void TerrainRenderer :: render(Graphics& g) const
{
	/* 
		Render terrain & lake probably

		Terrain is rendered from the terrain struct
		Lake as a huge quad.
	*/

	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);

	m_lake->render();

	// Bind terrain texture
	m_terrainTextures[0]->bind();
	m_terrainStruct->render();

	RenderEngine::POLY_COUNT += m_terrainStruct->getPolynum() + m_lake->getPolynum();
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
	Vector4 ldir;

	// Open the file
	string full_terrain_path = ModelMgr::instance().getModelDir() + parser.getSection("DataFiles")->getVal("TerrainData");
	TerrainLoader terloader((char *)(full_terrain_path.c_str()),3);
	m_heights = new float[terloader.dim * terloader.dim];
	memcpy(m_heights,terloader.heights,sizeof(float)*terloader.dim*terloader.dim);
	m_terrainStruct = new TerrainStruct(m_heights,terloader.dim,1,terloader.extent);
	

	// Read the dimension & light direction
	dimension = FromString<unsigned>(parser.getSection("Misc")->getVal("MapCellDim"));
	ldir = FromString<Vector4>(parser.getSection("Misc")->getVal("LightDir"));
	RenderEngine::instance().setLevelLight(ldir);

	m_lightColor = FromString<Vector4>(parser.getSection("Misc")->getVal("LightAmbDiff"));
	RenderEngine::instance().setLevelLightColor(m_lightColor);

	RenderEngine::instance().setLevelWind(FromString<Vector3>(parser.getSection("Misc")->getVal("WindLevel")));

	TplPalette * pal = TextureMgr::instance().loadPalette(parser.getSection("DataFiles")->getVal("Texture"),
														  "LevelsTPL.txt");
	m_terrainTextures[0] = new Texture(pal,0,"barren");
	m_terrainTextures[1] = new Texture(pal,1,"tform");
	


	// Get the heights & stuff for Terrain *, at the moment assume the scale is 1 & ymax = 100
	m_mapExtents = FromString<Vector3>(parser.getSection("Misc")->getVal("MapExtent"));
	RenderEngine::instance().setLevelExtents(m_mapExtents);
	m_terrainDimension = dimension;


	// LAKE STUFF
	
	//m_lakeTexture = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getNoiseTexture());
	//m_lakeNormalTexture = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getLakeNormalTexture());

	// Get water color
	m_waterColor = FromString<Vector4>(parser.getSection("Misc")->getVal("WaterColor"));
	GXColor watercolor = {u8(m_waterColor.getX()*255),
						  u8(m_waterColor.getY()*255),
						  u8(m_waterColor.getZ()*255),
						  u8(m_waterColor.getW()*255)};
	m_lake = new Lake(*(unsigned *)(&watercolor));
}

void TerrainRenderer :: onEvent(Terrain_Changed& evt)
{
	//m_terrain = evt.getValue();
	//m_terrain->fillData(m_heights,
	//					m_mapExtents.getX(),
	//					m_mapExtents.getY(),
	//					m_terrainDimension);
	//CoordinateFrame cf;
	//m_cloudPS = PS_Manager::instance().fetchNewPS("PS_Clouds");
	//m_cloudPS->setTransform(cf);
}

//void TerrainRenderer :: _addShadowCaster(const CoordinateModel& model)
//{
//	m_shadowCasters.push_back(model);
//}
//
//void TerrainRenderer :: _removeShadowCaster(const CoordinateFrame * cf)
//{
//	std::vector<CoordinateModel>::iterator it = m_shadowCasters.begin();
//	while(it != m_shadowCasters.end())
//	{
//		if(it->coordframe == cf)
//		{
//			m_shadowCasters.erase(it);
//			break;
//		}
//		++it;
//	}
//}


//void TerrainRenderer :: _updateTerraform(const float dt)
//{
//	const float tpu = RenderEngine::instance().getConstRenderSettings().getTerraformingTimePerUnit();
//	// For every active terraform process, add the dt
//	for(size_t i=0; i< m_tformInfo.size();++i)
//	{
//		m_tformInfo[i].currentTimeOffset += dt;
//		if(m_tformInfo[i].currentTimeOffset > (tpu*m_tformInfo[i].radius))
//		{
//			m_tformInfo.erase(m_tformInfo.begin() + i);
//			--i;
//		}
//	}
//	//CKLOG(string("ActiveTerraforms : ") + ToString<unsigned>(unsigned(m_tformInfo.size())),2);
//
//}

//void TerrainRenderer :: updateTerraformContribution() const
//{
//	// save settings
//	// bind the FBO
//	// make settings
//	// bind shader & texture
//	// for every active terraform , run the shader in the appropriate quad ( for starters the whole map)
//	// the shader makes the terraformed cells white
//	// restore settings , unbind FBO
//
//	// save draw buffer * viewport
//	int curdrawbuf;
//	glGetIntegerv(GL_DRAW_BUFFER,&curdrawbuf);
//	int vp[4];
//	glGetIntegerv(GL_VIEWPORT,vp);
//	const unsigned dim = m_terrain->getTerrainDim();
//	
//	// Change viewport - bind fbo - set draw buffer
//	glViewport(0,0,dim,dim);
//	m_tformFBO.Bind();
//	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
//
//	// push & alter transforms
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glOrtho(-m_mapExtents.getX()*0.5f,
//			m_mapExtents.getX()*0.5f,
//			-m_mapExtents.getZ()*0.5f,
//			m_mapExtents.getZ()*0.5f,
//			-1,1);
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//
//	// bind shader & uniforms & runs the shader
//	Vector3 ll(-m_mapExtents.getX()*0.5f,-m_mapExtents.getZ()*0.5f,0.0f);
//	Vector3 up(0.0f,m_mapExtents.getZ(),0.0f);
//	Vector3 right(m_mapExtents.getX(),0.0f,0.0f);
//
//	glDepthMask(GL_FALSE);
//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_BLEND);
//
//	ShaderManager::instance()->begin("terraformShader");
//
//	const float tpu = RenderEngine::instance().getConstRenderSettings().getTerraformingTimePerUnit();
//
//	for(size_t i=0;i< m_tformInfo.size();++i)
//	{
//		const float timepcent = m_tformInfo[i].currentTimeOffset / (tpu*m_tformInfo[0].radius);
//		Vector2 v(m_tformInfo[i].center.getX(),-m_tformInfo[i].center.getZ());
//		ShaderManager::instance()->setUniform1fv("timePercent", &timepcent);
//		ShaderManager::instance()->setUniform2fv("center",v.cfp());
//		const float sqradius = m_tformInfo[i].radius*m_tformInfo[i].radius;
//		ShaderManager::instance()->setUniform1fv("sqradius",&sqradius);
//
//		RenderEngine::drawQuad(ll,right,up);
//	}
//
//	glDepthMask(GL_TRUE);
//	glDisable(GL_BLEND);
//
//	// disable fbo - restore buffer & viewport
//	FramebufferObject::Disable();
//	glDrawBuffer(curdrawbuf);
//	glViewport(vp[0],vp[1],vp[2],vp[3]);
//
//	// restore matrix states
//	glPopMatrix();
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//}


void TerrainRenderer :: onEvent(Player_Spawned& evt)
{
	// Get the const render settings of the ship
	//const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//Model * model = ModelMgr::instance().getModel(settings.modelName);
	//_addShadowCaster(CoordinateModel(model,cf));
}

void TerrainRenderer :: onEvent(Player_Respawned& evt)
{
	// Get the const render settings of the ship
	//const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//Model * model = ModelMgr::instance().getModel(settings.modelName);
	//_addShadowCaster(CoordinateModel(model,cf));
}

void TerrainRenderer :: onEvent(Enemy_Spawned& evt)
{
	// Get the const render settings of the ship
	//const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//Model * model = ModelMgr::instance().getModel(settings.modelName);
	//_addShadowCaster(CoordinateModel(model,cf));
}

void TerrainRenderer :: onEvent(Enemy_Despawned& evt)
{
	// Fetch the enemy & remove
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//_removeShadowCaster(cf);
}

void TerrainRenderer :: onEvent(Enemy_Destroyed& evt)
{
	//CKLOG("TerrainRenderer got Enemy_Destroyed", 3);
	// Fetch the enemy & remove
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//_removeShadowCaster(cf);
}

void TerrainRenderer :: onEvent(Player_Despawned& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//_removeShadowCaster(cf);
}

void TerrainRenderer :: onEvent(Player_Destroyed& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	//const CoordinateFrame * cf = &(evt.getValue1()->getCoordinateFrame());
	//_removeShadowCaster(cf);
}

void TerrainRenderer :: onEvent(Ebomb_Spawned& evt)
{
	// Get the const render settings of the ship
	//const EntitySettings_t& settings = RenderEngine::instance().getConstRenderSettings().getEntitySettings(evt.getValue()->getType());
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//Model * model = ModelMgr::instance().getModel(settings.modelName);
	//_addShadowCaster(CoordinateModel(model,cf));
}

void TerrainRenderer :: onEvent(Ebomb_Despawned& evt)
{
	// Fetch the player & remove, based on coordinate frame address
	//const CoordinateFrame * cf = &(evt.getValue()->getCoordinateFrame());
	//_removeShadowCaster(cf);
}


//void TerrainRenderer :: renderShadows() const
//{
	//int drawbuf;
	//glGetIntegerv(GL_DRAW_BUFFER,&drawbuf);
	//int vp[4];
	//glGetIntegerv(GL_VIEWPORT,vp);
	//const unsigned ts = RenderEngine::instance().getConstRenderSettings().getShadowTextureSize();
	//glViewport(0,0,ts,ts);
	//VAttribStatus vstatus = VBOMgr::instance().getCurrentFlags();
	//m_shadowFBO.Bind();
	//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	///*
	//	Get the info from renderEngine : 
	//*/
	//
	//Point3 pts[4];
	//RenderEngine::instance().getWsScreenEdges(pts,0);
	//Point3 ll(pts[0]);
	//Point3 lr(pts[1]);
	//Point3 tl(pts[3]);
	//Point3 tr(pts[2]);

	//// For the lower edge ( +z ), use the val from gameplaneheight
	//Point3 pts_gph[4];
	//RenderEngine::instance().getWsScreenEdges(pts_gph,GameLogic::instance().getGamePlaneHeight());
	//ll.setZ(pts_gph[0].getZ());
	//lr.setZ(pts_gph[1].getZ());

	//Vector3 center = (ll.getVector() + lr.getVector() + tl.getVector() + tr.getVector())*0.25f;
	//Vector2 extents;
	//
	//
	//extents.setX(max(max(max(lr.getX() - center.getX(),
	//							  tr.getX() - center.getX()),
	//						  center.getX() - tl.getX()),
	//					  center.getX() - ll.getX()));
	//extents.setY(max(max(max(lr.getZ() - center.getZ(),
	//							  ll.getZ() - center.getZ()),
	//						  center.getZ() - tr.getZ()),
	//					  center.getZ() - tl.getZ()));
	//
	////extents.add(abs(center.getX()),abs(center.getZ()));
	////extents.add(1800,1800);

	//float proj[4];
	//m_cameraRef->getProjSettings(proj[0],proj[1],proj[2],proj[3]);
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//glOrtho(-extents.getX(),
	//		extents.getX(),
	//		-extents.getY(),
	//		extents.getY(),
	//		proj[2],
	//		proj[3]);		// set our proj to look at just the extents we need
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//gluLookAt(center.getX(),
	//		  center.getY() + RenderEngine::instance().getCameraHeightAbovePlane() + GameLogic::instance().getGamePlaneHeight(),
	//		  center.getZ(),
	//		  center.getX(),
	//		  center.getY(),
	//		  center.getZ(),
	//		  0.0f,
	//		  0.0f,
	//		  -1.0f);	// set mview as a top down view,from a really high point, at the center

	//glClear(GL_COLOR_BUFFER_BIT);
	//ShaderManager::instance()->begin("ShadowWrite2");
	//const Vector3 ldir(m_lightDir);

	//float flat[16] = {1 , 0,0,0,
	//				-ldir.getX() / ldir.getY(), 1, -ldir.getZ() / ldir.getY(), 0.0, 
	//				0.0,0.0,1.0 , 0.0, 
	//				0.0,0.0,0.0,1.0};

	//glMultMatrixf(flat);

	//// Draw Ships

	//VAttribStatus vonly;
	//const VertexAttribute * vattr = ShaderManager::instance()->vertexAttribute("Vertex");
	//vonly.attrib[ShaderManager::instance()->vertexAttributeIndex(vattr)] = 0xFF;
	//VBOMgr::instance().setCurrentFlags(vonly);

	//
	//for(std::vector<CoordinateModel>::const_iterator it = m_shadowCasters.begin();
	//	it != m_shadowCasters.end();
	//	++it)
	//{
	//	glPushMatrix();
	//	glMultMatrixf(it->coordframe->getMatrix().cfp());
	//	for(size_t i=0;i<it->model->getMatGroup().size();++i)
	//		it->model->getMatGroup()[i].getVboDesc().call();
	//	glPopMatrix();
	//}

	//// Draw Clouds
	//glEnable(GL_BLEND);
	//m_cloudPS->render();
	//glDisable(GL_BLEND);

	//VBOMgr::instance().setCurrentFlags(vstatus);

	//// Restore stuff

	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);
	//FramebufferObject::Disable();
	//glDrawBuffer(drawbuf);
	//glViewport(vp[0],vp[1],vp[2],vp[3]);
//}

//void TerrainRenderer :: _initShadows(const Vector4& lightdir)
//{
	//m_lightDir.set(-lightdir.getX(),-lightdir.getY(),-lightdir.getZ());
	//// initialize the texture & FBO
	//const unsigned ts = RenderEngine::instance().getConstRenderSettings().getShadowTextureSize();
	//std::vector<MipmapLevel> ml;
	//ml.push_back(MipmapLevel(0,0));
	//m_shadowFBO.Bind();
	//m_shadowTexture = new Texture2D(ts,ts,GL_RGB,GL_RGB,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"Shadow",false,false);
	////glGenerateMipmapEXT(GL_TEXTURE_2D);
	//m_shadowTexture->setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//m_shadowTexture->setParam(GL_TEXTURE_WRAP_S,GL_CLAMP);
	//m_shadowTexture->setParam(GL_TEXTURE_WRAP_T,GL_CLAMP);
	//m_shadowFBO.AttachTexture(GL_TEXTURE_2D,m_shadowTexture->getId(),GL_COLOR_ATTACHMENT0_EXT);
	//m_shadowFBO.IsValid();
	//FramebufferObject::Disable();
//}

void TerrainRenderer :: update(const float dt)
{
	// Also update trees & contrib tex
	//_updateTerraform(dt);
	//m_lakeTimer += dt;
	//m_cloudPS->update(dt);
	m_lake->update(dt);
}

//void TerrainRenderer::onEvent(Key_GoingDown &key) 
//{
//	int keyPressed = key.getValue();
//
//	switch (keyPressed) {
//		case 'M':
//			{
//			TerraformInfo_t tfi;
//			tfi.center = Vector3(0.0f,0.0f,0.0f);
//			tfi.radius = 2048;
//			tfi.currentTimeOffset = 0.0f;
//			m_tformInfo.push_back(tfi);
//			}
//			break;
//	}
//}

//void TerrainRenderer::drawLakeReflection(Graphics& g) const
//{
	//// get the reflection tex size
	//int vp[4];
	//RenderEngine::instance().getViewport(vp);
	//const float reflDimRatio = RenderEngine::instance().getConstRenderSettings().getReflectionTextureScreenRatio();
	//unsigned reflTexSize[2] = {unsigned(reflDimRatio*vp[2]),
	//						   unsigned(reflDimRatio*vp[3])};
	//// be sure that it is a mult of 2
	//reflTexSize[0] += reflTexSize[0]%2;
	//reflTexSize[1] += reflTexSize[1]%2;

	//// save state
	//int drawbuf;
	//glGetIntegerv(GL_DRAW_BUFFER,&drawbuf);
	//glViewport(0,0,reflTexSize[0],reflTexSize[1]);

	//// bind & set draw buffer
	//m_reflectionFBO.Bind();
	//glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//const float gph = GameLogic::instance().getGamePlaneHeight();

	//// compute the screen quad

	//Point3 pts[4];
	//RenderEngine::instance().getWsScreenEdges(pts,0);
	//Point3 ll(pts[0]);
	//Point3 lr(pts[1]);
	//Point3 tl(pts[3]);
	//Point3 tr(pts[2]);

	//// For the lower edge ( +z ), use the val from gameplaneheight
	//Point3 pts_gph[4];
	//RenderEngine::instance().getWsScreenEdges(pts_gph,GameLogic::instance().getGamePlaneHeight());
	//ll.setZ(pts_gph[0].getZ());
	//lr.setZ(pts_gph[1].getZ());

	//Vector3 center = (ll.getVector() + lr.getVector() + tl.getVector() + tr.getVector())*0.25f;
	//Vector2 extents;
	//
	//extents.setX(max(max(max(lr.getX() - center.getX(),
	//							  tr.getX() - center.getX()),
	//						  center.getX() - tl.getX()),
	//					  center.getX() - ll.getX()));
	//extents.setY(max(max(max(lr.getZ() - center.getZ(),
	//							  ll.getZ() - center.getZ()),
	//						  center.getZ() - tr.getZ()),
	//					  center.getZ() - tl.getZ()));
	////extents.add(abs(center.getX()),abs(center.getZ()));

	//// Fix the camera

	//float proj[4];
	//m_cameraRef->getProjSettings(proj[0],proj[1],proj[2],proj[3]);
	//glMatrixMode(GL_PROJECTION);
	//glPushMatrix();
	//glLoadIdentity();
	//glOrtho(-extents.getX(),
	//		extents.getX(),
	//		-extents.getY(),
	//		extents.getY(),
	//		proj[2],
	//		proj[3]);		// set our proj to look at just the extents we need
	//glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	//glLoadIdentity();
	//gluLookAt(center.getX(),
	//		  center.getY() + RenderEngine::instance().getCameraHeightAbovePlane(),
	//		  center.getZ(),
	//		  center.getX(),
	//		  center.getY(),
	//		  center.getZ(),
	//		  0.0f,
	//		  0.0f,
	//		  -1.0f);	// set mview as a top down view,from a really high point, at the center

	//
	//m_laserRendererRef->render(g);

	//glScalef(1.0f,-1.0f,1.0f);

	////RenderEngine::instance().setLevelLight(-RenderEngine::instance().getLevelLight());

	//// draw ships
	//m_shipRendererRef->render(g);

	////RenderEngine::instance().setLevelLight(-RenderEngine::instance().getLevelLight());

	//
	//glPopMatrix();
	//glMatrixMode(GL_PROJECTION);
	//glPopMatrix();
	//glMatrixMode(GL_MODELVIEW);

	////restore settings
	//FramebufferObject::Disable();
	//glDrawBuffer(drawbuf);
	//glViewport(vp[0],vp[1],vp[2],vp[3]);
	//
//}


void TerrainRenderer::onEvent(Crater_Spawned& evt)
{
	//m_craterList.push_back(CraterInfo_t(evt.getValue(),
	//									RenderEngine ::instance().getColorFromEbombType(evt.getValue()->getEbombType())));
}

void TerrainRenderer::onEvent(Crater_Despawned& evt)
{
	//for(std::vector<CraterInfo_t>::iterator it = m_craterList.begin();
	//	it != m_craterList.end();
	//	++it)
	//{
	//	if(it->crater == evt.getValue())
	//	{
	//		m_craterList.erase(it);
	//		break;
	//	}
	//}
}

void TerrainRenderer::onEvent(Life_Restored& evt)
{
	//Crater* crater = evt.getValue();

	//for(std::vector<CraterInfo_t>::iterator it = m_craterList.begin();
	//	it != m_craterList.end();
	//	++it)
	//{
	//	if (it->crater == crater)
	//	{
	//		m_craterList.erase(it);
	//		break;
	//	}
	//}

	//TerraformInfo_t tfi;
	//tfi.center = crater->getPosition().getVector();
	//tfi.radius = crater->getAffectedAreaRadius();
	//tfi.currentTimeOffset = 0.0f;
	//m_tformInfo.push_back(tfi);
}


void TerrainRenderer::onEvent(Level_Complete& evt)
{
	//TerraformInfo_t tfi;
	//tfi.center = Vector3(0.0f,0.0f,0.0f);
	//tfi.radius = m_mapExtents.getX()*sqrtf(2.0f);
	//tfi.currentTimeOffset = 0.0f;
	//m_tformInfo.push_back(tfi);
}