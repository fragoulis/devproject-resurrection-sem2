#include <string>
#include <iostream>
#include "textureIO.h"
#include "texture.h"
#include "../../gfxutils/Misc/utils.h"
#include "../../gfxutils/Misc/defines.h"

using namespace std;

TextureIO * TextureIO :: _instance(NULL);
bool TextureIO :: _init(false);

TextureIO :: TextureIO(const ParserSection * parsec)
:_textureDir("./"),
_loadAsRect(false),
_saveMipmaps(true),
_genMipmaps(true),
_normalizeFloat(false)
{
	_parseConfiguration(parsec);
}

TextureIO :: ~TextureIO()
{}


void TextureIO :: _parseConfiguration(const ParserSection * parsec)
{
	_textureDir = parsec->getVal("DefaultDir");
}

Texture * TextureIO :: loadImage(const std::string& fname)
{
	// Create a new Texture based on the loaded data
	const string fullname = _textureDir + fname;


	Texture * tex = 0;
	const string ext = getExtension(fname.c_str());
	if((ext == "rgb") || (ext == "sgi"))
		tex = _loadSGI(fullname);
	else if(ext == "tex")
		tex = _loadTEX(fullname);
	else if(ext == "dds")
		tex = _loadDDS(fullname);
	else if(ext == "bmp")
		tex = _loadBMP(fullname);
	if(!tex)
	{
		cout<<"..ERROR!!"<<endl;
		return 0;
	}
	else
		_textureTemplates.push_back(tex);
	return tex;
}

bool TextureIO :: saveImage(const Texture * tex, 
						    const std::string& fname)
{
	ASSERT(tex);
	tex->bind();
	
	const string fullname = _textureDir + fname;

	bool res = false;
	const string ext = getExtension(fname.c_str());
	if((ext == "rgb") || (ext == "sgi"))
		res = _saveSGI(tex,fullname);
	else if(ext == "tex")
		res = _saveTEX(tex,fullname);
	else if(ext == "dds")
		res = _saveDDS(tex,fullname);
	if(!res)
	{
		cout<<"..ERROR!!"<<endl;
		return false;
	}
	return true;
}

Texture * TextureIO :: getTexture(const std::string& fname)
{
	for(size_t i=0;i<_textureTemplates.size();++i)
		if(_textureTemplates[i]->getName() == fname)
			return _textureTemplates[i];
	return loadImage(fname);
}
