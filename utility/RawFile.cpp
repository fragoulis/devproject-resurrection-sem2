#include "RawFile.h"
#include <stdlib.h>

#include "../control/Gamecube/gc_new_delete.h"

#include <dolphin.h>

using namespace std;

RawFile :: RawFile(const char * infile)
:filename(0),
data(0),
length(0)
{
	if(infile)
	{
		// copy the filename
		filename = (char *)OSAlloc(strlen(infile));
		strcpy(filename,infile);

		// open the file

		DVDFileInfo finfo;
		BOOL res = DVDOpen(infile, &finfo);
		if(res)
		{
			unsigned read_needed = OSRoundUp32B(DVDGetLength(&finfo));
			data = OSAlloc(read_needed + 1);
			DVDRead(&finfo,data,read_needed,0);
			DVDClose(&finfo);
			length = DVDGetLength(&finfo);
			for(unsigned i=length;i<read_needed;++i)
				((char *&)data)[i] = 0;
		}
	}
}

RawFile :: ~RawFile()
{
	if(filename)
		OSFree(filename);
	if(data)
		OSFree(data);
}

void RawFile :: toStream(iostream& os)
{
	os<<string((char *)data);
}