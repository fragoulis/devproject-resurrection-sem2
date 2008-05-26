#include "VATTable.h"

GXVtxDescList VATTable :: m_vcd[8][GX_MAX_VTXDESCLIST_SZ];

void VATTable :: buildVAT()
{

	/*
		Used formats : 

		0) Terrain - Spawnpoints - in use
			position, texcoord

			F32 , U8

		1) Menu, loading screen, some HUD parts - in use
			position, texture

			S16, U8

		2) particles - Lasers
			position, color, texcoord

			F32, U32, U8
		
		3) particles2
			position, color, texcoord, normal

			F32, U32, U8, F32

		4) ships
			position, normal

			F32, F32

		5) ships2 
			position, texcoord, normal

			F32, F32 , F32

		6) HUD
			position, texcoord, color
			
			S16 , U8, U32

		7) Lasers, Lake - in use

	*/


	// set format 0
	GXClearVtxDesc();
	_setPosition(GX_VTXFMT0,GX_F32,GX_INDEX16);
	_setTexcoord(GX_VTXFMT0,GX_U16,GX_INDEX16);
	GXGetVtxDescv(m_vcd[0]);

	

	// set format 1
	GXClearVtxDesc();
	// Babis idea:
	//_setPosition(GX_VTXFMT1,GX_F32,GX_INDEX8);
	//_setColor(GX_VTXFMT1,GX_RGBA8,GX_INDEX8);

	// for my own idea:
	_setPosition(GX_VTXFMT1,GX_S16,GX_DIRECT);
	_setTexcoord(GX_VTXFMT1,GX_U8,GX_DIRECT);
	GXGetVtxDescv(m_vcd[1]);

	// set format 2
	GXClearVtxDesc();
	_setPosition(GX_VTXFMT2,GX_F32,GX_INDEX8);
	_setColor(GX_VTXFMT2,GX_RGBA8,GX_INDEX8);
	_setTexcoord(GX_VTXFMT2,GX_U8,GX_INDEX8);
	GXGetVtxDescv(m_vcd[2]);

	// set format 3
	GXClearVtxDesc();
	_setPosition(GX_VTXFMT3,GX_F32,GX_INDEX8);
	_setColor(GX_VTXFMT3,GX_RGBA8,GX_INDEX8);
	_setNormal(GX_VTXFMT3,GX_F32,GX_INDEX8);
	_setTexcoord(GX_VTXFMT3,GX_U8,GX_INDEX8);
	GXGetVtxDescv(m_vcd[3]);

	// set format 4
	GXClearVtxDesc();
	_setPosition(GX_VTXFMT4,GX_F32,GX_INDEX16);
	_setNormal(GX_VTXFMT4,GX_F32,GX_INDEX16);
	GXGetVtxDescv(m_vcd[4]);

	// set format 5
	GXClearVtxDesc();
	_setPosition(GX_VTXFMT5,GX_F32,GX_INDEX16);
	_setNormal(GX_VTXFMT5,GX_F32,GX_INDEX16);
	_setTexcoord(GX_VTXFMT5,GX_F32,GX_INDEX16);
	GXGetVtxDescv(m_vcd[5]);

	// set format 6
	GXClearVtxDesc();
	// Babis idea:
	//_setPosition(GX_VTXFMT6,GX_F32,GX_INDEX8);
	//_setNormal(GX_VTXFMT6,GX_U8,GX_INDEX8);
	//_setTexcoord(GX_VTXFMT6,GX_U8,GX_INDEX8);

	// for my own idea:
	_setPosition(GX_VTXFMT6,GX_S16,GX_DIRECT);
	_setTexcoord(GX_VTXFMT6,GX_U8,GX_DIRECT);
	_setColor(GX_VTXFMT6,GX_RGBA8,GX_DIRECT);
	GXGetVtxDescv(m_vcd[6]);

	// set format 7
	GXClearVtxDesc();
	_setPosition(GX_VTXFMT7,GX_F32,GX_DIRECT);
	_setTexcoord(GX_VTXFMT7,GX_U8,GX_DIRECT);
	GXGetVtxDescv(m_vcd[7]);


	GXSetMisc( GX_MT_XF_FLUSH, GX_XF_FLUSH_SAFE );

	OSReport("VAT Table initialized...\n");
	
}

void VATTable :: _setPosition(GXVtxFmt format, GXCompType type, GXAttrType itype)
{
	GXSetVtxAttrFmt(format, GX_VA_POS, GX_POS_XYZ, type, 0);
	GXSetVtxDesc(GX_VA_POS, itype);
}

void VATTable :: _setColor(GXVtxFmt format, GXCompType type, GXAttrType itype)
{
	GXSetVtxAttrFmt(format, GX_VA_CLR0, GX_CLR_RGBA, type, 0);
	GXSetVtxDesc(GX_VA_CLR0, itype);
}

void VATTable :: _setNormal(GXVtxFmt format, GXCompType type, GXAttrType itype)
{
	GXSetVtxAttrFmt(format, GX_VA_NRM, GX_NRM_XYZ, type, 0);
	GXSetVtxDesc(GX_VA_NRM, itype);
}

void VATTable :: _setTexcoord(GXVtxFmt format, GXCompType type, GXAttrType itype)
{
	GXSetVtxAttrFmt(format, GX_VA_TEX0, GX_TEX_ST, type, 0);
	GXSetVtxDesc(GX_VA_TEX0, itype);
}

