inline TextureMgr * TextureMgr :: instance()
{
	assert(_init);
	return _instance;
}

inline void TextureMgr :: setTextureTarget(const unsigned& target,const unsigned& i)
{
	ASSERT(i < MAX_TEXTURE_UNITS);
	if(target != _textureInfo[i].target)
	{
		if(_textureInfo[i].target)
			glDisable(_textureInfo[i].target);
		if(target)
			glEnable(target);
		_textureInfo[i].target = target;		
	}
}

inline void TextureMgr :: setBoundTexture(const unsigned& id,const unsigned& i)
{
	ASSERT(i < MAX_TEXTURE_UNITS);
	if((id != _textureInfo[i].id) && _textureInfo[i].target)
	{
		// Tex Bind ++ / Debugging code
		glBindTexture(_textureInfo[i].target,id);
		_textureInfo[i].id = id;
	}
}

inline void TextureMgr :: setTextureUnit(const unsigned& unit)
{
	ASSERT(unit < MAX_TEXTURE_UNITS);
	glActiveTexture(GL_TEXTURE0 + unit);
}

inline unsigned TextureMgr :: getTextureTarget(const unsigned& unit)
{return _textureInfo[unit].target;}
inline unsigned TextureMgr :: getBoundTexture(const unsigned& unit)
{return _textureInfo[unit].id;}
