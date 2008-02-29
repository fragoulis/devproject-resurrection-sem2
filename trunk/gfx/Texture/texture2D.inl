inline const GLuint Texture2D :: getHeight() const {return _height;}
inline const bool Texture2D :: isTexRect() const {return (_target == GL_TEXTURE_RECTANGLE_ARB);}

inline void * Texture2D :: operator new(const unsigned size)
{
	return MemMgrAny<Texture2D>::instance()->allocate();
}

inline void Texture2D :: operator delete(void * p)
{
	MemMgrAny<Texture2D>::instance()->free(reinterpret_cast<Texture2D *>(p));
}

