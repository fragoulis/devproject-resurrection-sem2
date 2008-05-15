#include "VertexArray.h"
#include "VATTable.h"

template <class VTF>
VertexArray<VTF> :: ~VertexArray()
{
	// FIXME : free the dl
}

// builds the display list
template <class VTF>
void VertexArray<VTF> :: set(VTF * data,		// interleaved vertex data array
						u32 datanum,		// data array size
						GXPrimitive primtype,	// primitive type
						u16 primnum,		// primitive num (no of calls)
						void * indices)	// indices array (no of calls)
{
	m_data = data;
	m_dataNum = datanum;
	
	GXSetVtxDescv(VATTable::getVDL(VTF::VAT_index));
	VTF::setArray(m_data);
	m_dlist.start();
	GXBegin(primtype, VTF::FMT_index, primnum);
	for(u16 i=0;i<primnum;++i)
		VTF::sendVertex(indices,i);
	GXEnd();
	m_dlist.end();
}

// sets the data arrays & calls the display list
template <class VTF>
void VertexArray<VTF> :: call() const		
{
	GXSetVtxDescv(VATTable::getVDL(VTF::VAT_index));
	//VTF::setArray(m_data);
	m_dlist.call();
}

template <class VTF>
void VertexArray<VTF> :: init()
{
	m_data = 0;
	m_dataNum = 0;
	m_dlist = DispList();
}

