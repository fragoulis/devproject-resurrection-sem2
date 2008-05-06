#include <Texture/TextureIO.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <glee.h>
#include <MemManager/MemMgr_RawData.h>
#include <Texture/Texture.h>
#include <Texture/Texture2D.h>
#include <Misc/Logger.h>

using namespace CKLib;
using namespace std;

static void swaps(short& s)
{
	char *ptr = (char *)(&s);
	swap(ptr[0],ptr[1]);
}
static void swapus(unsigned short& s)
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


class sgi_format_header
{
public : 
	short			MAGIC;     // IRIS image file magic number
    char			STORAGE;   // Storage format
    char			BPC;       // Number of bytes per pixel channel 
    unsigned short  DIMENSION; // Number of dimensions
    unsigned short  XSIZE;      // X size in pixels 
    unsigned short  YSIZE;      // Y size in pixels 
    unsigned short  ZSIZE;    // Number of channels
    long		    PIXMIN;    // Minimum pixel value
    long			PIXMAX;    // Maximum pixel value
    char			DUMMY[4];     // Ignored
    char			IMAGENAME[80]; // Image name
    long			COLORMAP;  // Colormap ID
    char			DUMMY2[404];     // Ignored

	// returns false if wrong header
	bool checkswap()
	{
		if(MAGIC == short(0x01da))
			return true;
		if(MAGIC != short(0xda01))
			return false;
		while(0);
		//swaps(MAGIC);
		swapus(DIMENSION);
		swapus(XSIZE);
		swapus(YSIZE);
		swapus(ZSIZE);
		swapl(PIXMIN);
		swapl(PIXMAX);
		swapl(COLORMAP);
		return true;
	}
};

Texture * TextureIO ::_loadSGI(const std::string& fname)
{
	FILE * fp = fopen(fname.c_str(),"rb");
	if(!fp)
	{
		// Log event
		return 0;
	}

	// Get header
	sgi_format_header head;
	size_t bytes_read = fread(&head,1,512,fp);
	if((bytes_read < 512) || (!head.checkswap()))
	{
		fclose(fp);
		return false;
	}
	if(head.STORAGE == 1)
	{
		// LOG event : loading.. RLE not supported!!
		fclose(fp);
		return 0;
	}
	unsigned width = head.XSIZE;
	unsigned height = head.YSIZE;
	unsigned internalFormat = head.ZSIZE;
	unsigned datatype = GL_UNSIGNED_BYTE;
	unsigned pixelFormat = 0;
	
	switch(internalFormat)
	{
	case 1: 
		pixelFormat = GL_LUMINANCE;
		break;
	case 2: 
		pixelFormat = GL_LUMINANCE_ALPHA;
		break;
	case 3:
		pixelFormat = GL_RGB;
		break;
	case 4:
		pixelFormat = GL_RGBA;
		break;
	default:
		// Log Event : loading.. Invalid pixel format!!
		fclose(fp);
		return 0;
	}
	
	const unsigned img_size = width * height * internalFormat;
	unsigned char * data, * _raw;
	data = MemMgrRaw::instance()->allocate<unsigned char>(img_size);
	_raw = MemMgrRaw::instance()->allocate<unsigned char>(img_size);
	bytes_read = fread(_raw,1,img_size,fp);
	fclose(fp);
	if(bytes_read != img_size)
	{
		MemMgrRaw::instance()->free(data);
		MemMgrRaw::instance()->free(_raw);
		return 0;
	}
	// Assign data
	for(unsigned i=0;i<height;++i)
		for(unsigned j=0;j< width;++j)
		{
			// Copy the four elements
			const unsigned di = (i*width + j)*internalFormat;
			for(unsigned k=0;k<internalFormat;++k)
			{
				const unsigned offset = width*height*k + width*i + j;
				data[di + k] = _raw[offset];
			}
		}

	
	size_t pos1 = fname.find_last_of("/\\",fname.size(),2);
	if(pos1 == string::npos)
		pos1 = -1;
	const string fileName = fname.substr(pos1 + 1,fname.size() - pos1 - 1);

	Texture * tex = 0;

	vector<MipmapLevel> level;
	level.push_back(MipmapLevel(data,img_size));

	unsigned target = (_loadAsRect) ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
	tex = new Texture2D(width,height,internalFormat,pixelFormat,datatype,
						level,target,fileName,_genMipmaps,_loadAsRect);

	MemMgrRaw::instance()->free(data);
	MemMgrRaw::instance()->free(_raw);
	return tex;
}

bool TextureIO ::_saveSGI(const Texture * tex, 
						  const std::string& fname)
{
	// Log event : Not Implemented Yet!
	TEX_header header = tex->getTEXheader();
	
	FILE * fp = fopen(fname.c_str(),"wb");
	if( fp == NULL )
	{
	   CKLOG(string("Error opening file ") + fname,0);
       return 0;
	}

	sgi_format_header outheader;
	
	// ASSUME format is ok.

	const unsigned img_size = tex->dataSize();
	const unsigned bpp = img_size / (header.width * header.height);

	outheader.MAGIC = short(0xda01);
	outheader.XSIZE = header.width;
	outheader.YSIZE = header.height;
	outheader.ZSIZE = bpp;
	outheader.STORAGE = 0;
	outheader.BPC = 1;
	outheader.DIMENSION = 3;
	outheader.PIXMIN = 0;
	outheader.PIXMAX = 255;
	outheader.COLORMAP = 0;
	outheader.checkswap();

	fwrite(&outheader,sizeof(outheader),1,fp);

	unsigned char * data, * _raw;
	data = MemMgrRaw::instance()->allocate<unsigned char>(img_size);
	_raw = MemMgrRaw::instance()->allocate<unsigned char>(img_size);

	tex->dlData(data);

	for(unsigned i=0;i<header.height;++i)
	{
		for(unsigned j=0;j< header.width;++j)
		{
			// Copy the four elements
			const unsigned di = (i*header.width + j)*bpp;
			for(unsigned k=0;k<bpp;++k)
			{
				const unsigned offset = header.width*header.height*k + header.width*i + j;
				_raw[offset] = data[di + k];
			}
		}
	}

	fwrite(_raw,1,img_size,fp);
	fclose(fp);

	MemMgrRaw::instance()->free(data);
	MemMgrRaw::instance()->free(_raw);

	return true;
}