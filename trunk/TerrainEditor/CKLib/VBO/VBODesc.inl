inline void VBODesc :: call() const
{
	_vbo->call(_start_i,_size_i,_drawMode);
}

inline const unsigned VBODesc :: start_i() const {return _start_i;}
inline const unsigned VBODesc :: start_d() const {return _start_d;}
inline const unsigned VBODesc :: size_i() const {return _size_i;}
inline const unsigned VBODesc :: size_d() const {return _size_d;}
inline const unsigned VBODesc :: drawMode() const {return _drawMode;}