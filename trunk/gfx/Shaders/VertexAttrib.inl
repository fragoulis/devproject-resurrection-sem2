// Access funcs
inline const unsigned VertexAttribute :: size() const
{return _size;}
inline const unsigned VertexAttribute :: components() const
{return _components;}
inline const unsigned VertexAttribute :: datatype() const
{return _datatype;}
inline const bool VertexAttribute :: built_in() const
{return _isBuiltIn;}
inline const std::string VertexAttribute :: name() const
{return _name;}
inline const std::string VertexAttribute :: nameInShader() const
{return _nameInShader;}
inline const unsigned VertexAttribute :: array_loc() const
{return _array_or_loc;}
inline const bool& VertexAttribute :: used() const
{return _used;}

inline void VertexAttribute :: set_loc(const unsigned& loc)
{_array_or_loc = loc;}

