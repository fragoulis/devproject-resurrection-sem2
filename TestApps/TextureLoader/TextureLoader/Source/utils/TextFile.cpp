#include "TextFile.h"
#include <stdlib.h>

#include <dolphin.h>

TextFile :: TextFile(const char * infile)
:filename(0),
lines(0),
linenum(0)
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
			unsigned length = DVDGetLength(&finfo);
			void * data = OSAlloc(length);
			DVDRead(&finfo,data,read_needed,0);
			DVDClose(&finfo);

			// convert to lines
			// FIXME : implement it or don't bother yet?
		}
	}
}

TextFile :: ~TextFile()
{
	if(filename)
		OSFree(filename);
	if(linenum)
	{
		for(unsigned i=0;i<linenum;++i)
			OSFree(lines[i]);
		OSFree(lines);
	}
}