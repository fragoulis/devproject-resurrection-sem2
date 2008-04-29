#include "Texture.h"
#include "../../gfxutils/Misc/utils.h"

using namespace std;

Texture :: Texture(const unsigned w,const unsigned ifmt,
			const unsigned pfmt,const unsigned dtype,const unsigned target,
			const std::string& texname,const bool genMipmaps)
			:_target(target),_width(w),_pixelFormat(pfmt),_internalFormat(ifmt),_datatype(dtype),_name(texname),
			_bytesPerTexel(0)
{
	glGenTextures(1,&_id);
}

Texture :: ~Texture()
{
	glDeleteTextures(1,&_id);
	_id = 0;
}

std::string Texture :: infoString(const TEX_header& h)
{
	string nl("\n");
	string out;
	out += string("Texture info : \n");
	switch(h.textype)
	{
	case TEX1D : 
		out += string("Texture 1D\n");
		break;
	case TEX2D : 
		out += string("Texture 2D\n");
		break;
	case TEX3D : 
		out += string("Texture 3D\n");
		break;
	case TEXCUBE : 
		out += string("Texture Cubemap\n");
		break;
	}
	out += string("Width : ") + ToString<unsigned>(h.width) + nl;
	if(h.textype != TEX1D)
		out += string("Height : ") + ToString<unsigned>(h.height) + nl;
	if(h.textype == TEX3D)
		out += string("Depth : ") + ToString<unsigned>(h.depth) + nl;
	out += string("Compressed : ") + ((h.compressed) ? string("true\n") : string("false\n"));
	out += string("Internal Format : ") + ToString<unsigned>(h.ifmt) + nl;
	out += string("Pixel Format : ") + ToString<unsigned>(h.pfmt) + nl;
	out += string("Datatype : ") + ToString<unsigned>(h.datatype) + nl;
	out += string("Mipmaps : ") + ToString<unsigned>(h.mipcount -1) + nl;

	return out;
}