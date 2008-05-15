#ifndef __VERTEXARRAY_H__
#define __VERTEXARRAY_H__

#include "DispList.h"
#include "VertexFormat.h"

/*
	Need to have the VAT table ready at init time (a static func?)
	then the app should provide the data & the flags & everything should be taken care of. fast.
	So the app instead of void ** data, etc, should provide the struct immediately as a single void *
	A flag could define the type & the target VAT entry
*/



template <class VTF>
class VertexArray
{
	DispList	  m_dlist;	
	VTF			* m_data;
	u32			  m_dataNum;

public :
	
	//ctor / dtor
	VertexArray():m_data(0),m_dataNum(0){};
	~VertexArray();

	// builds the display list
	void init();
	
	void set(VTF * data,		// interleaved vertex data array
		u32 datanum,		// data array size
		GXPrimitive primtype,	// primitive type
		u16 primnum,		// primitive num (no of calls)
		void * indices);	// indices array (no of calls)

	// sets the data arrays & calls the display list
	void call() const;		


	
};

#include "VertexArray.hpp"

#endif