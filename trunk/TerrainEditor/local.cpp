#include "local.h"
#include <ConfParser/ParserSection.h>
#include <Misc/Utils.h>
#include <MemManager/MemMgr_RawData.h>
#include <Texture/Texture2D.h>
#include <assert.h>
#include <Shaders/ShaderManager.h>
#include <Math/vec2.h>

#include <iostream>
#include <fstream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace CKLib;
using namespace std;

Terrain :: Terrain(const ParserSection * parsec)
:_dim(513),_terrainData(0),
_terrainTexData(0),
_tex(0),_texbck(0),
_drawLayerTextures(false),
_displayMode(D_BW),
_normalTex(0),
_xzscale(1),
_yscale(100),
_terrainNormalData(0),
_terrainPosData(0),
_fbo(),
_fbolm(),
_lightdir(1.0f,1.0f,-1.0f),
_lightMap(0),
_useLightmap(false),
_shadowEffect(0.7f),
_contribTex(0),
_terraTex(0),
_drawTexturedLightmap(false),
_layerTform(true),
_heightRGB(0),
_canUndo(false),
_displayTrees(false),
_lightAmbDiff(1.0f),
_displayCraters(false),
_lightmapTexDim(0)
{
	_lightdir.w() = 0.0f;
	_texLayer[0] = _texLayer[1] = _texLayer[2] = _texLayer[3] = 0;
	_tformLayer[0] = _tformLayer[1] = _tformLayer[2] = _tformLayer[3] = 0;
	_barrenLayer[0] = _barrenLayer[1] = _barrenLayer[2] = _barrenLayer[3] = 0;
	_textureRepeats[0] = _textureRepeats[1] = _textureRepeats[2] = _textureRepeats[3] = 14.0f;
	_seaLevel = 0.2f;
	_layerInfo[0]._bleed = 0.05f;
	_layerInfo[0]._changeAt = 0.25f;
	_layerInfo[1]._bleed = 0.05f;
	_layerInfo[1]._changeAt = 0.5f;
	_layerInfo[2]._bleed = 0.05f;
	_layerInfo[2]._changeAt = 0.75f;

	_outTexDim = FromString<unsigned>(parsec->getVal("OutputTextureDim"));
	_lightmapTexDim = FromString<unsigned>(parsec->getVal("LightmapTextureDim"));
	_exportDir = parsec->getVal("ExportDirectory");
	_levelName = parsec->getVal("LevelName");

	string file = parsec->getVal("File");
	_dim = FromString<unsigned>(parsec->getVal("Dim"));
	_loadRawData(file);

	TextureIO::instance()->setOptionGenMipmaps(true);
	_texLayer[0] = _tformLayer[0] = TextureIO::instance()->loadImage(parsec->getVal("Layer1Tform"));
	_texLayer[1] = _tformLayer[1] = TextureIO::instance()->loadImage(parsec->getVal("Layer2Tform"));
	_texLayer[2] = _tformLayer[2] = TextureIO::instance()->loadImage(parsec->getVal("Layer3Tform"));
	_texLayer[3] = _tformLayer[3] = TextureIO::instance()->loadImage(parsec->getVal("Layer4Tform"));

	_barrenLayer[0] = TextureIO::instance()->loadImage(parsec->getVal("Layer1Barren"));
	_barrenLayer[1] = TextureIO::instance()->loadImage(parsec->getVal("Layer2Barren"));
	_barrenLayer[2] = TextureIO::instance()->loadImage(parsec->getVal("Layer3Barren"));
	_barrenLayer[3] = TextureIO::instance()->loadImage(parsec->getVal("Layer4Barren"));
	CHECK_GL_ERROR();

	_initBuffers();

	// init fbo
	_fbo.Bind();
	_fbo.AttachTexture(GL_TEXTURE_RECTANGLE_ARB,_posTex->getId(),GL_COLOR_ATTACHMENT0_EXT);
	_fbo.AttachTexture(GL_TEXTURE_RECTANGLE_ARB,_normalTex->getId(),GL_COLOR_ATTACHMENT1_EXT);
	_fbo.IsValid();

	_initLightmap();

	// init fbolm
	_fbolm.Bind();
	_fbolm.AttachTexture(GL_TEXTURE_2D,_lightMap->getId(),GL_COLOR_ATTACHMENT0_EXT);
	_fbolm.IsValid();

	// init fboContrib
	_fboContrib.Bind();
	_fboContrib.AttachTexture(GL_TEXTURE_2D,_contribTex->getId(),GL_COLOR_ATTACHMENT0_EXT);
	_fboContrib.IsValid();

	// init fboTerra
	std::vector<MipmapLevel> mip;
	mip.push_back(MipmapLevel(0,_outTexDim*_outTexDim*3));
	_terraTex = new Texture2D(_outTexDim,_outTexDim,GL_RGB,GL_RGB,GL_UNSIGNED_BYTE,mip,GL_TEXTURE_2D,"Terra",false,false);
	_fboTerra.Bind();
	_fboTerra.AttachTexture(GL_TEXTURE_2D,_terraTex->getId(),GL_COLOR_ATTACHMENT0_EXT);
	_fboTerra.IsValid();

	// init fboTerra
	_heightRGB = new Texture2D(_dim,_dim,GL_RGB32F_ARB,GL_RGB,GL_FLOAT,mip,GL_TEXTURE_RECTANGLE_ARB,"TerraHeightRGB",false,true);
	_fboCrater.Bind();
	_fboCrater.AttachTexture(GL_TEXTURE_RECTANGLE_ARB,_heightRGB->getId(),GL_COLOR_ATTACHMENT0_EXT);
	_fboCrater.IsValid();

	FramebufferObject::Disable();

	updatePositionsNormals();
	updateContribTex();

	CHECK_GL_ERROR();

	updateLightmap();
	CHECK_GL_ERROR();
}

Terrain :: ~Terrain()
{	
	if(_terrainTexData)
		MemMgrRaw::instance()->free(_terrainTexData);
	if(_tex)
		delete _tex;
	if(_texbck)
		delete _texbck;
	if(_lightMap)
		delete _lightMap;
	if(_contribTex)
		delete _contribTex;
	if(_terraTex)
		delete _terraTex;
	if(_heightRGB)
		delete _heightRGB;
	for(unsigned i=0;i<4;++i)
	{
		if(_tformLayer[i])
			delete _tformLayer[i];
		if(_barrenLayer[i])
			delete _barrenLayer[i];
	}
	if(_normalTex)
		delete _normalTex;
	if(_terrainNormalData)
		MemMgrRaw::instance()->free(_terrainNormalData);
	if(_terrainPosData)
		MemMgrRaw::instance()->free(_terrainPosData);

	glDeleteBuffers(1,&_vbuffer);
	glDeleteBuffers(1,&_vtbuffer);
	glDeleteBuffers(1,&_ibuffer);
}

void Terrain :: _loadRawData(const std::string& fname)
{
	_terrainData = MemMgrRaw::instance()->allocate<unsigned short>(_dim*_dim);
	FILE * fp = fopen(fname.c_str(),"rb");
	size_t read = fread(_terrainData,2,_dim*_dim,fp);
	assert(read == (_dim*_dim));

	_terrainTexData = MemMgrRaw::instance()->allocate<float>(_dim*_dim);
	_terrainPosData = MemMgrRaw::instance()->allocate<vec3f>(_dim*_dim);
	_terrainNormalData = MemMgrRaw::instance()->allocate<vec3f>(_dim*_dim);

	// Convert to luminance float
	const float stf = 1.0f / 65536.0f;
	for(unsigned i=0;i< (_dim*_dim);++i)
	{
		_terrainPosData[i].y() = _yscale*(_terrainTexData[i] = float(_terrainData[i]) * stf);
		_terrainPosData[i].x() = _xzscale * (i%_dim);
		_terrainPosData[i].z() = -_xzscale * (i/_dim);
	}
	MemMgrRaw::instance()->free(_terrainData);

	std::vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel((unsigned char *)_terrainTexData,_dim*_dim*sizeof(float)));

	CHECK_GL_ERROR();
	_tex = new Texture2D(_dim,_dim,GL_LUMINANCE32F_ARB,GL_LUMINANCE,GL_FLOAT,ml,GL_TEXTURE_RECTANGLE_ARB,"Terrain",false,true);
	_texbck = new Texture2D(_dim,_dim,GL_LUMINANCE32F_ARB,GL_LUMINANCE,GL_FLOAT,ml,GL_TEXTURE_RECTANGLE_ARB,"Terrainbck",false,true);
	CHECK_GL_ERROR();

	//Create the position & normal textures
	ml.pop_back();
	ml.push_back(MipmapLevel((unsigned char *)_terrainNormalData,_dim*_dim*sizeof(vec3f)));
	_normalTex = new Texture2D(_dim,_dim,GL_RGBA32F_ARB,GL_RGBA,GL_FLOAT,ml,GL_TEXTURE_RECTANGLE_ARB,"TerrainNorms",false,true);
	ml.pop_back();

	CHECK_GL_ERROR();

	ml.push_back(MipmapLevel((unsigned char *)_terrainPosData,_dim*_dim*sizeof(vec3f)));
	_posTex = new Texture2D(_dim,_dim,GL_RGBA32F_ARB,GL_RGBA,GL_FLOAT,ml,GL_TEXTURE_RECTANGLE_ARB,"TerrainPos",false,true);
	ml.pop_back();

	ml.push_back(MipmapLevel((unsigned char *)NULL,4*(_dim-1)*(_dim-1)));
	_contribTex = new Texture2D(_dim-1,_dim-1,GL_RGBA,GL_RGBA,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"ContribTex",false,false);

	CHECK_GL_ERROR();
}

void Terrain :: updatePositionsNormals()
{
	static GLuint bufs[2] = {GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT};

	CHECK_GL_ERROR();
	// bind the shader & pass the uniforms
	ShaderManager::instance()->begin("compPosNorm");
	_tex->bind();
	ShaderManager::instance()->setUniform1i("heightmap",0);
	float xyzscale[3] = {_xzscale,_yscale,_xzscale};
	ShaderManager::instance()->setUniform3fv("xyzscale",xyzscale);

	CHECK_GL_ERROR();

	// Prepare the FBO

	GLint _curdrawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&_curdrawbuf);
	_fbo.Bind();
	glDrawBuffers(2,bufs);
	int vp[4];
	glGetIntegerv(GL_VIEWPORT,vp);
	glViewport(0,0,_dim,_dim);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,_dim,0,_dim,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	CHECK_GL_ERROR();
	
	glBegin(GL_QUADS);
		glTexCoord2i(0,_dim);
		glVertex2i(0,_dim);
		glTexCoord2i(0,0);
		glVertex2i(0,0);
		glTexCoord2i(_dim,0);
		glVertex2i(_dim,0);
		glTexCoord2i(_dim,_dim);
		glVertex2i(_dim,_dim);
	glEnd();

	CHECK_GL_ERROR();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	FramebufferObject::Disable();
	glDrawBuffer(_curdrawbuf);
	glViewport(vp[0],vp[1],vp[2],vp[3]);

	ShaderManager::instance()->end();

	_normalTex->dlData(_terrainNormalData);
	_posTex->dlData(_terrainPosData);

	update3DMode();
	updateLightmap();
	CHECK_GL_ERROR();
}

void Terrain :: updateLightmap()
{
	static GLuint bufs[2] = {GL_COLOR_ATTACHMENT0_EXT,GL_COLOR_ATTACHMENT1_EXT};

	// bind the shader & pass the uniforms
	ShaderManager::instance()->begin("computeLightmap");
	_tex->bind();
	_tex->setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	_tex->setParam(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	ShaderManager::instance()->setUniform1i("heightmap",0);
	_normalTex->bind(1);
	ShaderManager::instance()->setUniform1i("normalmap",1);
	float xyzscale[3] = {_xzscale,_yscale,_xzscale};
	ShaderManager::instance()->setUniform3fv("xyzscale",xyzscale);
	const float dimdenom = 1.0f / float(_dim);
	const vec3f ldir(normalize(_lightdir));
	ShaderManager::instance()->setUniform3fv("lightdir",(GLfloat *)(ldir.get()));

	ShaderManager::instance()->setUniform1fv("shadoweffect",&_shadowEffect);
	float texsize = float(_dim);
	ShaderManager::instance()->setUniform1fv("texsize",&texsize);
	CHECK_GL_ERROR();

	/*
		Send as uniforms some used variables
	*/

	vec2f ldirxz(_lightdir.x(),_lightdir.z());
	vec2f ldirxznorm(ldirxz);
	const float len = ldirxznorm.length();
	const float lendenom = (len > THRESHOLD<float>()) ? 1.0f / len : 9999999.9f;
	ldirxznorm *= lendenom*0.2f;
	
	vec2f coord_increment = ldirxznorm;
	coord_increment.y()*=-1.0f;
	float y_increment = _lightdir.y() * lendenom;

	ShaderManager::instance()->setUniform2fv("coord_increment",(GLfloat *)(coord_increment.get()));
	ShaderManager::instance()->setUniform1fv("y_increment",&y_increment);

	/*
		~~~ Send as uniforms some used variables
	*/
	
	const unsigned reddim = _lightmapTexDim;

	// Prepare the FBO

	GLint _curdrawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&_curdrawbuf);
	_fbolm.Bind();
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	int vp[4];
	CHECK_GL_ERROR();
	glGetIntegerv(GL_VIEWPORT,vp);
	glViewport(0,0,reddim,reddim);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	CHECK_GL_ERROR();
	glOrtho(0,reddim,0,reddim,-1,1);
	CHECK_GL_ERROR();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glBegin(GL_QUADS);
		glTexCoord2i(0,_dim);
		glVertex2i(0,reddim);
		glTexCoord2i(0,0);
		glVertex2i(0,0);
		glTexCoord2i(_dim,0);
		glVertex2i(reddim,0);
		glTexCoord2i(_dim,_dim);
		glVertex2i(reddim,reddim);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	FramebufferObject::Disable();
	glDrawBuffer(_curdrawbuf);
	glViewport(vp[0],vp[1],vp[2],vp[3]);

	ShaderManager::instance()->end();
	TextureMgr::instance()->setBoundTexture(0,1);
	TextureMgr::instance()->setTextureUnit(0);

	CHECK_GL_ERROR();
}

void Terrain :: updateContribTex()
{
	static GLuint bufs[2] = {GL_COLOR_ATTACHMENT0_EXT};

	// bind the shader & pass the uniforms
	ShaderManager::instance()->begin("computeContrib");
	_tex->bind();
	ShaderManager::instance()->setUniform1i("heightmap",0);
	GLfloat bleedBounds[3] = {_layerInfo[0]._bleed,_layerInfo[1]._bleed,_layerInfo[2]._bleed};
	GLfloat layerBounds[3] = {_layerInfo[0]._changeAt,_layerInfo[1]._changeAt,_layerInfo[2]._changeAt};
	ShaderManager::instance()->setUniform3fv("layerBounds",layerBounds);
	ShaderManager::instance()->setUniform3fv("bleedBounds",bleedBounds);
	
	CHECK_GL_ERROR();

	// Prepare the FBO

	const unsigned reddim = _dim - 1;

	GLint _curdrawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&_curdrawbuf);
	_fboContrib.Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	int vp[4];
	CHECK_GL_ERROR();
	glGetIntegerv(GL_VIEWPORT,vp);
	glViewport(0,0,reddim,reddim);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	CHECK_GL_ERROR();
	glOrtho(0,reddim,0,reddim,-1,1);
	CHECK_GL_ERROR();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glBegin(GL_QUADS);
		glTexCoord2i(0,_dim);
		glVertex2i(0,reddim);
		glTexCoord2i(0,0);
		glVertex2i(0,0);
		glTexCoord2i(_dim,0);
		glVertex2i(reddim,0);
		glTexCoord2i(_dim,_dim);
		glVertex2i(reddim,reddim);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	FramebufferObject::Disable();
	glDrawBuffer(_curdrawbuf);
	glViewport(vp[0],vp[1],vp[2],vp[3]);

	ShaderManager::instance()->end();

	CHECK_GL_ERROR();
}

unsigned Terrain :: getDim() {return _dim;}


void Terrain :: drawGrayscale()
{
	_tex->bind();
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);
		glTexCoord2i(0,_dim);
		glVertex2i(-1,1);
		glTexCoord2i(0,0);
		glVertex2i(-1,-1);
		glTexCoord2i(_dim,0);
		glVertex2i(1,-1);
		glTexCoord2i(_dim,_dim);
		glVertex2i(1,1);
	glEnd();
}

void Terrain :: drawNormals()
{
	if(!_normalTex)
		return;
	_normalTex->bind();
	glColor3f(1.0f,1.0f,1.0f);
	glBegin(GL_QUADS);
		glTexCoord2i(0,_dim);
		glVertex2i(-1,1);
		glTexCoord2i(0,0);
		glVertex2i(-1,-1);
		glTexCoord2i(_dim,0);
		glVertex2i(1,-1);
		glTexCoord2i(_dim,_dim);
		glVertex2i(1,1);
	glEnd();
}

LayerInfo& Terrain :: layerInfo(const unsigned i)
{
	assert(i < 3);
	return _layerInfo[i];
}

Texture * Terrain :: layerTexture(const unsigned i)
{
	assert(i < 4);
	return _texLayer[i];
}

float & Terrain :: seaLevel()
{
	return _seaLevel;
}

float& Terrain :: textureRepeats(const unsigned i)
{
	assert(i < 4);
	return _textureRepeats[i];
}

void Terrain :: drawLayers()
{
	// height : -1 to 1, centers : -0.6, -0.2, 0.2, 0.6, height = 0.4
	// width : 0.4, starting at 0.6

	const float extent = 0.15f;
	float start_x = 0.6f,
		  start_y = -0.6f,
		  offset_y = 0.4f;
	
	glColor3f(1.0f,1.0f,1.0f);
	for(unsigned i=0;i<4;++i)
	{
		_texLayer[i]->bind();
		glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(start_x - extent,start_y - extent + offset_y*i);
		glTexCoord2f(1,0);
		glVertex2f(start_x + extent,start_y - extent + offset_y*i);
		glTexCoord2f(1,1);
		glVertex2f(start_x + extent,start_y + extent + offset_y*i);
		glTexCoord2f(0,1);
		glVertex2f(start_x - extent,start_y + extent + offset_y*i);
		glEnd();
	}
}

void Terrain :: drawColourLayered()
{
	CHECK_GL_ERROR();
	static int iteration = 0;
	++iteration;
	ShaderManager::instance()->begin("ColouredLayers");
	// Send uniforms

	GLfloat bleedBounds[3] = {_layerInfo[0]._bleed,_layerInfo[1]._bleed,_layerInfo[2]._bleed};
	GLfloat layerBounds[3] = {_layerInfo[0]._changeAt,_layerInfo[1]._changeAt,_layerInfo[2]._changeAt};
	ShaderManager::instance()->setUniform3fv("layerBounds",layerBounds);
	ShaderManager::instance()->setUniform3fv("bleedBounds",bleedBounds);
	ShaderManager::instance()->setUniform1fv("seaLevel",&_seaLevel);
	_tex->bind();
	ShaderManager::instance()->setUniform1i("heightmap",0);
	CHECK_GL_ERROR();

	glBegin(GL_QUADS);
		glTexCoord2i(0,_dim);
		glVertex2i(-1,1);
		glTexCoord2i(0,0);
		glVertex2i(-1,-1);
		glTexCoord2i(_dim,0);
		glVertex2i(1,-1);
		glTexCoord2i(_dim,_dim);
		glVertex2i(1,1);
	glEnd();
	ShaderManager::instance()->end();
}

void Terrain :: draw()
{
	switch(_displayMode)
	{
		case D_BW : 
			drawGrayscale();
			break;
		case D_COL : 
			drawColourLayered();
			break;
		case D_TEX :
			drawSimpleTextured();
			break;
		case D_NORM :
			drawNormals();
			break;
		case D_3D :
			if(_drawTexturedLightmap)
			{
				drawFull3D();
				//generateTerraMap();
			}
			else
				draw3D();
			break;
	}

	if(_displayCraters)
		drawCraters();

	if(_drawLayerTextures)
		drawLayers();

	if(_displayTrees)
		drawTrees();
}

void Terrain :: drawSimpleTextured()
{
	CHECK_GL_ERROR();
	static int iteration = 0;
	++iteration;
	ShaderManager::instance()->begin("TexturedLayers");
	// Send uniforms

	GLfloat bleedBounds[3] = {_layerInfo[0]._bleed,_layerInfo[1]._bleed,_layerInfo[2]._bleed};
	GLfloat layerBounds[3] = {_layerInfo[0]._changeAt,_layerInfo[1]._changeAt,_layerInfo[2]._changeAt};
	ShaderManager::instance()->setUniform3fv("layerBounds",layerBounds);
	ShaderManager::instance()->setUniform3fv("bleedBounds",bleedBounds);
	ShaderManager::instance()->setUniform1fv("seaLevel",&_seaLevel);
	const float texcoordcorrected[4] = {_textureRepeats[0] / float(_dim),
										_textureRepeats[1] / float(_dim),
										_textureRepeats[2] / float(_dim),
										_textureRepeats[3] / float(_dim)};
	ShaderManager::instance()->setUniform4fv("texrepeats",texcoordcorrected);
	_tex->bind();
	ShaderManager::instance()->setUniform1i("heightmap",0);
	_texLayer[0]->bind(2);
	ShaderManager::instance()->setUniform1i("layer1map",2);
	_texLayer[1]->bind(3);
	ShaderManager::instance()->setUniform1i("layer2map",3);
	_texLayer[2]->bind(4);
	ShaderManager::instance()->setUniform1i("layer3map",4);
	_texLayer[3]->bind(5);
	ShaderManager::instance()->setUniform1i("layer4map",5);
	CHECK_GL_ERROR();

	glBegin(GL_QUADS);
		glTexCoord2i(0,_dim);
		glVertex2i(-1,1);
		glTexCoord2i(0,0);
		glVertex2i(-1,-1);
		glTexCoord2i(_dim,0);
		glVertex2i(1,-1);
		glTexCoord2i(_dim,_dim);
		glVertex2i(1,1);
	glEnd();

	TextureMgr::instance()->setTextureUnit(4);
	TextureMgr::instance()->setBoundTexture(0,4);
	TextureMgr::instance()->setTextureUnit(3);
	TextureMgr::instance()->setBoundTexture(0,3);
	TextureMgr::instance()->setTextureUnit(2);
	TextureMgr::instance()->setBoundTexture(0,2);
	TextureMgr::instance()->setTextureUnit(5);
	TextureMgr::instance()->setBoundTexture(0,5);
	TextureMgr::instance()->setTextureUnit(0);
	ShaderManager::instance()->end();
}

unsigned Terrain :: countValidHeights(const float low,const float high)
{
	_validTreeIndices.clear();
	unsigned count(0);
	if(_terrainTexData)
	{
		const unsigned total = _dim * _dim;
		for(unsigned i=0;i< total;++i)
			if((_terrainTexData[i] >= low) && (_terrainTexData[i] <= high))
			{
				++count;
				_validTreeIndices.push_back(i);
			}
	}
	return count;
}

// Return the ACTUAL number of generated trees
unsigned Terrain :: generateTreePositions(const float density,const unsigned interdistance)
{
	srand(unsigned(_validTreeIndices.size()));
	_actualTreeIndices.clear();
	unsigned treesToGenerate = unsigned(float(_validTreeIndices.size()) * density );
	unsigned i=0;
	while(i < treesToGenerate)
	{
		if(_validTreeIndices.empty())
			break;
		const unsigned curIndex = ((long(rand()) * long(rand())))%(unsigned(_validTreeIndices.size()));
		const unsigned pos = _validTreeIndices[curIndex];
		_actualTreeIndices.push_back(pos);
		_validTreeIndices.erase(_validTreeIndices.begin() + curIndex);
		// erase every index that points to the neighbours with max=interdistance
		const unsigned pos_y = pos/_dim;
		const unsigned pos_x = pos%_dim;
		for(int j=0;j<int(_validTreeIndices.size());++j)
		{
			//Find manhattan distance
			const unsigned npos = _validTreeIndices[j];
			const unsigned manh_h = ABS(int((npos/_dim) - pos_y));
			const unsigned manh_w = ABS(int((npos%_dim) - pos_x));
			if((manh_h + manh_w) <= interdistance)
			{
				_validTreeIndices.erase(_validTreeIndices.begin() + j);
				--j;
			}
		}
		++i;
	}
	return i;
}

void Terrain :: drawTrees()
{
	glColor3f(0.0f,1.0f,0.0f);
	TextureMgr::instance()->setBoundTexture(0,0);
	glBegin(GL_POINTS);
	const float denom = 1.0f / float(_dim);
	for(std::vector<unsigned>::iterator it =  _actualTreeIndices.begin() ; it != _actualTreeIndices.end() ; ++it)
	{
		glVertex2f(-1.0f + denom*2*((*it)%_dim),-1.0f + denom*2*((*it)/_dim));
	}
	glEnd();
}


void Terrain :: drawCraters()
{
	// Only in 2D
	if(_displayMode != D_3D)
	{
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		static unsigned CRATER_RES = 16;
		static const vec2f X_AXIS2f(1.0f,0.0f);
		static const vec2f Y_AXIS2f(0.0f,1.0f);
		for(size_t i=0;i<_craters.size();++i)
		{
			const vec2f normed_coords = vec2f(-1.0f,-1.0f) + 2.0f * (_craters[i].center * vec2f(1.0f,-1.0f) / (_xzscale*_dim));
			// First the crater radius
			glColor4f(1.0f,0.0f,0.0f,0.3f);
			const float normed_crater_radius = 2.0f*_craters[i].radius / (_xzscale*_dim);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2fv(normed_coords.get());
			for(unsigned j=0;j<=CRATER_RES;++j)
			{
				const float xpcent = cosf(math_const<float>::DEG2RAD*(j*360.0f/float(CRATER_RES)));
				const float ypcent = sinf(math_const<float>::DEG2RAD*(j*360.0f/float(CRATER_RES)));
				glVertex2f(normed_coords.x() + normed_crater_radius*xpcent,
						   normed_coords.y() + normed_crater_radius*ypcent);
			}
			glEnd();

			// Now the tform radius
			glColor4f(0.0f,0.0f,1.0f,0.3f);
			const float normed_tform_radius = 2.0f*_craters[i].tformradius / (_xzscale*_dim);
			glBegin(GL_TRIANGLE_FAN);
			glVertex2fv(normed_coords.get());
			for(unsigned j=0;j<=CRATER_RES;++j)
			{
				const float xpcent = cosf(math_const<float>::DEG2RAD*(j*360.0f/float(CRATER_RES)));
				const float ypcent = sinf(math_const<float>::DEG2RAD*(j*360.0f/float(CRATER_RES)));
				glVertex2f(normed_coords.x() + normed_tform_radius*xpcent,
						   normed_coords.y() + normed_tform_radius*ypcent);
			}
			glEnd();
		}
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glColor4f(1.0f,1.0f,1.0f,1.0f);
	}
}
void Terrain :: _initBuffers()
{
	// Generate buffers

	glGenBuffers(1,&_vbuffer);
	glGenBuffers(1,&_vtbuffer);
	glGenBuffers(1,&_ibuffer);

	// Generate index data
	const unsigned index_num = 2*(_dim + 1)*(_dim - 1);
	unsigned * indices = MemMgrRaw::instance()->allocate<unsigned>(index_num);
	unsigned iindex = 0;
	for(unsigned i=(_dim-1);i>0;--i)
	{
		for(unsigned j=0;j<_dim;++j)
		{
			indices[iindex++] = i*_dim + j;
			indices[iindex++] = (i-1)*_dim + j;
		}
		indices[iindex++] = (i-1)*_dim + (_dim -1); //plus max j
		indices[iindex++] = (i-1)*_dim;				// plus zero j
	}

	// Generate texcoord data
	vec2f * texcoords = MemMgrRaw::instance()->allocate<vec2f>(_dim*_dim);
	const double denom = 1.0 / double(_dim - 1);
	for(unsigned i=0;i<_dim;++i)
	{
		for(unsigned j=0;j<_dim;++j)
		{
			const unsigned index = i*_dim + j;
			texcoords[index].x() = float(j*denom);
			texcoords[index].y() = float(i*denom);
		}
	}

	// Initialize buffers

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,index_num*sizeof(unsigned),indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);
	glBufferData(GL_ARRAY_BUFFER, _dim*_dim*sizeof(vec3f), NULL, GL_DYNAMIC_DRAW);
	glVertexPointer(3, GL_FLOAT, sizeof(vec3f), 0);

	glBindBuffer(GL_ARRAY_BUFFER, _vtbuffer);
	glBufferData(GL_ARRAY_BUFFER, _dim*_dim*sizeof(vec3f), texcoords, GL_STATIC_DRAW);
	glTexCoordPointer(2,GL_FLOAT, sizeof(vec2f), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	MemMgrRaw::instance()->free(indices);
	MemMgrRaw::instance()->free(texcoords);
}

void Terrain :: update3DMode()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);
	glBufferData(GL_ARRAY_BUFFER, _dim*_dim*sizeof(vec3f), _terrainPosData, GL_DYNAMIC_DRAW);
	glVertexPointer(3, GL_FLOAT, sizeof(vec3f), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void Terrain :: draw3D()
{
	glColor3fv(_lightAmbDiff.get());
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	glPushMatrix();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);
	glVertexPointer(3, GL_FLOAT, sizeof(vec3f), 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	_lightMap->bind();

	glBindBuffer(GL_ARRAY_BUFFER, _vtbuffer);
	glTexCoordPointer(2,GL_FLOAT,sizeof(vec2f), 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	const unsigned total_indices = 2*(_dim + 1)*(_dim-1);
	glDrawRangeElements(GL_TRIANGLE_STRIP,
						0,total_indices-1,
						(GLsizei)total_indices,
						GL_UNSIGNED_INT,
						BUFFER_OFFSET(0 * sizeof(unsigned int)));

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glPopMatrix();

	glPopClientAttrib();
	glColor3f(1.0f,1.0f,1.0f);
}

void Terrain :: drawFull3D()
{

	ShaderManager::instance()->begin("lightmapTex");
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
	glPushMatrix();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);
	glVertexPointer(3, GL_FLOAT, sizeof(vec3f), 0);
	glEnableClientState(GL_VERTEX_ARRAY);

	_lightMap->bind();
	_lightMap->setParam(GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	_lightMap->setParam(GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	ShaderManager::instance()->setUniform1i("lightmap",0);

	glBindBuffer(GL_ARRAY_BUFFER, _vtbuffer);
	glTexCoordPointer(2,GL_FLOAT,sizeof(vec2f), 0);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	_contribTex->bind(1);
	ShaderManager::instance()->setUniform1i("contributionTex",1);

	_texLayer[0]->bind(2);
	ShaderManager::instance()->setUniform1i("layer1map",2);
	_texLayer[1]->bind(3);
	ShaderManager::instance()->setUniform1i("layer2map",3);
	_texLayer[2]->bind(4);
	ShaderManager::instance()->setUniform1i("layer3map",4);
	_texLayer[3]->bind(5);
	ShaderManager::instance()->setUniform1i("layer4map",5);

	ShaderManager::instance()->setUniform4fv("lightAmbDiff",(GLfloat *)(_lightAmbDiff.get()));

	float coordmod[4] = {_textureRepeats[0],
						_textureRepeats[1],
						_textureRepeats[2],
						_textureRepeats[3]};
	ShaderManager::instance()->setUniform4fv("coordModifier",coordmod);


	const unsigned total_indices = 2*(_dim + 1)*(_dim-1);
	glDrawRangeElements(GL_TRIANGLE_STRIP,
						0,total_indices-1,
						(GLsizei)total_indices,
						GL_UNSIGNED_INT,
						BUFFER_OFFSET(0 * sizeof(unsigned int)));

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glPopMatrix();

	glPopClientAttrib();
	ShaderManager::instance()->end();

	TextureMgr::instance()->setTextureUnit(5);
	TextureMgr::instance()->setBoundTexture(0,5);
	TextureMgr::instance()->setTextureUnit(4);
	TextureMgr::instance()->setBoundTexture(0,4);
	TextureMgr::instance()->setTextureUnit(3);
	TextureMgr::instance()->setBoundTexture(0,3);
	TextureMgr::instance()->setTextureUnit(2);
	TextureMgr::instance()->setBoundTexture(0,2);
	TextureMgr::instance()->setTextureUnit(1);
	TextureMgr::instance()->setBoundTexture(0,1);
	TextureMgr::instance()->setTextureUnit(0);
}

void Terrain :: _initLightmap()
{
	vector<MipmapLevel> ml;
	ml.push_back(MipmapLevel((unsigned char *)NULL,0));
	_lightMap = new Texture2D(_lightmapTexDim,_lightmapTexDim,GL_RGB,GL_RGB,GL_UNSIGNED_BYTE,ml,GL_TEXTURE_2D,"TerrainLightmap",false,false);
}

void Terrain :: generateTerraMap()
{
	// bind the shader & pass the uniforms

	ShaderManager::instance()->begin("lightmapTexBlur");
	_texLayer[3]->bind(5);
	ShaderManager::instance()->setUniform1i("layer4map",5);
	_texLayer[2]->bind(4);
	ShaderManager::instance()->setUniform1i("layer3map",4);
	_texLayer[1]->bind(3);
	ShaderManager::instance()->setUniform1i("layer2map",3);
	_texLayer[0]->bind(2);
	ShaderManager::instance()->setUniform1i("layer1map",2);
	_contribTex->bind(1);
	ShaderManager::instance()->setUniform1i("contributionTex",1);

	ShaderManager::instance()->setUniform4fv("lightAmbDiff",(GLfloat *)(_lightAmbDiff.get()));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,0);

	_lightMap->bind();
	ShaderManager::instance()->setUniform1i("lightmap",0);	

	float coordmod[4] = {_textureRepeats[0],
						_textureRepeats[1],
						_textureRepeats[2],
						_textureRepeats[3]};
	ShaderManager::instance()->setUniform4fv("coordModifier",coordmod);
	const float step = 1.0f / _lightmapTexDim;
	ShaderManager::instance()->setUniform1fv("step",&step);
	
	CHECK_GL_ERROR();

	// Prepare the FBO

	const unsigned reddim = _outTexDim;

	GLint _curdrawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&_curdrawbuf);
	_fboTerra.Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	int vp[4];
	CHECK_GL_ERROR();
	glGetIntegerv(GL_VIEWPORT,vp);
	glViewport(0,0,reddim,reddim);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	CHECK_GL_ERROR();
	glOrtho(0,reddim,0,reddim,-1,1);
	CHECK_GL_ERROR();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
		glTexCoord2f(0,1);
		glVertex2i(0,reddim);
		glTexCoord2f(0,0);
		glVertex2i(0,0);
		glTexCoord2f(1,0);
		glVertex2i(reddim,0);
		glTexCoord2f(1,1);
		glVertex2i(reddim,reddim);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	FramebufferObject::Disable();
	glDrawBuffer(_curdrawbuf);
	glViewport(vp[0],vp[1],vp[2],vp[3]);

	ShaderManager::instance()->end();

	TextureMgr::instance()->setTextureUnit(5);
	TextureMgr::instance()->setBoundTexture(0,5);
	TextureMgr::instance()->setTextureUnit(4);
	TextureMgr::instance()->setBoundTexture(0,4);
	TextureMgr::instance()->setTextureUnit(3);
	TextureMgr::instance()->setBoundTexture(0,3);
	TextureMgr::instance()->setTextureUnit(2);
	TextureMgr::instance()->setBoundTexture(0,2);
	TextureMgr::instance()->setTextureUnit(1);
	TextureMgr::instance()->setBoundTexture(0,1);
	TextureMgr::instance()->setTextureUnit(0);
	TextureMgr::instance()->setBoundTexture(0,0);

	TextureIO::instance()->saveImageFullPath(_terraTex,outDirectory() + _levelName + string("_") +string(_layerTform ? "TForm" : "Barren")+ string(".sgi"));

	CHECK_GL_ERROR();
}

void Terrain :: generateTerraOBJ(const float rescale)
{
	// GENERATE MTL FILE 
	const string mtlfile(outDirectory() + _levelName + string("_Model.mtl"));

	ofstream mtlofs(mtlfile.c_str());
	mtlofs<<"newmtl terrainMat"<<endl;
	mtlofs<<"Ka 1.000000 1.000000 1.000000"<<endl;
	mtlofs<<"Kd 0.000000 0.000000 0.000000"<<endl;
	mtlofs<<"Ks 0.000000 0.000000 0.000000"<<endl;
	mtlofs<<"map_Ka "<<_levelName + string("_") +string(_layerTform ? "TForm" : "Barren")+ string(".sgi")<<endl;
	mtlofs.close();

	// GENERATE OBJ FILE
	const string objfile(outDirectory() + _levelName + string("_Model.obj"));
	ofstream objofs(objfile.c_str());
	objofs<<"mtllib "<<_levelName<<"_Model.mtl"<<endl<<endl;

	// Write the vertex data
	vec3f offset_to_center(-_xzscale*_dim*0.5f,-_seaLevel*_yscale,_xzscale*_dim*0.5f);
	const float rescale_mod = rescale / (_xzscale*_dim);

	for(unsigned i=0;i<_dim;++i)
	{
		for(unsigned j=0;j<_dim;++j)
		{
			const unsigned linindex = i*_dim + j;
			const vec3f rescaled = (_terrainPosData[linindex] + offset_to_center)*rescale_mod;
			objofs<<"v "<<rescaled.x()<<" "<<rescaled.y()<<" "<<rescaled.z()<<endl;
			objofs<<"vt "<<float(j / float(_dim - 1))<<" "<<float(i / float(_dim - 1))<<endl;
		}
	}

	// write the faces
	objofs<<"usemtl terrainMat"<<endl;
	objofs<<"g terrainMat"<<endl<<endl;
	for(unsigned i=0;i<(_dim-1);++i)
	{
		for(unsigned j=0;j<(_dim-1);++j)
		{
			// Throw 2 triangles : 
			// index, index + _dim + 1, index + _dim
			// index, index + 1, index + _dim + 1
			const unsigned ll = (i*_dim + j) + 1;
			const unsigned lr = ll + 1;
			const unsigned ul = ll + _dim;
			const unsigned ur = lr + _dim;
			objofs<<"f "<<ll<<"/"<<ll<<" "<<ur<<"/"<<ur<<" "<<ul<<"/"<<ul<<endl;
			objofs<<"f "<<ll<<"/"<<ll<<" "<<lr<<"/"<<lr<<" "<<ur<<"/"<<ur<<endl;
		}
	}
	

	objofs.close();
}

void Terrain :: generateTerraData(const float rescale)
{
	// GENERATE DATA FILE 


	const string datafile(outDirectory() + _levelName + string("_Data.dat"));
	FILE * fp = fopen(datafile.c_str(),"wb");
	if(!fp)
	{
		assert(0);
		return;
	}

	// Save the map dimension
	// FIXME : see why shit happens here
	fwrite(&_dim,sizeof(_dim),1,fp);
	// Save the position data
	vec3f offset_to_center(-_xzscale*_dim*0.5f,-_seaLevel*_yscale,_xzscale*_dim*0.5f);
	const float rescale_mod = rescale / (_xzscale*_dim);
	vec3f * posdata = MemMgrRaw::instance()->allocate<vec3f>(_dim*_dim);
	for(unsigned i=0;i<_dim;++i)
	{
		for(unsigned j=0;j<_dim;++j)
		{
			const unsigned linindex = i*_dim + j;
			posdata[linindex] = (_terrainPosData[linindex] + offset_to_center)*rescale_mod;
		}
	}
	fwrite(posdata,sizeof(vec3f),_dim*_dim,fp);
	MemMgrRaw::instance()->free(posdata);

	// save the texcoord data
	glBindBuffer(GL_ARRAY_BUFFER,_vtbuffer);
	vec2f * vtdata = (vec2f *)glMapBuffer(GL_ARRAY_BUFFER,GL_READ_ONLY);
	fwrite(vtdata,sizeof(vec2f),_dim*_dim,fp);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	glBindBuffer(GL_ARRAY_BUFFER,0);

	// save the index data
	const unsigned index_num = 2*(_dim + 1)*(_dim - 1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);
	unsigned * vidata = (unsigned *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,GL_READ_ONLY);
	fwrite(vidata,sizeof(unsigned),index_num,fp);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	fclose(fp);
	CHECK_GL_ERROR();

	// GENERATE TREE POSITIONS FILE 
	const string treefile(outDirectory() + _levelName + string("_Treepos.bin"));
	FILE * fp3 = fopen(treefile.c_str(),"wb");

	unsigned treenum = unsigned(_actualTreeIndices.size());
	fwrite(&treenum,sizeof(unsigned),1,fp3);

	vec3f * tree_positions = MemMgrRaw::instance()->allocate<vec3f>(treenum);
	for(unsigned i=0;i<treenum;++i)
	{
		tree_positions[i] = (_terrainPosData[_actualTreeIndices[i]] + offset_to_center)*rescale_mod;
	}
	fwrite(tree_positions,sizeof(vec3f),treenum,fp3);
	MemMgrRaw::instance()->free(tree_positions);
	
	fclose(fp3);

	// GENERATE INFO FILE 
	const string infofile = outDirectory() + _levelName + string("_Info.txt");
	ofstream ofs(infofile.c_str());
	ofs<<"[Misc]\n"<<endl;
	ofs<<"TreePositions = "<<_levelName + string("_Treepos.bin")<<endl;
	const vec3f ldirnorm = normalize(_lightdir);
	ofs<<"LightDir = "<<"("<<vec4f(ldirnorm.x(),ldirnorm.y(),ldirnorm.z(),0.0f)<<")"<<endl;
	ofs<<"LightAmbDiff = "<<"("<<_lightAmbDiff<<")"<<endl;
	ofs<<"MapCellDim = "<<_dim<<endl;
	ofs<<"MapExtents= "<<"("<<vec3f(rescale,(1.0f - _seaLevel)*_yscale*rescale_mod,rescale)<<")"<<endl;
	// TODO : Tree model list here?

	ofs<<"\n[DataFiles]\n"<<endl;

	ofs<<"TerraformTexture = "<<_levelName + string("_TForm.sgi")<<endl;
	ofs<<"BarrenTexture = "<<_levelName + string("_Barren.sgi")<<endl;
	ofs<<"TerrainData = "<<_levelName + string("_Data.dat")<<endl;

	for(size_t i=0;i<_craters.size();++i)
	{
		ofs<<"\n[Craters : Crater"<<ToString<size_t>(i)<<"]\n"<<endl;
		ofs<<"Location = "<<"("<<(_craters[i].center + vec2f(offset_to_center.x(),offset_to_center.z()))*rescale_mod<<")"<<endl;
		ofs<<"CraterRadius = "<<_craters[i].radius*rescale_mod<<endl;
		ofs<<"AffectedAreaRadius = "<<_craters[i].tformradius*rescale_mod<<endl;
	}

	ofs.close();


}

void Terrain :: switchTextureLayers()
{
	if(_layerTform)
	{
		for(int i=0;i<4;++i)
			_texLayer[i] = _barrenLayer[i];
		_layerTform = false;
	}
	else
	{
		for(int i=0;i<4;++i)
			_texLayer[i] = _tformLayer[i];
		_layerTform = true;
	}
}

void Terrain :: undoCrater()
{
	if(_canUndo)
	{
		_craters.pop_back();
		// copy texbck to tex;
		float * hdata = MemMgrRaw::instance()->allocate<float>(_dim*_dim);
		_texbck->dlData(hdata);
		_tex->ulData(hdata);
		MemMgrRaw::instance()->free(hdata);
		updatePositionsNormals();
		updateContribTex();
		_canUndo = false;
	}
}

void Terrain :: createCrater(const float linbell_mix,
							 const float bell_area,
							 const float radius,
							 const vec2<float>& center,
							 const float oldmapmix)
{
	// read bck, write temp
	// bind the shader & pass the uniforms
	ShaderManager::instance()->begin("computeCrater");
	_texbck->bind();
	ShaderManager::instance()->setUniform1i("heightmap",0);

	const vec2<float> c(float(center.x()),float(center.y()));

	ShaderManager::instance()->setUniform1fv("linbell_mix",&linbell_mix);
	ShaderManager::instance()->setUniform1fv("bell_area",&bell_area);
	ShaderManager::instance()->setUniform1fv("radius",&radius);
	ShaderManager::instance()->setUniform2fv("center",(GLfloat *)(c.get()));
	ShaderManager::instance()->setUniform1fv("oldmap_mix",&oldmapmix);
	ShaderManager::instance()->setUniform1fv("xzscale",&_xzscale);

	// Prepare the FBO

	GLint _curdrawbuf;
	glGetIntegerv(GL_DRAW_BUFFER,&_curdrawbuf);
	_fboCrater.Bind();
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	int vp[4];
	CHECK_GL_ERROR();
	glGetIntegerv(GL_VIEWPORT,vp);
	glViewport(0,0,_dim,_dim);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	CHECK_GL_ERROR();
	glOrtho(0,_dim,0,_dim,-1,1);
	CHECK_GL_ERROR();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glBegin(GL_QUADS);
		glTexCoord2i(0,_dim);
		glVertex2i(0,_dim);
		glTexCoord2i(0,0);
		glVertex2i(0,0);
		glTexCoord2i(_dim,0);
		glVertex2i(_dim,0);
		glTexCoord2i(_dim,_dim);
		glVertex2i(_dim,_dim);
	glEnd();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	FramebufferObject::Disable();
	glDrawBuffer(_curdrawbuf);
	glViewport(vp[0],vp[1],vp[2],vp[3]);

	ShaderManager::instance()->end();
	TextureMgr::instance()->setBoundTexture(0,1);
	TextureMgr::instance()->setTextureUnit(0);

	// now convert from _heightRGB to _tex
	// updatePositionNormal
	// updateContribTex
	float * hdatargb = MemMgrRaw::instance()->allocate<float>(_dim*_dim*3);
	_heightRGB->dlData(hdatargb);
	for(unsigned i=0;i<(_dim*_dim);++i)
		_terrainTexData[i] = hdatargb[i*3];
	_tex->ulData(_terrainTexData);

	MemMgrRaw::instance()->free(hdatargb);

	updatePositionsNormals();

	CHECK_GL_ERROR();
}

void Terrain :: finalizeCrater(const float linbell_mix,
							 const float bell_area,
							 const float radius,
							 const float tformradius,
							 const vec2<float>& center,
							 const float oldmapmix)
{
	Crater cr;
	cr.center.x() = float(center.x());
	cr.center.y() = float(center.y());
	cr.radius = radius;
	cr.tformradius = tformradius;
	_craters.push_back(cr);

	_canUndo = true;

	_tex->dlData(_terrainTexData);
	_texbck->ulData(_terrainTexData);

	createCrater(linbell_mix,bell_area,radius,center,oldmapmix);
	updateContribTex();
}


void Terrain :: setTFormTexture(const unsigned i,Texture * tex)
{
	if(tex && (i < 4))
	{
		_tformLayer[i] = tex;
		switchTextureLayers();
		switchTextureLayers();
	}
}

void Terrain :: setBarrenTexture(const unsigned i,Texture * tex)
{
	if(tex && (i < 4))
	{
		_barrenLayer[i] = tex;
		switchTextureLayers();
		switchTextureLayers();
	}
}