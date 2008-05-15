#ifndef __VertexFormat_H__
#define __VertexFormat_H__

#include <dolphin.h>


// Terrain - spawnpoints vertex format
struct VFMT0
{
	f32 xyz[3];
	u16  st[2];

	static const u8 VAT_index = 0;
	static const GXVtxFmt FMT_index = GX_VTXFMT0;

	struct IndexStruct
	{
		u16 posi,
			texi;
	};

	static void sendVertex(void *indices, u16 i)
	{
		IndexStruct * is = (IndexStruct * )indices;
		GXPosition1x16(is[i].posi);
		GXTexCoord1x16(is[i].texi);
	}

	static void setArray(VFMT0 * data)
	{
		GXSetArray(GX_VA_POS,&(data->xyz),sizeof(VFMT0));
		GXSetArray(GX_VA_TEX0,&(data->st),sizeof(VFMT0));
	}
};

struct VFMT1
{
	f32 xyz[3];
	u32  rgba;

	static const u8 VAT_index = 1;
	static const GXVtxFmt FMT_index = GX_VTXFMT1;

	struct IndexStruct
	{
		u8 posi,
		   coli;
	};

	static void sendVertex(void *indices, u16 i)
	{
		IndexStruct * is = (IndexStruct * )indices;
		GXPosition1x8(is[i].posi);
		GXColor1x8(is[i].coli);
	}

	static void setArray(VFMT1 * data)
	{
		GXSetArray(GX_VA_POS,&(data->xyz),sizeof(VFMT1));
		GXSetArray(GX_VA_CLR0,&(data->rgba),sizeof(VFMT1));
	}
};

struct VFMT2
{
	f32 xyz[3];
	u32  rgba;
	u8 st[2];

	static const u8 VAT_index = 2;
	static const GXVtxFmt FMT_index = GX_VTXFMT2;

	struct IndexStruct
	{
		u8 posi,
		   coli,
		   texi;
	};

	static void sendVertex(void *indices, u16 i)
	{
		IndexStruct * is = (IndexStruct * )indices;
		GXPosition1x8(is[i].posi);
		GXColor1x8(is[i].coli);
		GXTexCoord1x8(is[i].texi);
	}

	static void setArray(VFMT2 * data)
	{
		GXSetArray(GX_VA_POS,&(data->xyz),sizeof(VFMT2));
		GXSetArray(GX_VA_CLR0,&(data->rgba),sizeof(VFMT2));
		GXSetArray(GX_VA_TEX0,&(data->st),sizeof(VFMT2));
	}
};

// BEWARE COPY_PASTES!!

/*
	Example code : 

	Global : 

	VertexArray<VTF0> va;

	In init : 

	VATTable::BuildVAT();
	VTF0 * data = (VTF0 *)OSAlloc(sizeof(VTF0)*4);
	data[0].x = 
	data[0].y = 
	data[0].z = 
	data[0].s = 
	data[0].t = 
	...

	VTF0::IndexStruct * indices = OSAlloc(sizeof(VTF0::IndexStruct)*6);
	indices[0].posi = indices[0].texi = 0x00;
	indices[1].posi = indices[1].texi = 0x03;
	indices[2].posi = indices[2].texi = 0x01;
	indices[3].posi = indices[3].texi = 0x01;
	indices[4].posi = indices[4].texi = 0x03;
	indices[5].posi = indices[5].texi = 0x02;
	va.set(data,4,GX_TRIANGLES,6,indices);

	In draw : 

	//set texture here
	va.call();

*/




#endif