inline const unsigned VBO :: dataSize()
{return _dataSize;}
inline const unsigned VBO :: indexSize()
{return _indexSize;}

inline unsigned VBO :: drawUsage() const
{return _drawUsage;}

inline void VBO :: drawUsage(const unsigned flag)
{_drawUsage = flag;}

inline const unsigned VBO :: dataOffset() const
{return _used_data;}

inline const unsigned VBO :: indexOffset() const
{return _used_index;}

inline const std::string& VBO :: getConfiguration() const
{return _configName;}

/*

inline unsigned& VBO :: dataOffset()
{return _used_data;}



inline unsigned& VBO :: indexOffset()
{return _used_index;}
*/

