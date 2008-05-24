#ifndef __RawFile_H__
#define __RawFile_H__

#include <iostream>

struct RawFile
{
	char * filename;
	void * data;
	unsigned length;

	RawFile(const char * infile);
	void toStream(std::iostream& os);
	~RawFile();
};

#endif