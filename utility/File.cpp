#include "File.h"
#include <dolphin/dvd.h>
#include <dolphin/os.h>
#include <iostream>
using namespace std;

File :: File(const std::string& fileName) : m_buffer(0)
{
	m_fileName = fileName;
	_load();
}

File :: File(const char* fileName) : m_buffer(0)
{
	m_fileName = fileName;
	_load();
}

File :: ~File()
{
	if (m_buffer != 0) delete [] m_buffer;
}


void File :: _load()
{
	DVDFileInfo fileInfo;
	if (DVDOpen(m_fileName.c_str(), &fileInfo) == FALSE)
	{
		cerr << "Couldn't open file " << m_fileName << endl;
		OSHalt("");
	}

	m_length = DVDGetLength(&fileInfo) + 1;
	m_buffer = new char[m_length];

	if (DVDRead(&fileInfo, m_buffer, OSRoundUp32B(m_length), 0) == 0)
	{
		cout << "Couldn't read file " << m_fileName << endl;
		OSHalt("");
	}

	m_buffer[m_length-1] = 0;

	DVDClose(&fileInfo);
}