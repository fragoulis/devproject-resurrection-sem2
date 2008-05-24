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

	static void sendVertex(IndexStruct *indices, u16 i)
	{
		GXPosition1x16(indices[i].posi);
		GXTexCoord1x16(indices[i].texi);
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

struct VFMT3
{
	f32 xyz[3];
	u32  rgba;
	f32 nrm[3];
	u8 st[2];

	static const u8 VAT_index = 3;
	static const GXVtxFmt FMT_index = GX_VTXFMT3;

	struct IndexStruct
	{
		u8 posi,
		   coli,
		   nrm,
		   texi;
	};

	static void sendVertex(void *indices, u16 i)
	{
		IndexStruct * is = (IndexStruct * )indices;
		GXPosition1x8(is[i].posi);
		GXColor1x8(is[i].coli);
		GXNormal1x8(is[i].nrm);
		GXTexCoord1x8(is[i].texi);
	}

	static void setArray(VFMT3 * data)
	{
		GXSetArray(GX_VA_POS,&(data->xyz),sizeof(VFMT3));
		GXSetArray(GX_VA_CLR0,&(data->rgba),sizeof(VFMT3));
		GXSetArray(GX_VA_NRM,&(data->nrm),sizeof(VFMT3));
		GXSetArray(GX_VA_TEX0,&(data->st),sizeof(VFMT3));
	}
};

struct VFMT4
{
	f32 * xyz;
	f32 * nrm;

	static const u8 VAT_index = 4;
	static const GXVtxFmt FMT_index = GX_VTXFMT4;

	struct IndexStruct
	{
		u16 posi,
		    nrm;
	};

	static void sendVertex(IndexStruct *indices, u16 i)
	{
		GXPosition1x16(indices[i].posi);
		GXNormal1x16(indices[i].nrm);
	}

	static void setArray(VFMT4 * data)
	{
		GXSetArray(GX_VA_POS,data->xyz,sizeof(f32)*3);
		GXSetArray(GX_VA_NRM,data->nrm,sizeof(f32)*3);
	}
};

struct VFMT5
{
	f32 * xyz;
	f32 * nrm;
	f32 * st;

	static const u8 VAT_index = 5;
	static const GXVtxFmt FMT_index = GX_VTXFMT5;

	struct IndexStruct
	{
		u16 posi,
		    nrm,
			st;
	};

	static void sendVertex(IndexStruct *indices, u16 i)
	{
		GXPosition1x16(indices[i].posi);
		GXNormal1x16(indices[i].nrm);
		GXTexCoord1x16(indices[i].st);
	}

	static void setArray(VFMT5 * data)
	{
		static const u8 size1 = sizeof(f32)*3;
		static const u8 size2 = sizeof(f32)*2;
		GXSetArray(GX_VA_POS,data->xyz,size1);
		GXSetArray(GX_VA_NRM,data->nrm,size1);
		GXSetArray(GX_VA_TEX0,data->st,size2);
	}
};

struct VFMT6
{
	f32 xyz[3];
	u8 nrm[3];
	u8 st[2];

	static const u8 VAT_index = 6;
	static const GXVtxFmt FMT_index = GX_VTXFMT6;

	struct IndexStruct
	{
		u8 posi,
		    nrm,
			st;
	};

	static void sendVertex(void *indices, u16 i)
	{
		IndexStruct * is = (IndexStruct * )indices;
		GXPosition1x8(is[i].posi);
		GXNormal1x8(is[i].nrm);
		GXTexCoord1x8(is[i].st);
	}

	static void setArray(VFMT6 * data)
	{
		GXSetArray(GX_VA_POS,&(data->xyz),sizeof(VFMT6));
		GXSetArray(GX_VA_NRM,&(data->nrm),sizeof(VFMT6));
		GXSetArray(GX_VA_TEX0,&(data->st),sizeof(VFMT6));
	}
};





#endif