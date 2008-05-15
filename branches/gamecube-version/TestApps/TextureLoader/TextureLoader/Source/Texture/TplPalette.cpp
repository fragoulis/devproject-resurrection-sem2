#include "TplPalette.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../utils/RawFile.h"

TplPalette :: TplPalette(char * imgfile,char * namefile)
:m_nameList(0),
m_palettePtr(0)
{
	// open img file
	TEXGetPalette(&m_palettePtr, imgfile);

	unsigned surfaceNum = m_palettePtr->numDescriptors;

	// open names file
	RawFile names(namefile);
	if(names.length)
	{
		// read the names list, separated by newlines
		u8 * current = (u8 *)names.data;
		
		m_nameList = (char **)OSAlloc(sizeof(char *)*surfaceNum);
		unsigned at_line = 0;
		unsigned i = 0;
		unsigned line_i;
		while((current[i] != '\0') && (at_line != surfaceNum))
		{
			m_nameList[at_line] = (char *)OSAlloc(32);
			line_i = 0;
			while(current[i] != 13)
			{
				m_nameList[at_line][line_i] = current[i];
				++i;
				++line_i;
			}
			m_nameList[at_line][line_i] = '\0';
			++at_line;
			i+=2;
		}

		for(unsigned i=0;i<getSurfaceNum();++i)
		{
			char buffer[512];
			sprintf(buffer,"texture %s : %d %d\n",m_nameList[i],m_palettePtr->descriptorArray[i].textureHeader->width,
															  m_palettePtr->descriptorArray[i].textureHeader->height);
			OSReport(buffer);									
		}
	}
	
}

TplPalette :: ~TplPalette()
{
	for(unsigned i=0;i<getSurfaceNum();++i)
		OSFree(m_nameList[i]);
	OSFree(m_nameList);
	// free tpl palette
}