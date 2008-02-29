#include "../Misc/utils.h"
#include <string>
#include <glee.h>
#include <GL/glu.h>
#include <iostream>

using namespace std;




std::string getExtension(const char *fname)
{
	char out[512];
	unsigned i=0,j=0;
	unsigned last_found = 0;
	while(fname[i] != 0)
	{
		if(fname[i] == '.')
			last_found = i;
		++i;
	}
	i=last_found+1;
	while(fname[i] != 0)
		out[j++] =fname[i++];
	out[j] = 0;
	return string(out);
}

std::string truncDir(const std::string& full)
{
	size_t pos = full.find_last_of("/\\");
	if(pos != string::npos)
		return full.substr(pos+1,full.size() - pos-1);
	else
		return full;
}

bool isPOT(const unsigned val)
{
	unsigned i=2;
	while(i<val)
	{i<<=1;}
	return (i == val);
}

unsigned nearestPOT(const unsigned number)
{
	unsigned test = 1;
	while(test < number)
		test<<=1;
	return test;
}

// GL ERROR CHECK
int CheckGLError(char *file, int line)
{
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	while (glErr != GL_NO_ERROR) 
   {
       cout << "GL Error #" << glErr << "(" << gluErrorString(glErr) << ") " << " in File " << file << " at line: " << line << endl;
		retCode = 1;
		glErr = glGetError();
		assert(0);
	}
	return retCode;
}

// GL ENUMERATION of IFMT & DTYPE
unsigned componentsByInternalFormat(const GLuint ifmt)
{
	switch(ifmt)
	{
		case 1 :
		case GL_LUMINANCE :
		case GL_SLUMINANCE :
		case GL_ALPHA :
		case GL_DEPTH_COMPONENT:
		case GL_DEPTH_COMPONENT16:
		case GL_DEPTH_COMPONENT24:
		case GL_DEPTH_COMPONENT32:

			return 1;
		
		case 2 :
		case GL_LUMINANCE_ALPHA :
		case GL_SLUMINANCE_ALPHA :
			return 2;

		case 3 :
		case GL_RGB :
		case GL_RGB8 :
		case GL_SRGB :
		case GL_SRGB8 :
		case GL_COMPRESSED_RGB_S3TC_DXT1_EXT :
			return 3;

		case 4:
		case GL_RGBA :
		case GL_SRGB_ALPHA :
		case GL_SRGB8_ALPHA8 :
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT :
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT :
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT :
			return 4;

		default :
			assert(0);
	}
	return 0;
}

unsigned BytesPerComponentByDatatype(const GLuint dtype)
{
	switch(dtype)
	{
		case GL_UNSIGNED_BYTE:
		case GL_BYTE:
			return 1;
		case GL_UNSIGNED_INT :
		case GL_INT :
			return sizeof(GLint);

		case GL_FLOAT : 
			return sizeof(GLfloat);
		
		default :
			assert(0);
	}
	return 0;
}

