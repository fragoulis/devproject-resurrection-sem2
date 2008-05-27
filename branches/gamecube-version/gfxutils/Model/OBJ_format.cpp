#include "OBJ_format.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "Model.h"
#include "ModelMgr.h"
#include "../Misc/utils.h"
#include "../../utility/RawFile.h"
#include "../../control/Gamecube/gc_new_delete.h"

using namespace std;


void Model :: _loadOBJ(const std::string& fname, bool loadTex)
{
	// Sanity check
	const string extension = getExtension(fname.c_str());
	ASSERT(extension == "obj");

	OBJModel objm;
	int current_material = -1;
	
	RawFile rf(fname.c_str());
	stringstream istr;
	rf.toStream(istr);

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
		if(token == "v")
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
		}	
	}

	// Be even more strict, all or nothing in ALL material groups
	for(unsigned i=0;i<objm._groups.size();++i)
	{
		if(objm._groups[i]._vfaces.empty())
		{
			objm._groups.erase(objm._groups.begin() + i);
			--i;
			continue;
		}
		if((objm._groups[i]._vnfaces.size() < objm._groups[i]._vfaces.size()) &&
		   (objm._groups[i]._vnfaces.size() > 0))
			objm._normal.clear();
		if(((objm._groups[i]._vtfaces.size() < objm._groups[i]._vfaces.size()) &&
		   (objm._groups[i]._vtfaces.size() > 0)))
			objm._texcoord.clear(); 
	}
	if(objm._groups.empty())
		ASSERT(0);

	// get basic attribs

	unsigned v_num = objm._vertex.size();
	unsigned vn_num = objm._normal.size();
	unsigned vt_num = objm._texcoord.size();
	unsigned f_num = 0;
	for(unsigned i=0;i<objm._groups.size();++i)
		f_num += objm._groups[i]._vfaces.size();

	//OSReport("Loaded model %s\n", m_name.c_str());
	//OSReport("***** Model Report : %s\n",m_name.c_str());
	//OSReport("***** Vertices     : %u\n",v_num);
	//OSReport("***** Normals      : %u\n",vn_num);
	//OSReport("***** Texcoords    : %u\n",vt_num);
	//OSReport("***** Triangles    : %u\n",f_num/3);

	m_polynum += f_num / 3;

	// allocate memory
	float * vdata = new float[v_num*3];
	float * vndata = new float[vn_num*3];
	float * vtdata = (vt_num) ? new float[vt_num*2] : 0;

	// copy the vertex data
	for(unsigned i=0;i<v_num;++i)
	{
		const unsigned out_i = i*3;
		vdata[out_i] = objm._vertex[i].getX();
		vdata[out_i + 1] = objm._vertex[i].getY();
		vdata[out_i + 2] = objm._vertex[i].getZ();
	}
	for(unsigned i=0;i<vn_num;++i)
	{
		const unsigned out_i = i*3;
		vndata[out_i] = objm._normal[i].getX();
		vndata[out_i + 1] = objm._normal[i].getY();
		vndata[out_i + 2] = objm._normal[i].getZ();
	}
	for(unsigned i=0;i<vt_num;++i)
	{
		const unsigned out_i = i*2;
		vtdata[out_i] = objm._texcoord[i].getX();
		vtdata[out_i + 1] = 1.0f - objm._texcoord[i].getY();		//INVERT TEXCOORDS!!
	}

	VFMT4::IndexStruct * notex_indices = 0;
	VFMT5::IndexStruct * tex_indices = 0;

	if(vt_num)
		tex_indices = new VFMT5::IndexStruct[f_num];
	else
		notex_indices = new VFMT4::IndexStruct[f_num];
	

	// make the indices
	unsigned total_i = 0;
	for(unsigned i=0;i<objm._groups.size();++i)
	{
		for(unsigned j=0;j<objm._groups[i]._vfaces.size();++j)
		{
			if(vt_num)
			{
				tex_indices[total_i].posi = objm._groups[i]._vfaces[j];
				tex_indices[total_i].nrm = objm._groups[i]._vnfaces[j];
				tex_indices[total_i].st = objm._groups[i]._vtfaces[j];
			}
			else
			{
				notex_indices[total_i].posi = objm._groups[i]._vfaces[j];
				notex_indices[total_i].nrm = objm._groups[i]._vnfaces[j];
			}
			total_i++;
		}
	}

	// Flush 

	if(vt_num)
	{
		DCFlushRange(tex_indices,sizeof(VFMT5::IndexStruct)*total_i);
		DCFlushRange(vtdata,vt_num*sizeof(float)*2);
	}
	else
		DCFlushRange(notex_indices,sizeof(VFMT4::IndexStruct)*total_i);

	DCFlushRange(vdata,v_num*sizeof(float)*3);
	DCFlushRange(vndata,vn_num*sizeof(float)*3);

	// Set the data

	if(vt_num)
	{
		m_isTextured = true;
		m_texData = new VFMT5();
		m_texData->xyz = vdata;
		m_texData->nrm = vndata;
		m_texData->st = vtdata;
		DCFlushRange(m_texData,sizeof(VFMT5));
		m_texVA = new VertexArray<VFMT5>();
		m_texVA->set(m_texData,GX_TRIANGLES,f_num,tex_indices);
	}
	else
	{
		m_notexData = new VFMT4();
		m_notexData->xyz = vdata;
		m_notexData->nrm = vndata;
		DCFlushRange(m_notexData,sizeof(VFMT4));
		m_notexVA = new VertexArray<VFMT4>();
		m_notexVA->set(m_notexData,GX_TRIANGLES,f_num,notex_indices);
	}


	// free resources 

	if(tex_indices)
		delete []tex_indices;
	else
		delete []notex_indices;
}
