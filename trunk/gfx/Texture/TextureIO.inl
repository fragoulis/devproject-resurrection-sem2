inline void TextureIO :: init(const ParserSection * parsec)
{
	if(!_init)
	{
		_instance = new TextureIO(parsec);
		_init = true;
	}
}
inline void TextureIO :: destroy()
{
	if(_init)
	{
		delete _instance;
		_init = false;
		_instance = NULL;
	}
}

inline TextureIO * TextureIO :: instance()
{
	assert(_instance);
	return _instance;
}

inline void TextureIO :: setOptionLoadAsRect(const bool opt) {_loadAsRect = opt;}
inline void TextureIO :: setOptionSaveMipmaps(const bool opt){_saveMipmaps = opt;}
inline void TextureIO :: setOptionGenMipmaps(const bool opt) {_genMipmaps = opt;}
inline void TextureIO :: setOptionNormalizeFloat(const bool opt) {_normalizeFloat = opt;}

inline bool TextureIO :: getOptionLoadAsRect() const {return _loadAsRect;}
inline bool TextureIO :: getOptionSaveMipmaps() const {return _saveMipmaps;}
inline bool TextureIO :: getOptionGenMipmaps() const {return _genMipmaps;}
inline bool TextureIO :: getOptionNormalizeFloat() const {return _normalizeFloat;}

inline const std::string& TextureIO :: getTextureDir() const {return _textureDir;}