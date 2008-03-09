//*****************************************************************************
//**
//**   File:               VBODesc.cpp
//**   Author:             $Author$
//**   Last Change:        $Date$
//**   Last Revision:      $Revision$
//**
//*****************************************************************************

#include "VBODesc.h"
#include "../Shaders/VertexAttrib.h"
#include "../VBO/VBO.h"

VBODesc :: VBODesc(VBO * vbo,
			std::vector<const VertexAttribute *> attrib,
			std::vector<void *> attrib_data,
			unsigned data_num,
			unsigned * index_data,
			unsigned index_num,
			unsigned drawMode)
:m_dSize(data_num),
m_iSize(index_num),
m_drawMode(drawMode),
m_vbo(vbo)
{
	vbo->querySetData(data_num,index_num,m_dStart,m_iStart);
	// Correct indices
	for(unsigned ii=0;ii<index_num;++ii)
		index_data[ii] += m_dStart;

	// Send data
	vbo->sendIndexData(index_data,m_iStart,index_num);

	for(unsigned k=0;k<attrib.size();++k)
		vbo->sendData(attrib[k],attrib_data[k],m_dStart,data_num);
}

void VBODesc :: call() const
{
	m_vbo->call(m_iStart,m_iSize,m_drawMode);
}