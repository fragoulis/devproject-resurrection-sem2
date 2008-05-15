#ifndef __TextFile_H__
#define __TextFile_H__

struct TextFile
{
	char * filename;
	char ** lines;
	unsigned linenum;

	TextFile(const char * infile);
	~TextFile();
};

#endif