#pragma once
#include <glee.h>
#include "../../gfxutils/MemManager/MemMgr_AnyData.h"
#include "TextureIO.h"
#include "TextureMgr.h"


struct MipmapLevel
{
	unsigned char * data;
	unsigned size;
	MipmapLevel():data(0),size(0){}
	MipmapLevel(unsigned char * d,const unsigned s):data(d),size(s){}
};

enum TexType
{
	TEX1D,
	TEX2D,
	TEX3D,
	TEXCUBE
};

struct TEX_header
{
	unsigned magic,
			 width,
			 height,
			 depth,
			 ifmt,
			 pfmt,
			 datatype,
			 mipcount,
			 compressed;
	TexType  textype;
};

enum CubemapFace
{
	CM_POSX = 0,
	CM_NEGX,
	CM_POSY,
	CM_NEGY,
	CM_POSZ,
	CM_NEGZ,
};

/*
	Texture : 
	Base class for textures
*/

class Texture 
{
protected :

	GLuint	 _width,
			 _pixelFormat,
			 _internalFormat,
			 _datatype,
			 _maxMiplevel,
			 _isCompressed,
			 _bytesPerTexel,
			 _id,
			 _target;

	std::string _name;
	TexType _type;

	// No copy ctor
	Texture(const Texture& tex);

public :
	Texture(const unsigned w,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,const unsigned target,
			const std::string& texname,const bool genMipmaps);
	virtual ~Texture();
	
	//virtual void ulSubData(const void * buffer,const const GLuint miplevel = 0) const;
	//virtual void dlSubData(void * buffer,const GLuint miplevel = 0) const;
	//virtual void newTexture(); // specify all params needed

	// Accessors

	const GLuint getWidth() const;
	const GLuint getPixelFormat() const;
	const GLuint getInternalFormat() const;
	const GLuint getDatatype() const;
	const GLuint getMaxMiplevel() const;
	const GLuint isCompressed() const;
	const TexType getType() const;
	const GLuint getTarget() const;
	const GLuint getId() const;
	const std::string& getName() const;

	virtual const GLuint getHeight() const	{return 0;}
	virtual const bool isTexRect() const {return false;}
	virtual const GLuint getDepth() const	{return 0;}

	virtual void ulData(const void * buffer,const GLuint miplevel = 0) const{};
	virtual void dlData(void * buffer,const GLuint miplevel = 0) const{};
	virtual void ulData(const void * buffer,const CubemapFace face,const GLuint miplevel = 0) const{};
	virtual void dlData(void * buffer,const CubemapFace face,const GLuint miplevel = 0) const{};

	virtual TEX_header getTEXheader() const = 0;

	void bind(const GLuint texunit = 0) const;
	void setParam(const GLuint pname, const GLuint param,const GLuint texunit = 0) const;

	virtual const GLuint dataSize(const int miplevel =0) const = 0;	// miplevel == -1 > ALL levels

	static std::string infoString(const TEX_header& h);
};

inline const TexType Texture :: getType() const {return _type;}
inline const GLuint Texture :: getWidth() const {return _width;}
inline const GLuint Texture :: getPixelFormat() const {return _pixelFormat;}
inline const GLuint Texture :: getInternalFormat() const {return _internalFormat;}
inline const GLuint Texture :: getDatatype() const {return _datatype;}
inline const GLuint Texture :: getMaxMiplevel() const {return _maxMiplevel;}
inline const GLuint Texture :: isCompressed() const {return _isCompressed;}
inline const GLuint Texture :: getId() const {return _id;}
inline const GLuint Texture :: getTarget() const {return _target;}
inline const std::string& Texture :: getName() const {return _name;}

inline void Texture :: bind( const GLuint texunit) const
{
	assert(texunit < MAX_TEXTURE_UNITS);
	TextureMgr::instance()->setTextureUnit(texunit);
	TextureMgr::instance()->setTextureTarget(_target,texunit);
	TextureMgr::instance()->setBoundTexture(_id,texunit);
}

inline void Texture :: setParam( const GLuint pname, const GLuint param,const GLuint texunit) const
{
	bind(texunit);
	glTexParameteri(_target,pname,param);
}

