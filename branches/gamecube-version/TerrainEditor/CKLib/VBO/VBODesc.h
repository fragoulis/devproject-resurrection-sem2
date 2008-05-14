#pragma once 

#include <vector>
#include <VBO/VBO.h>

namespace CKLib {

class VBO;
class VertexAttribute;

/*
	class - descriptor for a VBO renderable chunk 
*/
class VBODesc
{
	unsigned int _start_i,
				 _size_i,
				 _start_d,
				 _size_d,
				 _drawMode;
	VBO			*_vbo;

public :
	
	VBODesc(VBO * vbo,
			std::vector<const VertexAttribute *> attrib,
			std::vector<void *> attrib_data,
			unsigned data_num,
			unsigned * index_data,
			unsigned index_num,
			unsigned drawMode);

	void call() const;
	void mapVertexData(const VertexAttribute * attrib, const unsigned access,void *& data,unsigned& num) const;
	void unmapVertexData(const VertexAttribute * attrib, void *& data) const;
	void mapIndexData(const unsigned access,void *& data,unsigned& num) const;
	void unmapIndexData(void *& data) const;

	const unsigned start_i() const;
	const unsigned start_d() const;
	const unsigned size_i() const;
	const unsigned size_d() const;
	const unsigned drawMode() const;

};

#include <VBO/VBODesc.inl>

}