#ifndef __VERTEXFORMATS_H__
#define __VERTEXFORMATS_H__

#include <dolphin.h>

struct VF32_TF32
{
	f32 pos[3];
	f32 tex[2];
};

struct VF32_TU8
{
	f32 pos[3];
	u8 tex[2];
};

struct VF32_TU8_CU32
{
	f32 pos[3];
	u8 tex[2];
	u32 rgba;
};




#endif