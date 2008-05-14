inline const GLuint Texture1D :: getHeight() const {return _height;}
inline const bool Texture1D :: isTexRect() const {return (_target == GL_TEXTURE_RECTANGLE_ARB);}

inline void * Texture1D :: operator new(const unsigned size)
{
	return MemMgrAny<Texture1D>::instance()->allocate();
}

inline void Texture1D :: operator delete(void * p)
{
	MemMgrAny<Texture1D>::instance()->free(reinterpret_cast<Texture1D *>(p));
}

