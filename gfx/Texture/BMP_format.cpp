//*****************************************************************************
//**
//**   File:               BMP_format.cpp
//**   Author:             $Author: esrever2357 $
//**   Last Change:        $Date: 2008-03-13 23:43:47 +0000 (Thu, 13 Mar 2008) $
//**   Last Revision:      $Revision: 165 $
//**
//*****************************************************************************

#include "TextureIO.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <gl/glee.h>
#include "../../gfxutils/MemManager/MemMgr_RawData.h"
#include "Texture.h"
#include "Texture2D.h"
#include "Texture1D.h"

using namespace std;

static void swaps(short& s)
{
	char *ptr = (char *)(&s);
	swap(ptr[0],ptr[1]);
}
static void swapus(unsigned __int16& s)
{
	char *ptr = (char *)(&s);
	swap(ptr[0],ptr[1]);
}
static void swapl(long& l)
{
	char *ptr = (char *)(&l);
	swap(ptr[0],ptr[3]);
	swap(ptr[1],ptr[2]);
}

static void swapu(unsigned& u)
{
	char *ptr = (char *)(&u);
	swap(ptr[0],ptr[3]);
	swap(ptr[1],ptr[2]);
}


struct bmp_format_header
{
	unsigned __int16	bfType;
	unsigned		bfSize;
	unsigned __int16	bfReserved1;
	unsigned __int16	bfReserved2;
	unsigned		bfOffBits;

	unsigned		biSize;
	unsigned		biWidth;
	unsigned		biHeight;
	unsigned __int16  biPlanes;
	unsigned __int16  biBitCount;
	unsigned		biCompression;
	unsigned		biSizeImage;
	unsigned		biXPelsPerMeter;
	unsigned		biYPelsPerMeter;
	unsigned		biClrUsed;
	unsigned		biClrImportant;

	char data[64];

	// returns false if wrong header
	bool checkswap()
	{
		
		bfType = *(unsigned __int16 *)(data);
		bfSize = *(unsigned *)(&data[2]);
		bfReserved1 = *(unsigned __int16 *)(&data[6]);
		bfReserved2 = *(unsigned __int16 *)(&data[8]);
		bfOffBits = *(unsigned *)(&data[10]);	

		biSize = *(unsigned *)(&data[14]);
		biWidth = *(unsigned *)(&data[18]);
		biHeight = *(unsigned *)(&data[22]);	
		biPlanes = *(unsigned __int16 *)(&data[26]);
		biBitCount = *(unsigned __int16 *)(&data[28]);
		biCompression = *(unsigned *)(&data[30]);	
		biSizeImage = *(unsigned *)(&data[34]);
		biXPelsPerMeter = *(unsigned *)(&data[38]);	
		biYPelsPerMeter = *(unsigned *)(&data[42]);	
		biClrUsed = *(unsigned *)(&data[46]);
		biClrImportant = *(unsigned *)(&data[50]);

		if(bfType == unsigned short(0x4D42))
			return true;
		if(bfType != unsigned short(0x424D))
			return false;
		
		swapus(biPlanes);
		swapus(biBitCount);
		swapu(bfSize);
		swapu(biSize);
		swapu(biWidth);
		swapu(biHeight);
		swapu(biCompression);
		swapu(biSizeImage);
		swapu(biXPelsPerMeter);
		swapu(biYPelsPerMeter);
		swapu(biClrUsed);
		swapu(biClrImportant);
		return true;
	}
};

Texture * TextureIO ::_loadBMP(const std::string& fname)
{
	FILE * fp = fopen(fname.c_str(),"rb");
	if(!fp)
	{
		// Log event
		return 0;
	}

	// Get header
	bmp_format_header head;
	size_t bytes_read = fread(head.data,1,54,fp);
	if((bytes_read < sizeof(54)) || (!head.checkswap()))
	{
		fclose(fp);
		return false;
	}
	if(head.biCompression)
	{
		// LOG event : loading.. RLE not supported!!
		fclose(fp);
		return 0;
	}
	unsigned width = head.biWidth;
	unsigned height = head.biHeight;
	unsigned internalFormat = head.biBitCount / 8;
	unsigned datatype = GL_UNSIGNED_BYTE;
	unsigned pixelFormat = 0;
	
	switch(internalFormat)
	{
	case 1: 
		pixelFormat = GL_LUMINANCE;
		break;
	case 3:
		pixelFormat = GL_BGR;
		break;
	default:
		// Log Event : loading.. Invalid pixel format!!
		fclose(fp);
		return 0;
	}
	
	const unsigned img_size = width * height * internalFormat;
	unsigned char * _raw;
	_raw = MemMgrRaw::instance()->allocate<unsigned char>(img_size);
	bytes_read = fread(_raw,1,img_size,fp);
	fclose(fp);
	if(bytes_read != img_size)
	{
		MemMgrRaw::instance()->free(_raw);
		return 0;
	}

	size_t pos1 = fname.find_last_of("/\\",fname.size(),2);
	if(pos1 == string::npos)
		pos1 = -1;
	const string fileName = fname.substr(pos1 + 1,fname.size() - pos1 - 1);

	Texture * tex = 0;

	vector<MipmapLevel> level;
	level.push_back(MipmapLevel(_raw,img_size));

	unsigned target = (_loadAsRect) ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
	//if(height != 1)
	{
		tex = new Texture2D(width,height,internalFormat,pixelFormat,datatype,
						level,target,fileName,_genMipmaps,_loadAsRect);
	}
	/*
	else
	{
		target = GL_TEXTURE_1D;
		tex = new Texture1D(width,internalFormat,pixelFormat,datatype,
						level,target,fileName,_genMipmaps,false);
	}
	*/

	MemMgrRaw::instance()->free(_raw);
	return tex;
}

bool TextureIO ::_saveBMP(const Texture * tex, 
						  const std::string& fname)
{
	// Log event : Not Implemented Yet!
	return false;
}