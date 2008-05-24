#ifndef __OBJ_FORMAT_H
#define __OBJ_FORMAT_H

//#include "../Texture/Texture.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector4.h"
#include <string>
#include <vector>


/*
	OBJModel :

	Class that loads Wavefront OBJ model data based on a subset of the specification
*/
struct OBJModel
{
	public : 
	/*
		MaterialGroup :
		class that holds the data for each different material
	*/
	class OBJGroup
	{
	public :
		std::vector<unsigned >  _vfaces;
		std::vector<unsigned >  _vtfaces;
		std::vector<unsigned >  _vnfaces;
		std::string			   _matname;		// for indexing
		std::string			   _texture;
		OBJGroup(const std::string& str)
			:_matname(str),
			 _texture("")
				{}
	};

	std::vector<Vector4>	  _vertex;
	std::vector<Vector4>	  _normal;
	std::vector<Vector2>	  _texcoord;
	std::vector<OBJGroup>	  _groups;
};

#endif