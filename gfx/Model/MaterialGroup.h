#pragma once

#include "../Material.h"
#include "VBODesc.h"


class Texture;
class VBO;
class MaterialGroup
{
	int					   m_shaderIndex;
	std::vector<Texture *> m_textureList;
	Material			   m_material;
	VBODesc				   m_vboDesc;
	

public :
	MaterialGroup(const Material& mat,
				  const std::vector<Texture *>& texlist,
				  const VBODesc& vbodesc,
				  const int shaderi = -1)
				  :m_shaderIndex(-1),m_textureList(texlist),
				  m_material(mat),m_vboDesc(vbodesc){}

	~MaterialGroup(){}

	// Accessors
	const int getShaderIndex() const {return m_shaderIndex;}
	const std::vector<Texture *>& getTextureList() const {return m_textureList;}
	const Material& getMaterial() const {return m_material;}
	const VBODesc& getVboDesc() const {return m_vboDesc;}

	//Mutators
	void setShaderIndex(const int i) {m_shaderIndex = i;}
	Material& material() {return m_material;}
	std::vector<Texture *>& textureList() {return m_textureList;}
	VBODesc& vboDesc() {return m_vboDesc;}
};