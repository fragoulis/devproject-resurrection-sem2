#pragma once

#include <string>
#include <vector>
#include <glee.h>
#include <Math/vec3.h>
#include <FBO/FramebufferObject.h>
#include <Camera/Camera.h>
#include <Math/vec2.h>
#include <Math/vec4.h>

namespace CKLib {

class ParserSection;
class Texture;

struct LayerInfo
{
	float _changeAt,
		  _bleed;
};

enum DisplayMode
{
	D_BW = 0,
	D_COL,
	D_TEX,
	D_NORM,
	D_3D
};

struct Crater
{
	vec2<float> center;		// world coords
	float		radius;		// world size
	float		tformradius; // world size
};


/*
	The terrain super-structure.
	Holds everything needed.
*/

class Terrain
{
	// Data
	unsigned short * _terrainData;			// unnormalized data
	float * _terrainTexData;				// normalized 0-1 data
	vec3f * _terrainNormalData;				// surface normal data
	vec3f * _terrainPosData;				// position data

	FramebufferObject _fbo,_fbolm,_fboContrib,_fboTerra,_fboCrater;	

	// Tree stuff
	std::vector<unsigned> _validTreeIndices;
	std::vector<unsigned> _actualTreeIndices;

	// VBO buffers
	GLuint _vbuffer,	
		   _vtbuffer,
		   _ibuffer;

	// terrain params
	unsigned _dim,				// terrain dimension
			 _outTexDim,		// output texture dimension
			 _lightmapTexDim;	// lightmap texture dimension
	LayerInfo _layerInfo[3];	// layer information at change points
	float _seaLevel;			// sealevel percent
	float _xzscale;				// scale of x-z terrain values
	float _yscale;				// scale of y terrain values
	float _shadowEffect;		// percentage of shadow effect
	bool  _canUndo;				// if the user can undo a crater

	std::string _exportDir;		// export directory
	std::string _levelName;		// output level name

	// Textures
	Texture * _tex, * _texbck;
	Texture *_texLayer[4],
			*_normalTex,
			*_posTex,
			*_lightMap,
			*_contribTex,
			*_terraTex,
			*_tformLayer[4],
			*_barrenLayer[4],
			*_heightRGB;

	vec3f	_lightdir;			// light direction
	vec4f	_lightAmbDiff;		// light ambient - diffuse components

	bool	_layerTform;		// if we're at terraform layer

	std::vector<Crater> _craters;	// crater list

	// display params

	bool _drawLayerTextures;
	DisplayMode _displayMode;
	float _textureRepeats[4];
	bool _useLightmap;
	bool _drawTexturedLightmap;
	bool _displayTrees;
	bool _displayCraters;

	// Funcs

	void _loadRawData(const std::string& fname);
	void _initBuffers();
	void _initLightmap();

	// draw funcs

	void drawGrayscale();
	void drawColourLayered();
	void drawLayers();
	void drawSimpleTextured();
	void drawNormals();
	void draw3D();
	void drawFull3D();

	void drawTrees();
	void drawCraters();

public :

	Terrain(const ParserSection * parsec);
	~Terrain();

	unsigned getDim();

	// Accessors
	LayerInfo& layerInfo(const unsigned i);
	Texture * layerTexture(const unsigned i);
	float & seaLevel();
	const unsigned dim() const {return _dim;}
	DisplayMode& displayMode() {return _displayMode;}
	bool& drawLayerTextures() {return _drawLayerTextures;}
	float& textureRepeats(const unsigned i);
	float& xzscale() {return _xzscale;}
	float& yscale() {return _yscale;}
	vec3f& lightdir() {return _lightdir;}
	bool& useLightmap() {return _useLightmap;}
	float& shadowEffect() {return _shadowEffect;}
	bool & drawTexturedLightmap() {return _drawTexturedLightmap;}
	std::vector<Crater>& craters() {return _craters;}
	const bool& canUndoCrater() const {return _canUndo;}
	bool& displayTrees() {return _displayTrees;}
	const std::string outDirectory() const {return _exportDir + _levelName + string("\\");}
	vec4f& lightColor() {return _lightAmbDiff;}
	bool& displayCraters() {return _displayCraters;}

	// funcs
	void draw();

	void setTFormTexture(const unsigned i,Texture * tex);
	void setBarrenTexture(const unsigned i,Texture * tex);

	unsigned generateTreePositions(const float density,const unsigned interdistance);
	unsigned countValidHeights(const float low,const float high);
	void update3DMode();
	void updatePositionsNormals();
	void updateLightmap();
	void updateContribTex();

	void generateTerraMap();
	void generateTerraData(const float rescale);
	void generateTerraOBJ(const float rescale);
	void switchTextureLayers();

	void createCrater(const float linbell_mix,
					 const float bell_area,
					 const float radius,
					 const vec2<float>& center,
					 const float oldmapmix);
	void finalizeCrater(const float linbell_mix,
					 const float bell_area,
					 const float radius,
					 const float tformradius,
					 const vec2<float>& center,
					 const float oldmapmix);
	void undoCrater();
	
};

}