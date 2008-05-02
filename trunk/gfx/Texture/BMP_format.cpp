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

	void set_bfType(const unsigned val) {*(unsigned __int16 *)(&data[0]) = val;}
	void set_bfSize(const unsigned val) {*(unsigned *)(&data[2]) = val;}
	void set_bfReserved1(const unsigned val) {*(unsigned __int16 *)(&data[6]) = val;}
	void set_bfReserved2(const unsigned val) {*(unsigned __int16 *)(&data[8]) = val;}
	void set_bfOffBits(const unsigned val) {*(unsigned *)(&data[10]) = val;}
	void set_biSize(const unsigned val) {*(unsigned *)(&data[14]) = val;}
	void set_biWidth(const unsigned val) {*(unsigned *)(&data[18]) = val;}
	void set_biHeight(const unsigned val) {*(unsigned *)(&data[22]) = val;}
	void set_biPlanes(const unsigned val) {*(unsigned __int16*)(&data[26]) = val;}
	void set_biBitCount(const unsigned val) {*(unsigned __int16*)(&data[28]) = val;}
	void set_biCompression(const unsigned val) {*(unsigned *)(&data[30]) = val;}
	void set_biSizeImage(const unsigned val) {*(unsigned *)(&data[34]) = val;}
	void set_biXPelsPerMeter(const unsigned val) {*(unsigned *)(&data[38]) = val;}
	void set_biYPelsPerMeter(const unsigned val) {*(unsigned *)(&data[42]) = val;}
	void set_biClrUsed(const unsigned val) {*(unsigned *)(&data[46]) = val;}
	void set_biClrImportant(const unsigned val) {*(unsigned *)(&data[50]) = val;}

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
	TEX_header header = tex->getTEXheader();
	
	FILE * fp = fopen(fname.c_str(),"wb");
	if( fp == NULL )
	{
       return 0;
	}

	const unsigned img_size = tex->dataSize();
	const unsigned bpp = img_size / (header.width * header.height);

	bmp_format_header outheader;
	outheader.set_biCompression(0);
	outheader.set_biWidth(header.width);
	outheader.set_biHeight(header.height);
	outheader.set_biBitCount(bpp*8);
	outheader.set_bfType(unsigned short(0x4D42));
	outheader.set_bfSize(img_size);
	outheader.set_bfReserved1(0);
	outheader.set_bfReserved2(0);
	outheader.set_bfOffBits(54*8);
	outheader.set_biClrUsed(0);
	outheader.set_biClrImportant(0);
	outheader.set_biPlanes(1);
	outheader.set_biSize(40);
	outheader.set_biXPelsPerMeter(0);
	outheader.set_biYPelsPerMeter(0);
	
	// ASSUME format is ok.
	
	fwrite(outheader.data,1,54,fp);

	unsigned char * data;
	data = MemMgrRaw::instance()->allocate<unsigned char>(img_size);

	tex->dlData(data);

	fwrite(data,1,img_size,fp);
	fclose(fp);

	MemMgrRaw::instance()->free(data);

	return true;
}