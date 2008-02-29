#include "OBJ_format.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <gl/glee.h>
#include "ModelMgr.h"
#include "Model.h"
#include "../VBO/vbo.h"
#include "../../gfxutils/MemManager/MemMgr_RawData.h"
#include "../VBO/VBOMgr.h"
#include "../Shaders/ShaderManager.h"

using namespace std;


static void _parseMTL(OBJModel& objm, const std::string& fname)
{
	ifstream istr(fname.c_str());
	string line;
	while((!istr.eof()) && (istr.good()))
	{
		getline(istr,line);
		if(line[0] == '#')		// If a comment
			continue;
		else if(line == "")
			continue;
		istringstream lstr(line);
		string token;
		lstr>>token;
		if(token == "newmtl")
		{
			lstr>>token;
			objm._groups.push_back(OBJModel::OBJGroup(token));
		}
		else if(token == "Ka")
		{
			float amb[4] = {0.0f,0.0f,0.0f,1.0f};
			lstr>>amb[0]>>amb[1]>>amb[2];
			objm._groups[objm._groups.size() - 1]._material.SetAmbient(Color(amb));
		}
		else if(token == "Kd")
		{
			float diff[4] = {0.0f,0.0f,0.0f,1.0f};
			lstr>>diff[0]>>diff[1]>>diff[2];
			objm._groups[objm._groups.size() - 1]._material.SetDiffuse(Color(diff));
		}
		else if(token == "Ks")
		{
			float spec[4] = {0.0f,0.0f,0.0f,1.0f};
			lstr>>spec[0]>>spec[1]>>spec[2];
			objm._groups[objm._groups.size() - 1]._material.SetSpecular(Color(spec));
		}
		else if(token == "Ns")
		{
			float shine;
			lstr>>shine;
			objm._groups[objm._groups.size() - 1]._material.SetShininess(shine);
		}
		else if(token == "Map_Ka")
		{
			lstr>>token;
			objm._groups[objm._groups.size() - 1]._texture = TextureIO::instance()->getTexture(token);
		}	
	}

}

Model * ModelMgr :: _loadOBJ(const std::string& fname,const unsigned usage_hint)
{
	// Sanity check
	const string extension = getExtension(fname.c_str());
	assert(extension == "obj");

	OBJModel objm;
	int current_material = -1;
	ifstream istr(fname.c_str());
	string line;
	while((!istr.eof()) && (istr.good()))
	{
		getline(istr,line);
		if(line[0] == '#')		// If a comment
			continue;
		else if(line == "")
			continue;
		istringstream lstr(line);
		string token;
		lstr>>token;
		if(token == "mtllib")
		{
			lstr>>token;
			_parseMTL(objm,m_modelDir + token);
		}
		else if(token == "v")
		{
			float v[4];
			lstr>>v[0]>>v[1]>>v[2];
			objm._vertex.push_back(Vector4(v));
		}
		else if(token == "vt")
		{
			float v[2];
			lstr>>v[0]>>v[1];
			objm._texcoord.push_back(Vector2(v));
		}
		else if(token == "vn")
		{
			float v[4];
			lstr>>v[0]>>v[1]>>v[2];
			objm._normal.push_back(Vector4(v));
		}
		else if(token == "usemtl")
		{
			lstr>>token;
			for(unsigned i=0;i<objm._groups.size();++i)
				if(token == objm._groups[i]._matname)
				{
					current_material = i;
					break;
				}
		}
		else if(token == "f")
		{
			if(current_material == -1)
			{
				objm._groups.push_back(OBJModel::OBJGroup("default"));
				objm._groups[objm._groups.size() - 1]._material = Material();
				current_material = static_cast<int>(objm._groups.size() - 1);
			}
			string triad1,triad2,triad3,v_str,vt_str,vn_str;
			size_t pos1,pos2;
			lstr>>triad1>>triad2>>triad3;

			Vector4 v,vt,vn;

			bool has_vt = true, has_vn = true;

			// 1st triad
			pos1 = triad1.find_first_of("/");
			pos2 = triad1.find_last_of("/");
			v_str = triad1.substr(0,pos1);
			vt_str = triad1.substr(pos1+1,pos2 - pos1 - 1);
			vn_str = triad1.substr(pos2+1,triad1.size() - pos2 - 1);
#pragma warning( push )	
#pragma warning( disable : 4244 )	
			v.setX(atoi(v_str.c_str()) - 1);
			if(!vt_str.empty())
				vt.setX(atoi(vt_str.c_str()) - 1);
			else 
				has_vt = false;
			if(!vn_str.empty())
				vn.setX(atoi(vn_str.c_str()) - 1);
			else
				has_vn = false;

			// 2nd triad
			pos1 = triad2.find_first_of("/");
			pos2 = triad2.find_last_of("/");
			v_str = triad2.substr(0,pos1);
			vt_str = triad2.substr(pos1+1,pos2 - pos1 - 1);
			vn_str = triad2.substr(pos2+1,triad2.size() - pos2 - 1);
			
			v.setY(atoi(v_str.c_str()) - 1);
			if(!vt_str.empty())
				vt.setY(atoi(vt_str.c_str()) - 1);
			else 
				has_vt = false;
			if(!vn_str.empty())
				vn.setY(atoi(vn_str.c_str()) - 1);
			else
				has_vn = false;

			// 3rd triad
			pos1 = triad3.find_first_of("/");
			pos2 = triad3.find_last_of("/");
			v_str = triad3.substr(0,pos1);
			vt_str = triad3.substr(pos1+1,pos2 - pos1 - 1);
			vn_str = triad3.substr(pos2+1,triad3.size() - pos2 - 1);
			
			v.setZ(atoi(v_str.c_str()) - 1);
			if(!vt_str.empty())
				vt.setZ(atoi(vt_str.c_str()) - 1);
			else 
				has_vt = false;
			if(!vn_str.empty())
				vn.setZ(atoi(vn_str.c_str()) - 1);
			else
				has_vn = false;


			objm._groups[current_material]._vfaces.push_back(v.getX());
			objm._groups[current_material]._vfaces.push_back(v.getY());
			objm._groups[current_material]._vfaces.push_back(v.getZ());
			if(has_vt)
			{
				objm._groups[current_material]._vtfaces.push_back(vt.getX());
				objm._groups[current_material]._vtfaces.push_back(vt.getY());
				objm._groups[current_material]._vtfaces.push_back(vt.getZ());
			}
			if(has_vn)
			{
				objm._groups[current_material]._vnfaces.push_back(vn.getX());
				objm._groups[current_material]._vnfaces.push_back(vn.getY());
				objm._groups[current_material]._vnfaces.push_back(vn.getZ());
			}
#pragma warning ( pop )
		}	
	}
	// Make sure that it's an all-or-nothing situation for tex & norms
	for(unsigned i=0;i<objm._groups.size();++i)
	{
		if((objm._groups[i]._vnfaces.size() < objm._groups[i]._vfaces.size()) &&
		   (objm._groups[i]._vnfaces.size() > 0))
			objm._groups[i]._vnfaces.clear();
		if(((objm._groups[i]._vtfaces.size() < objm._groups[i]._vfaces.size()) &&
		   (objm._groups[i]._vtfaces.size() > 0)) || (!(objm._groups[i]._texture)))
			objm._groups[i]._vtfaces.clear(); 
	}
	if(objm._groups.empty())
		assert(0);

	// Make the vertexAttrib vector
	std::vector<const VertexAttribute *> attrs;
	attrs.push_back(ShaderManager::instance()->vertexAttribute("Vertex"));
	if(!objm._normal.empty())
		attrs.push_back(ShaderManager::instance()->vertexAttribute("Normal"));
	if(!objm._texcoord.empty())
		attrs.push_back(ShaderManager::instance()->vertexAttribute("Texcoord"));

	/*
		############## SCALE HACK!!!! #####################
	*/
	
	const size_t size = objm._vertex.size();
	for(size_t i=0;i<size;++i)
	{
		objm._vertex[i].multiply(0.2f);
	}

	/*
		############## END OF SCALE HACK!!!! #####################
	*/


	// Fetch the VBO that we'll use
	size_t max_elems(0);
	for(size_t i=0;i<objm._groups.size();++i)
		max_elems += objm._groups[i]._vfaces.size();
	
	VBO * vbo = VBOMgr::instance().fetchVBO(usage_hint,unsigned(max_elems),unsigned(max_elems),attrs);
	
	
	// Now create the model-matgroup hierarchy
	Model * model = new Model(truncDir(fname),vbo);

	for(size_t i =0;i< objm._groups.size() ; ++i)
	{
		const size_t total_elems = objm._groups[i]._vfaces.size();
		// Make the data vector
		std::vector<void *> attr_data;
		for(size_t vi=0;vi<attrs.size();++vi)
		{
			if(attrs[vi]->name() == "Vertex")
			{
				// Fill vertices
				attr_data.push_back(MemMgrRaw::instance()->allocate<Vector4>(unsigned(total_elems)));
				for(size_t j=0;j<total_elems;++j)
					reinterpret_cast<Vector4 *>(attr_data[vi])[j] = objm._vertex[objm._groups[i]._vfaces[j]];
			}
			else if(attrs[vi]->name() == "Normal")
			{
				// Fill normals
				attr_data.push_back(MemMgrRaw::instance()->allocate<Vector4>(unsigned(total_elems)));
				for(size_t j=0;j<total_elems;++j)
				{
					reinterpret_cast<Vector4 *>(attr_data[vi])[j] = objm._normal[objm._groups[i]._vnfaces[j]];
				}
			}
			else if(attrs[vi]->name() == "Texcoord")
			{
				// Fill texcoords
				attr_data.push_back(MemMgrRaw::instance()->allocate<Vector2>(unsigned(total_elems)));
				for(size_t j=0;j<total_elems;++j)
				{
					reinterpret_cast<Vector2 *>(attr_data[vi])[j] = objm._texcoord[objm._groups[i]._vtfaces[j]];
				}
			}
		}

		// Create & fill the index data

		unsigned * index_data = MemMgrRaw::instance()->allocate<unsigned>(unsigned(total_elems));
		for(unsigned ii=0;ii<total_elems;++ii)
					index_data[ii] = ii;

		std::vector<Texture *> tl;
		if(objm._groups[i]._texture)
			tl.push_back(objm._groups[i]._texture);

		model->addMatGroup(MaterialGroup(objm._groups[i]._material,
										 tl,
										 VBODesc(vbo,
												attrs,
												attr_data,
												unsigned(total_elems),
												index_data,
												unsigned(total_elems),
												GL_TRIANGLES)));

		for(size_t vi=0;vi<attrs.size();++vi)
			MemMgrRaw::instance()->free(attr_data[vi]);
		MemMgrRaw::instance()->free(index_data);
	}
	
	return model;
}
