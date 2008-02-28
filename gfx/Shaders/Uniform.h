#ifndef _Uniform_h_
#define _Uniform_h_

#include <string>
//#include <Parse/gParser.h>
#include <glee.h>

namespace CKLib {


/*
	Uniform :
	Holds uniform data, not used anymore (on the fly queries to gl instead)
*/
class ParserSection;
class Uniform
{
private:
	unsigned	_size;
	unsigned	_components;
	unsigned	_datatype;
	std::string _nameInShader;

	// built-in / custom attrib
	unsigned	_loc;

public : 

	// ctors
	Uniform(){}
	Uniform(const Uniform& uni);

	// dtor
	~Uniform(){}
	
	// Utils
	void parseConfiguration(const ParserSection * parsec); // 'i' is the appropriate category in parser 

	// Access funcs
	const unsigned size() const;
	const unsigned components() const;
	const unsigned datatype() const;
	const std::string nameInShader() const;
	const unsigned loc() const;
	void set_loc(const unsigned& loc);

};

#include <Shaders/Uniform.inl>

}

#endif