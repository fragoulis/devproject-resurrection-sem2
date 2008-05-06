#ifndef _VBO_h_
#define _VBO_h_

#include <string>
#include <map>
#include <vector>
#include <Math/vec3.h>
#include <Math/vec2.h>
#include <Shaders/VertexAttrib.h>
#include <VBO/VBOMgr.h>

namespace CKLib {


/*
	VBO : 
	Vertex Buffer Object class. Each VBO can hold many objects, if they have the same attributes
	in number & type. 
	
*/
class VBO
{
	unsigned						_ibuffer;
	unsigned						_buffer_num;		// so we don't call _buffer_info.size()
	VAttribStatus					_potentialAttribs;	// the existent attribs are 1'ed
	unsigned						_attribBuffers[ATTRIB_NUM];
	unsigned						_used_data,
									_used_index;
	std::vector<vec2<unsigned> >	_ifreeSpace,_dfreeSpace;
	unsigned						_drawUsage;					// MAke this static by def
	
	unsigned						_dataSize;
	unsigned						_indexSize;
	std::string						_configName;
	static VBO *					_active;

	static VAttribStatus			_enabledAttribs;
	static VAttribStatus			_currentFlags;

public : 

	VBO(std::vector<const VertexAttribute *> attribs,
						   const unsigned maxVertices,
						   const unsigned maxIndices,
						   const unsigned usage,
						   const std::string& configName);
	~VBO();

	// access
	const unsigned dataOffset() const;
	const unsigned indexOffset() const;
	//unsigned& dataOffset();
	//unsigned& indexOffset();

	const std::string& getConfiguration() const;

	const unsigned attrib_num() const;
	const VertexAttribute * attrib(const unsigned& i) const;
	const std::vector<const VertexAttribute *> attribs() const;
	unsigned drawUsage() const;
	void drawUsage(const unsigned flag);

	// Mapping funcs
	void map(const unsigned& start,
			 const unsigned& access,
			 std::vector<const VertexAttribute *> vattrs,
			 std::vector<void *>& mem) const;
	void map(const unsigned& start,
			 const unsigned& access,
			 const VertexAttribute * vattr,
			 void *& mem) const;
	void unmap(std::vector<const VertexAttribute *> vattrs) const;
	void unmap(const VertexAttribute * vattr) const;
	void mapIndices(const unsigned& start,const unsigned& access,void ** mem) const;
	void unmapIndices() const;

	// utilities
	void sendData(const VertexAttribute * vattr,
				  const void * mem,
				  const unsigned pos,const unsigned count) const;
	void sendIndexData(const void * mem,
					  const unsigned pos,const unsigned count) const;
	void getData(const VertexAttribute * vattr,
				  void *& mem,
				  const unsigned pos,const unsigned count) const;
	void getIndexData(void *& mem,
					  const unsigned pos,const unsigned count) const;
	// In setup, define usage, & in which attributes
	void setup(const std::vector<const VertexAttribute *>& vattrs);
	void call(const unsigned start,const unsigned size,const unsigned drawmode);

	bool querySetData(const unsigned d_size,const unsigned i_size,unsigned& d_pos,unsigned& i_pos);
	bool querySize(const unsigned d_size,const unsigned i_size);
	void freeData(const unsigned d_size,const unsigned i_size,const unsigned& d_pos,const unsigned& i_pos);

	void addAttribute(const VertexAttribute * vattr);

	// static funcs
	const unsigned dataSize();
	const unsigned indexSize();

	static const unsigned char enabledAttribStatus(const unsigned i) {return _enabledAttribs.attrib[i];}
	static void setEnabledAttribStatus(const unsigned i,const unsigned char val) {_enabledAttribs.attrib[i] = val;}
	static VAttribStatus& enabledAttribs() {return _enabledAttribs;}

	static const std::vector<VBO *> list();
};

#include <VBO/vbo.inl>

}

#endif