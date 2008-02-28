inline const GLuint TextureCube :: getHeight() const {return _height;}

inline void * TextureCube :: operator new(const unsigned size)
{
	return MemMgrAny<TextureCube>::instance()->allocate();
}

inline void TextureCube :: operator delete(void * p)
{
	MemMgrAny<TextureCube>::instance()->free(reinterpret_cast<TextureCube *>(p));
}
