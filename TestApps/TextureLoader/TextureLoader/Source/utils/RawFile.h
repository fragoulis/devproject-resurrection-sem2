#ifndef __RawFile_H__
#define __RawFile_H__

struct RawFile
{
	char * filename;
	void * data;
	unsigned length;

	RawFile(const char * infile);
	~RawFile();
};

#endif