#pragma once

//#include <Parse/gParser.h>
#include <string>

/*
	VertexAttribute : 
	class for vertex attributes. The types of vertex attribs are parsed from
	the config file & here their traits are stored for later use.
*/
class ParserSection;
class VertexAttribute
{
private:
	unsigned	_size;
	unsigned	_components;
	unsigned	_datatype;
	bool		_isBuiltIn;
	std::string _name,_nameInShader;
	bool		_used;

	// built-in / custom attrib
	unsigned	_array_or_loc;		// constant for vertex/normal/color/texcoord, but changes dynamically
									// for others, then it gets it's value at each shader change 
									// from the shader manager

public : 

	// ctors
	VertexAttribute():_used(false){}

	// dtor
	~VertexAttribute(){}
	
	// Utils
	void parseConfiguration(const ParserSection * parsec);

	// Access funcs
	const unsigned size() const;
	const unsigned components() const;
	const unsigned datatype() const;
	const bool built_in() const;
	const std::string name() const;
	const std::string nameInShader() const;
	const unsigned array_loc() const;
	const bool& used() const;
	//bool& used();

	void set_loc(const unsigned& loc);

	// GL changing states
	void EnableClientArray();
	void DisableClientArray();
	void setAttribPointer(const void * pointer =0) const;

};

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

