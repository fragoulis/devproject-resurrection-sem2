#include <Texture/TextureIO.h>
#include <Texture/Texture2D.h>
#include <Texture/Texture3D.h>
#include <Texture/TextureCube.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <glee.h>
#include <MemManager/MemMgr_RawData.h>
#include <ddraw.h>
#include <Misc/Logger.h>

using namespace CKLib;
using namespace std;

const unsigned int DDS_FOURCC = 0x00000004;
const unsigned int DDS_RGB    = 0x00000040;
const unsigned int DDS_RGBA   = 0x00000041;
const unsigned int DDS_DEPTH  = 0x00800000;

const unsigned int DDS_COMPLEX = 0x00000008;
const unsigned int DDS_CUBEMAP = 0x00000200;
const unsigned int DDS_VOLUME  = 0x00200000;

//const unsigned int FOURCC_DXT1 = 0x31545844; //(MAKEFOURCC('D','X','T','1'))
//const unsigned int FOURCC_DXT3 = 0x33545844; //(MAKEFOURCC('D','X','T','3'))
//const unsigned int FOURCC_DXT5 = 0x35545844; //(MAKEFOURCC('D','X','T','5'))


struct DXTColBlock
{
    unsigned short col0;
    unsigned short col1;

    unsigned char row[4];
};

struct DXT3AlphaBlock
{
    unsigned short row[4];
};

struct DXT5AlphaBlock
{
    unsigned char alpha0;
    unsigned char alpha1;
    
    unsigned char row[6];
};

///////////////////////////////////////////////////////////////////////////////
// swap to sections of memory
static void swapsect(void *byte1, void *byte2, int size)
{
    unsigned char *tmp = new unsigned char[size];

    memcpy(tmp, byte1, size);
    memcpy(byte1, byte2, size);
    memcpy(byte2, tmp, size);

    delete [] tmp;
}

///////////////////////////////////////////////////////////////////////////////
// flip a DXT1 color block
static void flip_blocks_dxtc1(DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;

    for (int i = 0; i < numBlocks; i++)
    {
        swapsect(&curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swapsect(&curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// flip a DXT3 color block
static void flip_blocks_dxtc3(DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;
    DXT3AlphaBlock *alphablock;

    for (int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT3AlphaBlock*)curblock;

        swapsect(&alphablock->row[0], &alphablock->row[3], sizeof(unsigned short));
        swapsect(&alphablock->row[1], &alphablock->row[2], sizeof(unsigned short));

        curblock++;

        swapsect(&curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swapsect(&curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// flip a DXT5 alpha block
static void flip_dxt5_alpha(DXT5AlphaBlock *block)
{
    unsigned char gBits[4][4];
    
	const unsigned long mask = 0x00000007;		    // bits = 00 00 01 11
	unsigned long bits = 0;
    memcpy(&bits, &block->row[0], sizeof(unsigned char) * 3);
    
	gBits[0][0] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[0][1] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[0][2] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[0][3] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[1][0] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[1][1] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[1][2] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[1][3] = (unsigned char)(bits & mask);

    bits = 0;
    memcpy(&bits, &block->row[3], sizeof(unsigned char) * 3);

	gBits[2][0] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[2][1] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[2][2] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[2][3] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[3][0] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[3][1] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[3][2] = (unsigned char)(bits & mask);
	bits >>= 3;
	gBits[3][3] = (unsigned char)(bits & mask);

    unsigned long *pBits = ((unsigned long*) &(block->row[0]));

    *pBits &= 0xff000000;

    *pBits = *pBits | (gBits[3][0] << 0);
    *pBits = *pBits | (gBits[3][1] << 3);
    *pBits = *pBits | (gBits[3][2] << 6);
    *pBits = *pBits | (gBits[3][3] << 9);

    *pBits = *pBits | (gBits[2][0] << 12);
    *pBits = *pBits | (gBits[2][1] << 15);
    *pBits = *pBits | (gBits[2][2] << 18);
    *pBits = *pBits | (gBits[2][3] << 21);

	pBits = ((unsigned long*) &(block->row[3]));

    *pBits &= 0xff000000;

    *pBits = *pBits | (gBits[1][0] << 0);
    *pBits = *pBits | (gBits[1][1] << 3);
    *pBits = *pBits | (gBits[1][2] << 6);
    *pBits = *pBits | (gBits[1][3] << 9);

    *pBits = *pBits | (gBits[0][0] << 12);
    *pBits = *pBits | (gBits[0][1] << 15);
    *pBits = *pBits | (gBits[0][2] << 18);
    *pBits = *pBits | (gBits[0][3] << 21);
}

///////////////////////////////////////////////////////////////////////////////
// flip a DXT5 color block
static void flip_blocks_dxtc5(DXTColBlock *line, int numBlocks)
{
    DXTColBlock *curblock = line;
    DXT5AlphaBlock *alphablock;
    
    for (int i = 0; i < numBlocks; i++)
    {
        alphablock = (DXT5AlphaBlock*)curblock;
        
        flip_dxt5_alpha(alphablock);

        curblock++;

        swapsect(&curblock->row[0], &curblock->row[3], sizeof(unsigned char));
        swapsect(&curblock->row[1], &curblock->row[2], sizeof(unsigned char));

        curblock++;
    }
}

///////////////////////////////////////////////////////////////////////////////
// get line width
static int get_line_width(int width, int bpp)
{
    return ((width * bpp + 31) & -32) >> 3;
}

///////////////////////////////////////////////////////////////////////////////
// calculates size of DXTC texture in bytes
static int size_dxtc(int width, int height, GLuint ifmt)
{
    return ((width+3)/4)*((height+3)/4)*
        (ifmt == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ? 8 : 16);   
}

///////////////////////////////////////////////////////////////////////////////
// calculates size of uncompressed RGB texture in bytes
static int size_rgb(int width, int height,GLuint components)
{
    return width*height*components;
}

///////////////////////////////////////////////////////////////////////////////
// align to 4 byte boundary (add pad bytes to end of each line in the image)
/*
static void align_memory(CTexture *surface)
{
    // don't bother with compressed images, volume textures, or cubemaps
    if (compressed || volume || cubemap)
        return;

    // calculate new image size
    int linesize = get_line_width(surface->width, components*8);
    int imagesize = linesize*surface->height;

    // exit if already aligned
    if (surface->size == imagesize)
        return;

    // create new image of new size
    CTexture newSurface(surface->width, surface->height, surface->depth, 
        imagesize);

    // add pad bytes to end of each line
    char *srcimage = (char*)*surface;
    char *dstimage = (char*)newSurface;
    for (int n = 0; n < surface->depth; n++)
    {
        char *curline = srcimage;
        char *newline = dstimage;

        int imsize = surface->size / surface->depth;
        int lnsize = imsize / surface->height;
        
        for (int i = 0; i < surface->height; i++)
        {
            memcpy(newline, curline, lnsize);
            newline += linesize;
            curline += lnsize;
        }
    }

    // save padded image
    *surface = newSurface;
}
*/

///////////////////////////////////////////////////////////////////////////////
// flip image around X axis
static void flip(char *image, int width, int height, int depth, int size,GLuint ifmt,bool compressed)
{
    int linesize;
    int offset;

    if (!compressed)
    {
        assert(depth > 0);

        int imagesize = size/depth;
        linesize = imagesize / height;

        for (int n = 0; n < depth; n++)
        {
            offset = imagesize*n;
            char *top = image + offset;
            char *bottom = top + (imagesize-linesize);
    
            for (int i = 0; i < (height >> 1); i++)
            {
				swapsect(bottom, top, linesize);

                top += linesize;
                bottom -= linesize;
            }
        }
    }
    else
    {
        void (*flipblocks)(DXTColBlock*, int);
    	int xblocks = width / 4;
    	int yblocks = height / 4;
        int blocksize;

        switch (ifmt)
        {
            case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT: 
			case GL_COMPRESSED_RGB_S3TC_DXT1_EXT: 
                blocksize = 8;
                flipblocks = &flip_blocks_dxtc1; 
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT: 
                blocksize = 16;
                flipblocks = &flip_blocks_dxtc3; 
                break;
            case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT: 
                blocksize = 16;
                flipblocks = &flip_blocks_dxtc5; 
                break;
            default:
                return;
        }

        linesize = xblocks * blocksize;

    	DXTColBlock *top;
    	DXTColBlock *bottom;
    
	    for (int j = 0; j < (yblocks >> 1); j++)
	    {
    		top = (DXTColBlock*)(image + j * linesize);
    		bottom = (DXTColBlock*)(image + (((yblocks-j)-1) * linesize));

            (*flipblocks)(top, xblocks);
            (*flipblocks)(bottom, xblocks);

            swapsect(bottom, top, linesize);
        }
    }
}    







static void dds2glfmt(const DDPIXELFORMAT& pf,GLuint& ifmt,GLuint& pfmt,GLuint& dtype,GLuint& components)
{
	
    if (pf.dwFlags == DDS_RGBA && pf.dwRGBBitCount == 32)
    {
        ifmt = GL_RGBA; 
		dtype = GL_UNSIGNED_BYTE;
        pfmt = GL_BGRA;
		components = 4;
    }
    else if (pf.dwFlags == DDS_RGB  && pf.dwRGBBitCount == 24)
    {
        ifmt = GL_RGB; 
		dtype = GL_UNSIGNED_BYTE;
        pfmt = GL_BGR;
		components = 3;
    }
    else if (pf.dwRGBBitCount == 8)
    {
        ifmt = GL_LUMINANCE; 
		dtype = GL_UNSIGNED_BYTE;
        pfmt = GL_LUMINANCE;
		components = 1;
    }
	else
		assert(0);
}


Texture * TextureIO :: _loadDDS(const std::string& fname)
{
    DDSURFACEDESC2 ddsd;
    char filecode[4];
    FILE *pFile;
    int factor;
    int bufferSize;

    // Open the file
    pFile = fopen( fname.c_str(), "rb" );

    if( pFile == NULL )
	{
	   CKLOG(string("Error opening file ") + fname,0);
       return 0;
	}

    // Verify the file is a true .dds file
    fread( filecode, 1, 4, pFile );

    if( strncmp( filecode, "DDS ", 4 ) != 0 )
    {
		CKLOG(string("The file \"") + fname + string("\" doesn't appear to be a valid .dds file!"),0);
        return 0;
    }

    // Get the surface descriptor
    fread( &ddsd, sizeof(ddsd), 1, pFile );

    /*
		Now that we have the descriptor, here's the real meat
		- Check if compressed or uncompressed format
		- Check if volume, cubemap or 2D image
		- Check mipmaps
		- Check w,h,pfmt,ifmt,dtype & more, dep. on tex type
		- Mipmap/cubeface offsets for compressed textures??
	*/

	// get compression & mipmap information
	bool compressed = (ddsd.ddpfPixelFormat.dwFlags & DDPF_FOURCC) ? true : false;
	bool hasMipmaps = (ddsd.dwFlags & DDSD_MIPMAPCOUNT) ? true : false;
	unsigned mipcount = hasMipmaps ? ddsd.dwMipMapCount : 1;

	// declare variables here, in case they're needed for the tex creation
	unsigned width=ddsd.dwWidth,
			  height=ddsd.dwHeight,
			  depth=ddsd.dwDepth,
			  pfmt=0,ifmt=0,dtype=GL_UNSIGNED_BYTE,		// These three later
			  components=0;

	vector<vector<MipmapLevel> > texdata;
	unsigned char * pixels;

	bool isCubemap = (ddsd.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP) ? true : false;

	unsigned facenum = (isCubemap) ? 6 : 1;

	bool isVolume = (ddsd.ddsCaps.dwCaps2 & DDSCAPS2_VOLUME) ? true : false;

	if(compressed)
	{
		unsigned minbytes=0;
		switch( ddsd.ddpfPixelFormat.dwFourCC )
		{
			case FOURCC_DXT1:
				// DXT1's compression ratio is 8:1
				ifmt = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT ;
				minbytes = 8;
				factor = 2;
				break;

			case FOURCC_DXT3:
				// DXT3's compression ratio is 4:1
				ifmt = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				factor = 4;
				minbytes = 16;
				break;

			case FOURCC_DXT5:
				// DXT5's compression ratio is 4:1
				ifmt = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				factor = 4;
				minbytes = 16;
				break;

			default:
				// Log Event : invalid compression mode
				return 0;
		}
		assert(ddsd.dwLinearSize);
		bufferSize = int(ddsd.dwLinearSize * ((hasMipmaps) ? 1.5 : 1) * facenum);
	}
	else
	{
		dds2glfmt(ddsd.ddpfPixelFormat,ifmt,pfmt,dtype,components);
		bufferSize = width * height * components;
		if(isVolume)
			bufferSize*=depth;
		if(hasMipmaps)
		{
			unsigned levelsize = bufferSize;
			for(unsigned i=1;i<mipcount;++i)
			{
				bufferSize += (levelsize>>=2);
			}
		}
		if(isCubemap)
			bufferSize*=6;
	}

	pixels = MemMgrRaw::instance()->allocate<unsigned char>(bufferSize);
	fread( pixels, 1, bufferSize, pFile );
	// Close the file
    fclose( pFile );

	int (*sizefunc)(int,int,GLuint) = NULL;
	sizefunc = (compressed) ? &size_dxtc : &size_rgb;
	unsigned arg3 = (compressed) ? ifmt : components ;

	unsigned total_offset=0;
	for(unsigned i=0;i<facenum;++i)
	{
		vector<MipmapLevel> curlvl;
		unsigned tmp_offset = 0;
		unsigned tmp_w = width,
				 tmp_h = height,
				 tmp_d = MAX(depth,1);
		for(unsigned j=0;j<mipcount;++j)
		{
			// reverse row order
			unsigned offset = total_offset + tmp_offset;
			unsigned size = sizefunc(tmp_w,tmp_h,arg3)*tmp_d;
			if(_genMipmaps || (!j))
			{
				if (!isCubemap)
					flip((char *)(&pixels[offset]), tmp_w, tmp_h, tmp_d, size,ifmt,compressed);
				curlvl.push_back(MipmapLevel(&pixels[offset],size));
			}
			tmp_offset += size;
			tmp_w = MAX((tmp_w>>1),1);
			tmp_h = MAX((tmp_h>>1),1);
			tmp_d = MAX((tmp_d>>1),1);
		}
		total_offset += tmp_offset;
		texdata.push_back(curlvl);
	}

	size_t pos1 = fname.find_last_of("/\\",fname.size(),2);
	if(pos1 == string::npos)
		pos1 = -1;
	const string fileName = fname.substr(pos1 + 1,fname.size() - pos1 - 1);

	Texture * tex(NULL);

	bool genMips = (hasMipmaps) ? false : _genMipmaps;

	if(isCubemap)
	{
		tex = new TextureCube(width,height,ifmt,pfmt,dtype,texdata,GL_TEXTURE_CUBE_MAP,fileName,genMips);
	}
	else if(isVolume)
	{
		tex = new Texture3D(width,height,depth,ifmt,pfmt,dtype,texdata[0],GL_TEXTURE_3D,fileName,genMips);
	}
	else
	{
		bool loadrect = (compressed || _genMipmaps) ? false : _loadAsRect;
		unsigned target = loadrect ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
		tex = new Texture2D(width,height,ifmt,pfmt,dtype,texdata[0],target,fileName,genMips,loadrect);
	}

	MemMgrRaw::instance()->free(pixels);

    return tex;
}

bool TextureIO ::_saveDDS(const Texture * tex, 
						  const std::string& fname)
{
	CKLOG("DDS saving not implemented..",0);
	return false;
}