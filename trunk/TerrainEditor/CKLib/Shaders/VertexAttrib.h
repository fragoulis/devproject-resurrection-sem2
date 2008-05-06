#ifndef _VertexAttrib_h_
#define _VertexAttrib_h_

//#include <Parse/gParser.h>
#include <string>

namespace CKLib {

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

#include <Shaders/VertexAttrib.inl>

}

#endif