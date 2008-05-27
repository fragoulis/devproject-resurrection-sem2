#ifndef __Model_h__
#define __Model_h__

#include "../VA/VertexFormat.h"
#include "../VA/VertexArray.h"

#include <string>

class Texture;
class Model
{
	bool m_isTextured;
	VertexArray<VFMT4> * m_notexVA;
	VertexArray<VFMT5> * m_texVA;
	std::string			 m_name;

	VFMT4 * m_notexData;
	VFMT5 * m_texData;

	Texture * m_texture;

	unsigned m_polynum;

	// bounding box for shadows?
	void _loadOBJ(const std::string& fname, bool loadTex = true);

public :

	Model(const std::string& fname);	// read a model from file : ModelMgr does this
	~Model();
	void render() const;
	const std::string& getName() const {return m_name;}

	void setTexture();
	bool isTextured() const {return m_isTextured;}

	const unsigned getPolynum() const {return m_polynum;}

	// bbox getBBox() const? 
};

#endif