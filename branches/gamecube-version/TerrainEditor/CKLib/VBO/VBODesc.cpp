#include <VBO/VBODesc.h>
#include <Shaders/VertexAttrib.h>
#include <VBO/VBO.h>

using namespace CKLib;
using namespace std;

VBODesc :: VBODesc(VBO * vbo,
			std::vector<const VertexAttribute *> attrib,
			std::vector<void *> attrib_data,
			unsigned data_num,
			unsigned * index_data,
			unsigned index_num,
			unsigned drawMode)
:_size_i(index_num),
_size_d(data_num),
_vbo(vbo),
_drawMode(drawMode)
{
	vbo->querySetData(data_num,index_num,_start_d,_start_i);
	// Correct indices
	for(unsigned ii=0;ii<index_num;++ii)
		index_data[ii] += _start_d;

	// Send data
	vbo->sendIndexData(index_data,_start_i,index_num);

	for(unsigned k=0;k<attrib.size();++k)
		vbo->sendData(attrib[k],attrib_data[k],_start_d,data_num);
}

void VBODesc :: mapVertexData(const VertexAttribute * attrib, const unsigned access, void *& data,unsigned& num) const
{
	_vbo->map(_start_d,access,attrib,data);
	num = _size_d;
}

void VBODesc :: unmapVertexData(const VertexAttribute * attrib, void *& data) const
{
	_vbo->unmap(attrib);
	data = 0;
}