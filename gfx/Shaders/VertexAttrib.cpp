//*****************************************************************************
//**
//**   File:               VertexAttrib.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#include "../Shaders/VertexAttrib.h"
#include <gl/glee.h>
#include <string>
#include <assert.h>
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/Utils.h"

using namespace std;


void VertexAttribute :: parseConfiguration(const ParserSection * parsec)
{
	// If another attribute is found, it will (hopefully) set current cat to VertexAttribute & exit gracefully,
	// so client can repeat constructing another attribute

	string val;
	val = parsec->getVal("Components");
	assert(val != string(""));
	_components = FromString<unsigned>(val);
	val = parsec->getVal("Size");
	assert(val != string(""));
	_size = FromString<unsigned>(val);
	val = parsec->getVal("Datatype");
	if(val == "float")
		_datatype = GL_FLOAT;
	else if(val == "int")
		_datatype = GL_INT;
	else if(val == "uint")
		_datatype = GL_UNSIGNED_INT;
	else if(val == "double")
		_datatype = GL_DOUBLE;
	else if(val == "short")
		_datatype = GL_SHORT;
	else if(val == "ushort")
		_datatype = GL_UNSIGNED_SHORT;
	else if(val == "byte")
		_datatype = GL_BYTE;
	else if(val == "ubyte")
		_datatype = GL_UNSIGNED_BYTE;
	else
		assert(0);
	val = parsec->getName();
	assert(val != string(""));
	_name = val;
	_array_or_loc = 0;
	_isBuiltIn = true;
	//set Array appropriately, or loc = 0
	if(_name == "Vertex")
		_array_or_loc = GL_VERTEX_ARRAY;
	else if(_name == "Normal")
		_array_or_loc = GL_NORMAL_ARRAY;
	else if(_name == "Color")
		_array_or_loc = GL_COLOR_ARRAY;
	else if(_name == "Texcoord")
		_array_or_loc = GL_TEXTURE_COORD_ARRAY;
	else
	{
		_isBuiltIn = false;
	}
}


void VertexAttribute :: EnableClientArray()
{
	assert(_array_or_loc);
	if(!built_in())
		glEnableVertexAttribArray(_array_or_loc);
	else
		glEnableClientState(_array_or_loc);
	_used = true;
}

void VertexAttribute :: DisableClientArray()
{
	//ASSERT(_array_or_loc);
	if(!built_in())
		glDisableVertexAttribArray(_array_or_loc);
	else
	{
		if(_array_or_loc)
			glDisableClientState(_array_or_loc);
	}
	_used = false;
}

void VertexAttribute :: setAttribPointer(const void * pointer) const
{
	if(!built_in())
		glVertexAttribPointer(_array_or_loc, _components,_datatype, 0, _size,pointer);
	else if(_array_or_loc == GL_VERTEX_ARRAY)
		glVertexPointer(_components,_datatype,_size,pointer);
	else if(_array_or_loc == GL_TEXTURE_COORD_ARRAY)
		glTexCoordPointer(_components,_datatype,_size,pointer);
	else if(_array_or_loc == GL_NORMAL_ARRAY)
		glNormalPointer(_datatype,_size,pointer);
	else if(_array_or_loc == GL_COLOR_ARRAY)
		glColorPointer(_components,_datatype,_size,pointer);
}