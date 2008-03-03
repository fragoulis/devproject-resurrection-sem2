#include "../VBO/vbo.h"
#include "../Shaders/ShaderManager.h"
#include "VBOMgr.h"
#include <gl/glee.h>
#include <sstream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

using namespace std;

VBO * VBO :: _active = 0;

VAttribStatus VBO :: _enabledAttribs(0);


VBO :: VBO(std::vector<const VertexAttribute *> attribs,
			const unsigned maxVertices,
			const unsigned maxIndices,
			const unsigned usage,
			const std::string& configName)
:_used_data(0),
_used_index(0),
_drawUsage(usage),
_ibuffer(0),
_buffer_num(0),
_dataSize(maxVertices),
_indexSize(maxIndices),
_configName(configName)
{
	for(unsigned i=0;i<ATTRIB_NUM;++i)
		_attribBuffers[i] = 0;
	setup(attribs);
}

VBO :: ~VBO()
{
	if(_ibuffer)
		glDeleteBuffers(1,&_ibuffer);
	for(unsigned i=0;i<ATTRIB_NUM;++i)
		if(_attribBuffers[i])
			glDeleteBuffers(1,&(_attribBuffers[i]));
}

inline static void setAttributePointer(const VertexAttribute * vattr)
{
	const string attr_name = vattr->name();
	if(attr_name == "Vertex")
		glVertexPointer(vattr->components(),vattr->datatype(),vattr->size(),0);
	else if(attr_name == "Normal")
		glNormalPointer(vattr->datatype(),vattr->size(),0);
	else if(attr_name == "Texcoord")
		glTexCoordPointer(vattr->components(),vattr->datatype(),vattr->size(),0);
	else if(attr_name == "Color")
		glColorPointer(vattr->components(),vattr->datatype(),vattr->size(),0);
	else
	{
		ASSERT(vattr->array_loc());
		glVertexAttribPointer(vattr->array_loc(),vattr->components(),vattr->datatype(),GL_FALSE,vattr->size(),0);
	}
}

void VBO :: setup(const std::vector<const VertexAttribute *>& vattrs)
{
	glGenBuffers(1,&_ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,_indexSize*sizeof(unsigned),0,_drawUsage);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	_ifreeSpace.push_back(vec2U(0,_indexSize));
	_dfreeSpace.push_back(vec2U(0,_dataSize));

	for(unsigned i=0;i<vattrs.size();++i)
	{
		const unsigned attrib_index = ShaderManager::instance()->vertexAttributeIndex(vattrs[i]);
		_potentialAttribs.attrib[attrib_index] = 0xFF;
		glGenBuffers(1,&(_attribBuffers[attrib_index]));

		glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[attrib_index]);
		glBufferData(GL_ARRAY_BUFFER,_dataSize*vattrs[i]->size(),0,_drawUsage);
		CHECK_GL_ERROR();
		++_buffer_num;
	}
}


void VBO :: call(const unsigned start,const unsigned size,const unsigned drawmode) const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);

	VAttribStatus cur_flags = VBOMgr::instance().getCurrentFlags().AND(_potentialAttribs);

	const unsigned total_attribs = unsigned(ShaderManager::instance()->vertexAttributeNum());
	if(_active == this)
	{
		// Need only the differences from the render flags & the currently enabled
		VAttribStatus to_change = cur_flags.XOR(_enabledAttribs);
		for(unsigned i=0;i<total_attribs;++i)
		{
			VertexAttribute * vattr = ShaderManager::instance()->vertexAttribute(i);
			if(to_change.attrib[i])
			{
				if(!_enabledAttribs.attrib[i])
				{
					ASSERT(_attribBuffers[i]);
					glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[i]);
					//CHECK_GL_ERROR();
					vattr->setAttribPointer();
					//CHECK_GL_ERROR();
					vattr->EnableClientArray();
					_enabledAttribs.attrib[i] = 0xFF;
					//CHECK_GL_ERROR();
				}
				else
				{
					vattr->DisableClientArray();
					_enabledAttribs.attrib[i] = 0;
				}
			}
		}
	}
	else
	{
		//cout<<"VBO changed from "<<_active<<" to "<<this<<endl;
		_active = const_cast<VBO *>(this);
		VAttribStatus to_change = cur_flags.OR(_enabledAttribs);
		for(unsigned i=0;i<total_attribs;++i)
		{
			VertexAttribute * vattr = ShaderManager::instance()->vertexAttribute(i);
			if(to_change.attrib[i])
			{
				if(cur_flags.attrib[i])	// MUST BE ON, reload
				{
					ASSERT(_attribBuffers[i]);
					glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[i]);
					//CHECK_GL_ERROR();
					vattr->setAttribPointer();
					//CHECK_GL_ERROR();
					if(!_enabledAttribs.attrib[i])
					{
						vattr->EnableClientArray();
						_enabledAttribs.attrib[i] = 0xFF;
						//CHECK_GL_ERROR();
					}
				}
				else
				{
					vattr->DisableClientArray();
					_enabledAttribs.attrib[i] = 0;
				}
			}
		}
	}


	//CHECK_GL_ERROR();					
	glDrawRangeElements(drawmode,
						start,
						start+size-1,
						size,
						GL_UNSIGNED_INT,
						BUFFER_OFFSET(start*sizeof(unsigned)));

	for(unsigned i=0;i<total_attribs;++i)
	{
		ShaderManager::instance()->vertexAttribute(i)->DisableClientArray();
		_enabledAttribs.attrib[i] = 0;
		glBindBuffer(GL_ARRAY_BUFFER,0);
		_active = 0;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}

	CHECK_GL_ERROR();	
}

void VBO :: sendIndexData(const void * mem,
						  const unsigned pos,const unsigned count) const
{	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
					pos*sizeof(unsigned),
					count*sizeof(unsigned),
					mem);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	CHECK_GL_ERROR();
}

void VBO :: sendData(const VertexAttribute * vattr,
			  const void * mem,
			  const unsigned pos,const unsigned count) const
{
			const unsigned index = ShaderManager::instance()->vertexAttributeIndex(vattr);
			ASSERT(_attribBuffers[index]);
			glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[index]);
			glBufferSubData(GL_ARRAY_BUFFER,
							pos*vattr->size(),
							count*vattr->size(),
							mem);
			//setAttributePointer(_buffer_info[i].attrib);
			CHECK_GL_ERROR();
}

void VBO :: getIndexData(void *& mem,
						  const unsigned pos,const unsigned count) const
{	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);
	glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER,
					pos*sizeof(unsigned),
					count*sizeof(unsigned),
					mem);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	CHECK_GL_ERROR();
}

void VBO :: getData(const VertexAttribute * vattr,
			  void *& mem,
			  const unsigned pos,const unsigned count) const
{
			const unsigned index = ShaderManager::instance()->vertexAttributeIndex(vattr);
			glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[index]);
			glGetBufferSubData(GL_ARRAY_BUFFER,
							pos*vattr->size(),
							count*vattr->size(),
							mem);
			//setAttributePointer(_buffer_info[i].attrib);
			CHECK_GL_ERROR();
}

// Mapping funcs
void VBO :: map(const unsigned& start,
			 const unsigned& access,
			 std::vector<const VertexAttribute *> vattrs,
			 std::vector<void *>& mem) const
{
	for(unsigned i=0;i<vattrs.size();++i)
	{
		const unsigned index = ShaderManager::instance()->vertexAttributeIndex(vattrs[i]);
			glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[index]);
				unsigned char * ptr = reinterpret_cast<unsigned char *>(glMapBuffer(GL_ARRAY_BUFFER,access));
				mem[i] = &ptr[start*vattrs[i]->size()];
				CHECK_GL_ERROR();
	}
}

void VBO :: map(const unsigned& start,
			 const unsigned& access,
			 const VertexAttribute * vattr,
			 void *& mem) const
{
	const unsigned index = ShaderManager::instance()->vertexAttributeIndex(vattr);
	glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[index]);
	unsigned char * ptr = reinterpret_cast<unsigned char *>(glMapBuffer(GL_ARRAY_BUFFER,access));
	mem = &ptr[start*vattr->size()];
	CHECK_GL_ERROR();
}

void VBO :: unmap(std::vector<const VertexAttribute *> vattrs) const
{
	for(unsigned i=0;i<vattrs.size();++i)
	{
		const unsigned index = ShaderManager::instance()->vertexAttributeIndex(vattrs[i]);
			glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[index]);
				const int res = glUnmapBuffer(GL_ARRAY_BUFFER);
				ASSERT(res == GL_TRUE);
				CHECK_GL_ERROR();
	}
}

void VBO :: unmap(const VertexAttribute * vattr) const
{

	const unsigned index = ShaderManager::instance()->vertexAttributeIndex(vattr);
	glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[index]);
	const int res = glUnmapBuffer(GL_ARRAY_BUFFER);
	ASSERT(res == GL_TRUE);
	CHECK_GL_ERROR();
}

void VBO :: mapIndices(const unsigned& start,const unsigned& access,void ** mem) const
{
	ASSERT(_ibuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,_ibuffer);
	unsigned * ptr = reinterpret_cast<unsigned *>(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER,access));
	*mem = reinterpret_cast<void *>(&ptr[start]);
	CHECK_GL_ERROR();
}

void VBO :: unmapIndices() const
{
	const GLboolean res = glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	ASSERT(res == GL_TRUE);
	CHECK_GL_ERROR();
}

bool VBO :: querySetData(const unsigned d_size,const unsigned i_size,unsigned& d_pos,unsigned& i_pos)
{
	//cout<<"VBO "<<this<<" QuerySet Data : "<<endl;
	//cout<<" - Asking for "<<i_size<<" i  & "<<d_size<<" d "<<endl;
	bool ok_one = false,ok_two = false;
	vector<vec2U>::iterator iti = _ifreeSpace.begin();
	while(iti != _ifreeSpace.end())
	{
		//cout<<" - Searching i range[ "<<*iti<<" ]"<<endl;
		if(( iti->y() - iti->x()) >= i_size)
		{
			ok_one = true;
			i_pos = iti->x();
			iti->x() += i_size;
			//cout<<" - Ok, new range = [ "<<*iti<<" ]"<<endl;
			break;
		}
		++iti;
	}
	if(ok_one)
	{
		vector<vec2U>::iterator itd = _dfreeSpace.begin();
		while(itd != _dfreeSpace.end())
		{
			//cout<<" - Searching d range[ "<<*itd<<" ]"<<endl;
			if(( itd->y() - itd->x()) >= d_size)
			{
				d_pos = itd->x();
				itd->x() += d_size;
				//cout<<" - Ok, new range = [ "<<*itd<<" ]"<<endl;
				return true;
			}
			++itd;
		}
		iti->x() -= i_size;
	}
	//cout<<" - Couldn't find both!!"<<endl;
	return false;
}

bool VBO :: querySize(const unsigned d_size,const unsigned i_size)
{
	bool ok_one = false,ok_two = false;
	vector<vec2U>::iterator iti = _ifreeSpace.begin();
	while(iti != _ifreeSpace.end())
	{
		if(( iti->y() - iti->x()) >= i_size)
		{
			ok_one = true;
			break;
		}
		++iti;
	}
	if(ok_one)
	{
		vector<vec2U >::iterator itd = _dfreeSpace.begin();
		while(itd != _dfreeSpace.end())
		{
			if(( itd->y() - itd->x()) >= d_size)
			{
				return true;
			}
			++itd;
		}
		iti->x() -= i_size;
	}
	return false;
}

void VBO :: freeData(const unsigned d_size,const unsigned i_size,const unsigned& d_pos,const unsigned& i_pos)
{
	ASSERT(i_size && d_size);
	//cout<<"VBO "<<this<<" free Data : "<<endl;
	//cout<<" - Deleting (pos,size) i( "<<i_pos<<" "<<i_size<<" ) & d( "<<d_pos<<" "<<d_size<<" )"<<endl;
	const unsigned i_bound = i_pos + i_size;
	for(unsigned i=0;i<_ifreeSpace.size();++i)
	{
		if( _ifreeSpace[i].x() > i_bound)
		{
			if(i && _ifreeSpace[i-1].y() == i_pos)
				_ifreeSpace[i-1].y() += i_size;
			else
				_ifreeSpace.insert(_ifreeSpace.begin() + i,vec2U(i_pos,i_pos+i_size));
			break;
		}
		else if(_ifreeSpace[i].x() == i_bound)
		{
			if(i && _ifreeSpace[i-1].y() == i_pos)
			{
				_ifreeSpace[i].x() = _ifreeSpace[i-1].x();
				_ifreeSpace.erase(_ifreeSpace.begin() + i - 1);
			}
			else
				_ifreeSpace[i].x() -= i_size;
			break;
		}
	}
	const unsigned d_bound = d_pos + d_size;
	for(unsigned i=0;i<_dfreeSpace.size();++i)
	{
		if( _dfreeSpace[i].x() > d_bound)
		{
			if(i && _dfreeSpace[i-1].y() == d_pos)
				_dfreeSpace[i-1].y() += d_size;
			else
				_dfreeSpace.insert(_dfreeSpace.begin() + i,vec2U(d_pos,d_pos+d_size));
			return;
		}
		else if(_dfreeSpace[i].x() == d_bound)
		{
			if(i && _dfreeSpace[i-1].y() == d_pos)
			{
				_dfreeSpace[i].x() = _dfreeSpace[i-1].x();
				_dfreeSpace.erase(_dfreeSpace.begin() + i - 1);
			}
			else
				_dfreeSpace[i].x() -= d_size;
			return;
		}
	}
	assert(0);
}

const unsigned VBO :: attrib_num() const
{
	return _buffer_num;
}

const VertexAttribute * VBO :: attrib(const unsigned& i) const
{
	int at = 0;
	int index =-1;
	while(++index < ATTRIB_NUM)
	{
		if(_attribBuffers[index])
		{
			if(at == i)
				return ShaderManager::instance()->vertexAttribute(index);
			++at;
		}
	}
	ASSERT(0);
	return 0;
}

const std::vector<const VertexAttribute *> VBO :: attribs() const
{
	std::vector<const VertexAttribute *> vattrs;
	for(unsigned i=0;i<attrib_num();++i)
		vattrs.push_back(attrib(i));
	return vattrs;
}

void VBO :: addAttribute(const VertexAttribute * vattr)
{
	const int index = ShaderManager::instance()->vertexAttributeIndex(vattr);
	if(_attribBuffers[index])	// if buffer already exists
		return;
	_potentialAttribs.attrib[index] = 0xFF;
	glGenBuffers(1,&(_attribBuffers[index]));

	glBindBuffer(GL_ARRAY_BUFFER,_attribBuffers[index]);
	glBufferData(GL_ARRAY_BUFFER,_dataSize*vattr->size(),0,_drawUsage);
	CHECK_GL_ERROR();
	++_buffer_num;
}