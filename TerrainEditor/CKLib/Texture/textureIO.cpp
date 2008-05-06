#include <string>
#include <iostream>
#include <Texture/textureIO.h>
#include <Texture/texture.h>
#include <Misc/utils.h>
#include <Misc/defines.h>
#include <Misc/Logger.h>

using namespace CKLib;
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
	vector<string> files = parsec->getValVector("Files");
	for(size_t i=0;i<files.size();++i)
		loadImage(files[i]);
}

Texture * TextureIO :: loadImage(const std::string& fname)
{
	// Create a new Texture based on the loaded data
	const string fullname = _textureDir + fname;

	return loadImageFullPath(fullname);
}

Texture * TextureIO :: loadImageFullPath(const std::string& fullname)
{
	// Create a new Texture based on the loaded data
	CKLOG(string("Loading File ") + fullname + string(" .."),2);

	Texture * tex = 0;
	const string ext = CKLib::getExtension(fullname.c_str());
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
	const string fullname = _textureDir + fname;

	return saveImageFullPath(tex,fullname);
}

bool TextureIO :: saveImageFullPath(const Texture * tex, 
						    const std::string& fullname)
{
	ASSERT(tex);
	tex->bind();

	bool res = false;
	const string ext = CKLib::getExtension(fullname.c_str());
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
