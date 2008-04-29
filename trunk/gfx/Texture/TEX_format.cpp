#include "TextureIO.h"
#include "Texture.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "TextureCube.h"
#include <string>
#include "../../gfxutils/MemManager/MemMgr_RawData.h"

using namespace std;

// Include it in texture.h & add a virtual func to return a struct for a given texture


/*
	unsigned magic,
			 width,
			 height,
			 depth,
			 ifmt,
			 pfmt,
			 datatype,
			 textype,
			 mipcount,
			 compressed;
	
	DATA
	Cubemap face ( if any)
		size
		Main Image
		size
		Mipmap1
		...
*/

Texture * TextureIO :: _loadTEX(const std::string& fname)
{
	// Log Event : Not Implemented Yet!
	FILE * fp = fopen(fname.c_str(),"rb");
	if( fp == NULL )
	{
       return 0;
	}
	TEX_header header;
	size_t read = fread(&header,sizeof(header),1,fp);
	if((!read) || (header.magic != 1234))
	{
       return 0;
	}
	vector<vector<MipmapLevel> > data;
	
	unsigned faces = (header.textype == TEXCUBE) ? 6 : 1;
	if(header.textype != TEX3D)
		header.depth = 1;

	// Get the size of mip 0 of the tex (or 1 of 6 in cubemaps)
	unsigned size(0);
	// Get the data
	for(unsigned i=0;i<faces;++i)
	{
		vector<MipmapLevel> single_data;
		for(unsigned j=0;j<header.mipcount;++j)
		{
			fread(&size,sizeof(unsigned),1,fp);
			MipmapLevel miplevel(MemMgrRaw::instance()->allocate<unsigned char>(size),size);
			fread(miplevel.data,1,size,fp);
			if(_genMipmaps || (!j))
				single_data.push_back(miplevel);
			else
				MemMgrRaw::instance()->free(miplevel.data);
		}
		data.push_back(single_data);
	}
	fclose(fp);

	header.mipcount = 1;

	Texture * tex(NULL);
	bool genMips = (header.mipcount > 1) ? false : _genMipmaps;

	// Create the textures
	switch(header.textype)
	{
		case TEX1D:
			break;
		case TEX2D :
			{
			bool loadrect = (header.compressed || _genMipmaps) ? false : _loadAsRect;
			unsigned target = loadrect ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
			tex = new Texture2D(header.width,header.height,header.ifmt,header.pfmt,
								header.datatype,data[0],target,truncDir(fname),genMips,loadrect);
			}
			break;
		case TEX3D :
			tex = new Texture3D(header.width,header.height,header.depth,header.ifmt,header.pfmt,
								header.datatype,data[0],GL_TEXTURE_3D,truncDir(fname),genMips);
			break;
		case TEXCUBE :
			tex = new TextureCube(header.width,header.height,header.ifmt,header.pfmt,
								header.datatype,data,GL_TEXTURE_CUBE_MAP,truncDir(fname),genMips);
			break;
		default :
			assert(0);
	}

	// Release memory
	for(unsigned i=0;i<faces;++i)
		for(unsigned j=0;j<header.mipcount;++j)
			MemMgrRaw::instance()->free(data[i][j].data);

	return tex;
}

bool TextureIO :: _saveTEX(const Texture * tex, 
						  const std::string& fname)
{
	TEX_header header = tex->getTEXheader();
	
	FILE * fp = fopen(fname.c_str(),"wb");
	if( fp == NULL )
	{
       return 0;
	}

	// Write header
	fwrite(&header,sizeof(header),1,fp);

	if(!_saveMipmaps)
		header.mipcount = 1;

	unsigned faces = (header.textype == TEXCUBE) ? 6 : 1;
	for(unsigned i=0;i<faces;++i)
	{
		for(unsigned j=0;j<header.mipcount;++j)
		{
			// write mipmap level
			unsigned size = tex->dataSize(j);
			unsigned char * data = MemMgrRaw::instance()->allocate<unsigned char>(size);
			if(header.textype == TEXCUBE)
				tex->dlData(data,CubemapFace(i),j);
			else
				tex->dlData(data,j);
			fwrite(&size,sizeof(unsigned),1,fp);
			fwrite(data,sizeof(unsigned char),size,fp);
			MemMgrRaw::instance()->free<unsigned char>(data);
		}
	}
	fclose(fp);

	return true;
}