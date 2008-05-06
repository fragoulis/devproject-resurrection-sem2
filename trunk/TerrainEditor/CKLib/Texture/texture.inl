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