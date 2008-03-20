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
#include "../gfx/Camera.h"

using namespace std;

TerrainRenderer :: TerrainRenderer() :
	m_terrainModel(0),
	m_vbo(0),
	m_tformContribTex(0),
	m_heights(0),
	m_shadowTexture(0),
	m_lakeTimer(0.0f),
	m_heightTexture(0)
{
	EventManager::instance().registerEventListener< Terrain_Changed >(this);
	EventManager::instance().registerEventListener< Level_Load >(this);
	EventManager::instance().registerEventListener< Player_Spawned >(this);
	EventManager::instance().registerEventListener< Enemy_Spawned >(this);
	EventManager::instance().registerEventListener< Player_Despawned >(this);
	EventManager::instance().registerEventListener< Enemy_Despawned >(this);

	EventManager::instance().registerEventListener<Key_GoingDown>(this); //DEBUG PURPOSES
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
	if(m_shadowTexture)
		delete m_shadowTexture;
	if(m_heightTexture)
		delete m_heightTexture;
}


void TerrainRenderer :: render(Graphics& g) const
{
	_renderShadows();
	_updateTerraformContribution();

	//creating the texture projection matrix
	float texProjMat[16];
	float viewMat[16];
	glPushMatrix();
		glLoadIdentity();
		// First scale and bias into [0..1] range.
		glTranslatef(0.5, 0.5, 0);
		glScalef(0.5, 0.5, 1);
		glOrtho(m_lightCameraProjSettings[0],
			m_lightCameraProjSettings[1],
			m_lightCameraProjSettings[2],
			m_lightCameraProjSettings[3],
			m_lightCameraProjSettings[4],
			m_lightCameraProjSettings[5]);
		gluLookAt(m_lightCameraEye.getX(),
			m_lightCameraEye.getY(),
			m_lightCameraEye.getZ(),
			m_lightCameraEye.getX() + m_lightCameraVectorView.getX(),
			m_lightCameraEye.getY() + m_lightCameraVectorView.getY(),
			m_lightCameraEye.getZ() + m_lightCameraVectorView.getZ(),
			m_lightCameraVectorUp.getX(),
			m_lightCameraVectorUp.getY(),
			m_lightCameraVectorUp.getZ());
		/* ~ MY WAY */
		glGetFloatv(GL_MODELVIEW_MATRIX, texProjMat);
	glPopMatrix();

	glGetFloatv(GL_MODELVIEW_MATRIX, viewMat);

	Matrix44 invViewMatrix(viewMat);
	invViewMatrix.invert();

	// Do some magic to render the terrain
	ShaderManager::instance()->begin("TerrainShader");
	m_terrainModel->matGroup(0).getTextureList()[0]->bind(0);
	ShaderManager::instance()->setUniform1i("texmap0",0);
	m_terrainModel->matGroup(0).getTextureList()[1]->bind(1);
	ShaderManager::instance()->setUniform1i("texmap1",1);
	m_tformContribTex->bind(2);
	ShaderManager::instance()->setUniform1i("contribMap",2);
	m_shadowTexture->bind(3);
	ShaderManager::instance()->setUniform1i("shadowTex",3);
	ShaderManager::instance()->setUniformMatrix4fv("TexGenMat", texProjMat);
	ShaderManager::instance()->setUniformMatrix4fv("InvViewMat", invViewMatrix.cfp());

	const float mapsize = float(m_terrain->getTerrainDim());
	ShaderManager::instance()->setUniform1fv("mapCellNum",&mapsize);

	m_terrainModel->matGroup(0).vboDesc().call();

	TextureMgr::instance()->setTextureUnit(3);
	TextureMgr::instance()->setBoundTexture(0,3);
	TextureMgr::instance()->setTextureUnit(2);
	TextureMgr::instance()->setBoundTexture(0,2);

	CHECK_GL_ERROR();

	// Draw the lake
	// FIXME : do it appropriately

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	
	ShaderManager::instance()->begin("lakeShader");
	m_lakeTexture->bind(0);
	ShaderManager::instance()->setUniform1i("noiseTex",0);
	m_shadowTexture->bind(1);
	ShaderManager::instance()->setUniform1i("shadowTex",1);
	m_cloudTexture->bind(2);
	ShaderManager::instance()->setUniform1i("cloudTex",2);
	m_heightTexture->bind(3);
	ShaderManager::instance()->setUniform1i("heightTex",3);

	ShaderManager::instance()->setUniformMatrix4fv("TexGenMat", texProjMat);
	ShaderManager::instance()->setUniformMatrix4fv("InvViewMat", invViewMatrix.cfp());
	ShaderManager::instance()->setUniform1fv("timer",&m_lakeTimer);
	
	const float ws = RenderEngine::instance().getConstRenderSettings().getWaveSpeed();
	const float wcr = RenderEngine::instance().getConstRenderSettings().getWaveChangeRate();
	const float wr = RenderEngine::instance().getConstRenderSettings().getWaveRepeats();
	const float cr = RenderEngine::instance().getConstRenderSettings().getCloudRepeats();

	ShaderManager::instance()->setUniform1fv("timer",&m_lakeTimer);
	ShaderManager::instance()->setUniform1fv("waveChangeRate",&wcr);
	ShaderManager::instance()->setUniform1fv("waveSpeed",&ws);
	ShaderManager::instance()->setUniform4fv("lightColor",m_lightColor.cfp());
	Vector4 lightPos(m_lightDir.getX(),m_lightDir.getY(),m_lightDir.getZ(),0.0f);
	ShaderManager::instance()->setUniform4fv("lightPosition",lightPos.cfp());

	
	ShaderManager::instance()->setUniform1fv("cloudRepeats",&cr);
	ShaderManager::instance()->setUniform1fv("waveRepeats",&wr);

	Vector3 ll(-m_mapExtents.getX()*0.5f,0.0f,m_mapExtents.getX()*0.5f);
	Vector3 right(m_mapExtents.getX(),0,0);
	Vector3 up(0,0,-m_mapExtents.getX());
	RenderEngine::drawTexturedQuad(ll,right,up,Vector2(0,0),Vector2(1.0f,1.0f));

	ShaderManager::instance()->end();
	glDisable(GL_BLEND);

	TextureMgr::instance()->setTextureUnit(3);
	TextureMgr::instance()->setBoundTexture(0,3);
	TextureMgr::instance()->setTextureUnit(2);
	TextureMgr::instance()->setBoundTexture(0,2);
	TextureMgr::instance()->setTextureUnit(1);
	TextureMgr::instance()->setBoundTexture(0,1);
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

	m_lightColor = FromString<Vector4>(parser.getSection("Misc")->getVal("LightAmbDiff"));

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
	m_tformContribTex = new Texture2D(dimension,dimension,GL_RGB,GL_RGB,GL_UNSIGNED_BYTE,
									  ml,GL_TEXTURE_RECTANGLE_ARB,"Terraform contribution",false,true);
	m_tformFBO.Bind();
	m_tformFBO.UnattachAll();
	m_tformFBO.AttachTexture(m_tformContribTex->getTarget(),m_tformContribTex->getId(),GL_COLOR_ATTACHMENT0_EXT);
	bool res = m_tformFBO.IsValid();
	assert(res);
	FramebufferObject::Disable();


	// LAKE STUFF
	
	m_lakeTexture = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getLakeTexture());
	m_cloudTexture = TextureIO::instance()->getTexture(RenderEngine::instance().getConstRenderSettings().getCloudTexture());

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
		Vector2 v(m_tformInfo[i].center.getX(),m_tformInfo[i].center.getZ());
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

void TerrainRenderer :: _renderShadows() const
{
	// switches VBOs to vertex-only mode
	// set's up light camera
	// renders ships
	// fetches - stores texture
	// switches camera
	// computes 

	int drawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&drawbuf);
	int vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	const unsigned ts = RenderEngine::instance().getConstRenderSettings().getShadowTextureSize();
	glViewport(0,0,ts,ts);
	VAttribStatus vstatus = VBOMgr::instance().getCurrentFlags();
	m_shadowFBO.Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);

	VAttribStatus vonly;
	const VertexAttribute * vattr = ShaderManager::instance()->vertexAttribute("Vertex");
	vonly.attrib[ShaderManager::instance()->vertexAttributeIndex(vattr)] = 0xFF;
	VBOMgr::instance().setCurrentFlags(vonly);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(m_lightCameraProjSettings[0],
		m_lightCameraProjSettings[1],
		m_lightCameraProjSettings[2],
		m_lightCameraProjSettings[3],
		m_lightCameraProjSettings[4],
		m_lightCameraProjSettings[5]);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	gluLookAt(m_lightCameraEye.getX(),
			m_lightCameraEye.getY(),
			m_lightCameraEye.getZ(),
			m_lightCameraEye.getX() + m_lightCameraVectorView.getX(),
			m_lightCameraEye.getY() + m_lightCameraVectorView.getY(),
			m_lightCameraEye.getZ() + m_lightCameraVectorView.getZ(),
			m_lightCameraVectorUp.getX(),
			m_lightCameraVectorUp.getY(),
			m_lightCameraVectorUp.getZ());

	// TODO : Disable Color Writes Here?? render to depth & fetch it as a luminance?
	// Render the ships minimally - set the minimal shader

	glClear(GL_COLOR_BUFFER_BIT);
	ShaderManager::instance()->begin("ShadowWriteShader");
	
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

	// Restore stuff

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	FramebufferObject::Disable();
	glDrawBuffer(drawbuf);
	VBOMgr::instance().setCurrentFlags(vstatus);
	glViewport(vp[0],vp[1],vp[2],vp[3]);
}

void TerrainRenderer :: _initShadows(const Vector4& lightdir)
{
	// get camera projector settings
	float w,h,n,f;
	m_cameraRef->getProjSettings(w,h,n,f);
	
	// compute the max quad dims shown in height = gameplane
	const float gameplanedepth = 1500.0f;
	float maxLeftRight = w* gameplanedepth/ n;
	float maxTopDown = h*gameplanedepth / n;

	// compute the light camera vectors
	Vector3 ldir(-lightdir.getX(),-lightdir.getY(),-lightdir.getZ());
	ldir.normalize();
	Vector3 yAxis(0.0f,1.0f,0.0f);
	Vector3 right(Vector3::cross(ldir,yAxis));
	right.normalize();
	m_lightCameraVectorUp = Vector3::cross(right,ldir);
	m_lightCameraVectorUp.normalize();

	// FIXME : 
	maxLeftRight = 950;		// -- GOOD BUT HACK!!
	maxTopDown = 950;

	// set a safe distance for the light eye
	m_lightCameraEye = Vector3(0.0f,gameplanedepth + GameLogic::instance().getGamePlaneHeight(),0.0f) - 500.0f*ldir;
	m_lightCameraVectorView = ldir;

	m_lightCameraProjSettings[0] = -maxLeftRight;
	m_lightCameraProjSettings[1] = maxLeftRight;
	m_lightCameraProjSettings[2] = -maxTopDown;
	m_lightCameraProjSettings[3] = maxTopDown;
	m_lightCameraProjSettings[4] = n;
	m_lightCameraProjSettings[5] = f;

	m_lightDir = ldir;

	// initialize the texture & FBO
	const unsigned ts = RenderEngine::instance().getConstRenderSettings().getShadowTextureSize();
	std::vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel(0,0));
	m_shadowFBO.Bind();
	m_shadowTexture = new Texture2D(ts,ts,GL_RGB,GL_RGB,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"Shadow",false,false);
	//glGenerateMipmapEXT(GL_TEXTURE_2D);
	m_shadowTexture->setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	m_shadowFBO.AttachTexture(GL_TEXTURE_2D,m_shadowTexture->getId(),GL_COLOR_ATTACHMENT0_EXT);
	m_shadowFBO.IsValid();
	FramebufferObject::Disable();
}

void TerrainRenderer :: update(const float dt)
{
	// Also update trees & contrib tex
	
	m_lightCameraEye = m_cameraRef->getEye() - 500.0f*m_lightDir;
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