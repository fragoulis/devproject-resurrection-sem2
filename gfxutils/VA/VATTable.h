#ifndef __VATTABLE_H__
#define __VATTABLE_H__

#include <dolphin.h>

class VATTable
{
	static GXVtxDescList m_vcd[8][GX_MAX_VTXDESCLIST_SZ];

	// VAT building helpers
	static void _setPosition(GXVtxFmt format, GXCompType type, GXAttrType itype);
	static void _setColor(GXVtxFmt format, GXCompType type, GXAttrType itype);
	static void _setNormal(GXVtxFmt format, GXCompType type, GXAttrType itype);
	static void _setTexcoord(GXVtxFmt format, GXCompType type, GXAttrType itype);

public :

	static void buildVAT();
	static GXVtxDescList * getVDL(u8 index) {return m_vcd[index];}
};

#endif