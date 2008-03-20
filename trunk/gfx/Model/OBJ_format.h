//*****************************************************************************
//**
//**   File:               OBJ_format.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include "../Texture/Texture.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector4.h"
#include "../Material.h"
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
		Material		      _material;
		std::vector<unsigned >  _vfaces;
		std::vector<unsigned >  _vtfaces;
		std::vector<unsigned >  _vnfaces;
		std::string			   _matname;		// for indexing
		Texture				*  _texture;
		OBJGroup(const std::string& str)
			:_matname(str),
			_material(),
			_texture(0)
				{}
	};

	std::vector<Vector4>	  _vertex;
	std::vector<Vector4>	  _normal;
	std::vector<Vector2>	  _texcoord;
	std::vector<OBJGroup>	  _groups;
};

