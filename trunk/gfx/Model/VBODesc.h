#pragma once

#include <vector>

/*
	VBO descriptor class : 
	describes a renderable portion of a VBO
*/

class VBO;
class VertexAttribute;
class VBODesc
{
	unsigned			   m_iStart,
						   m_iSize,
						   m_dStart,
						   m_dSize,
						   m_drawMode;
	VBO					 * m_vbo;

public : 

	VBODesc(VBO * vbo,
			std::vector<const VertexAttribute *> attrib,
			std::vector<void *> attrib_data,
			unsigned data_num,
			unsigned * index_data,
			unsigned index_num,
			unsigned drawMode);

	// Accessors
	const unsigned getIStart() const {return m_iStart;}
	const unsigned getISize() const {return m_iSize;}
	const unsigned getDStart() const {return m_dStart;}
	const unsigned getDSize() const {return m_dSize;}
	const unsigned getDrawMode() const {return m_drawMode;}
	const VBO * getVBO() const {return m_vbo;}

	// Mutators
	VBO * getVBO() {return m_vbo;}

	// Funcs
	void call() const;
};