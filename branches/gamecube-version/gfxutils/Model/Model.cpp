#include "Model.h"
#include "../../control/Gamecube/gc_new_delete.h"
#include "../../utility/RawFile.h"
#include "../VA/VertexArray.h"
#include "../VA/VertexFormat.h"
#include "ModelMgr.h"
#include "../Texture/TextureMgr.h"
#include "../Texture/Texture.h"
#include <stdio.h>

Model :: Model(const std::string& fname)	// read a model from file : ModelMgr does this
:m_isTextured(false),
m_notexVA(0),
m_texVA(0),
m_name(fname),
m_notexData(0),
m_texData(0),
m_texture(0)
{
	_loadOBJ(ModelMgr::instance().getModelDir() + fname);
	if(m_isTextured)
	{
		// HACK_YEAH! the only tex model is enforcer, so grab the tex
		m_texture = TextureMgr::instance().getTexture("enforcer");
	}
}

Model :: ~Model()
{
	if(m_isTextured)
	{
		delete [] m_texData->xyz;
		delete [] m_texData->nrm;
		delete [] m_texData->st;
		delete m_texData;
		delete m_texVA;
	}
	else
	{
		delete [] m_notexData->xyz;
		delete [] m_notexData->nrm;
		delete m_notexData;
		delete m_notexVA;
	}
}

void Model :: render() const
{
	if(m_isTextured)
	{
		m_texture->bind();
		m_texVA->call();
	}
	else
		m_notexVA->call();
}