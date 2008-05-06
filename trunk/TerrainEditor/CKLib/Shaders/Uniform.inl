// Access funcs
inline const unsigned Uniform :: size() const
{return _size;}
inline const unsigned Uniform :: components() const
{return _components;}
inline const unsigned Uniform :: datatype() const
{return _datatype;}
inline const std::string Uniform :: nameInShader() const
{return _nameInShader;}
inline const unsigned Uniform :: loc() const
{return _loc;}

inline void Uniform :: set_loc(const unsigned& loc)
{_loc = loc;}

