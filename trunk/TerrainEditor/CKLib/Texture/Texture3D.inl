inline const GLuint Texture3D :: getHeight() const {return _height;}
inline const GLuint Texture3D :: getDepth() const {return _depth;}

inline void * Texture3D :: operator new(const unsigned size)
{
	return MemMgrAny<Texture3D>::instance()->allocate();
}

inline void Texture3D :: operator delete(void * p)
{
	MemMgrAny<Texture3D>::instance()->free(reinterpret_cast<Texture3D *>(p));
}
