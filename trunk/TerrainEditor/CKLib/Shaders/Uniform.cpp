#include <Shaders/Uniform.h>
#include <glee.h>
#include <string>
#include <ConfParser/ParserSection.h>
#include <assert.h>
#include <Misc/utils.h>

using namespace std;
using namespace CKLib;


Uniform :: Uniform(const Uniform& uni)
{
	_components = uni.components();
	_size = uni.size();
	_nameInShader  = uni.nameInShader();
	_datatype = uni.datatype();
	_loc = uni.loc();
}


void Uniform :: parseConfiguration(const ParserSection * parsec)
{
	_nameInShader = parsec->getName();
	string val;
	val = parsec->getVal("Components");
	assert(val != "");
	_components = FromString<unsigned>(val);
	val = parsec->getVal("Datatype");
	if(val == "float")
	{
		_datatype = GL_FLOAT;
	}
	else if(val == "int")
		_datatype = GL_INT;
	else
		assert(0);			// support only floats & ints at the moment

	_size = _components * (_datatype == GL_FLOAT) ? sizeof(float) : sizeof(int);
}


