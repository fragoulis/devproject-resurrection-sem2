#include "RawFile.h"
#include <stdlib.h>

#include <dolphin.h>

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
			length = DVDGetLength(&finfo);
			data = OSAlloc(length);
			DVDRead(&finfo,data,read_needed,0);
			DVDClose(&finfo);
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