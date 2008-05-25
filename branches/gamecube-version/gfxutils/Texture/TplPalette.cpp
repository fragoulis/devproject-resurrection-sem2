#include "TplPalette.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../../utility/RawFile.h"
#include "../../control/Gamecube/gc_new_delete.h"
#include "TextureMgr.h"
#include <iostream>
#include <sstream>

using namespace std;

TplPalette :: TplPalette(const std::string& imgfile,const std::string& namefile)
:m_palettePtr(0),
m_name(imgfile)
{
	OSReport("Attempting to load %s --- %s..\n",imgfile.c_str(),namefile.c_str());
	string full = TextureMgr::instance().getTextureDir() + imgfile;
	// open img file
	TEXGetPalette(&m_palettePtr, (char *)(full.c_str()));

	// open names file
	const string full_names = TextureMgr::instance().getTextureDir() + namefile;
	RawFile names((char *)(full_names.c_str()));
	m_nameList.clear();
	if(names.length)
	{
		stringstream is;
		names.toStream(is);
		while((!is.eof()) && (is.good()))
		{
			string line;
			getline(is,line);
			if(line[line.size() - 1] == 13)
				line.erase(line.size()-1);
			if(line == "")
				continue;
			else if(line[0] == '#')
				continue;
			else
			{
				for(unsigned j=0;j<line.size();++j)
					OSReport("%d ",(int)(line[j]));
				OSReport("\n");
				m_nameList.push_back(line);
			}
		}
		OSReport("(%u bytes, %u names)Found under %s : \n",names.length,m_nameList.size(),namefile.c_str());
		for(unsigned i=0;i<m_nameList.size();++i)
			OSReport(" -- %s\n",m_nameList[i].c_str());
		ASSERTMSG(m_palettePtr->numDescriptors == m_nameList.size(),
					"Not equal number of names! tex has %u, while namefile has %u!\n",
					m_palettePtr->numDescriptors,
					m_nameList.size());
	}
	else
		OSReport("No namefile loaded for %s !!\n",namefile.c_str());
}

TplPalette :: ~TplPalette()
{
	TEXReleasePalette(&m_palettePtr);
}