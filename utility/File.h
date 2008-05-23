#ifndef _RES_FILE_H_
#define _RES_FILE_H_
#include <string>
#include <ostream>
#include <sstream>


class File
{
public:
	File(const std::string& fileName);
	File(const char* fileName);
	~File();

	const char* getBuffer() const { return m_buffer; }
	int getLength() const { return m_length; }
	//const ostream& getStream() const { return stringstream(m_buffer); }

private:
	std::string m_fileName;
	char* m_buffer;
	int m_length;


	void _load();
};



#endif