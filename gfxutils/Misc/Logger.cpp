#include "Logger.h"
#include <string>
#include "../ConfParser/ParserSection.h"
#include "../../utility/assert.h"
#include "utils.h"

using namespace std;

Logger * Logger :: _instance(NULL);
bool Logger :: _init(false);

// ctor - dtor

Logger :: Logger(const ParserSection * parsec)		// ctor
:
_stream(&cout),
_isStreamFile(false),
_displayFileLine(true)
{
	_parseConfiguration(parsec);
}

Logger :: ~Logger()
{
	// close stream
	if(_isStreamFile)
		_ofs.close();
}

void Logger :: _parseConfiguration(const ParserSection * parsec)
{
	if(!parsec)
		return;
	const string stream = parsec->getVal("Stream");
	_level = FromString<unsigned>(parsec->getVal("Level"));
	string fname;
	if(stream == "stdout")
		_stream = &cout;
	else if(stream == "stderr")
		_stream = &cerr;
	else 
	{
		_isStreamFile = true;
		fname = parsec->getVal("LogFile");
		if(fname == "")
			fname = "Logfile.txt";
	}
	if(_isStreamFile)
	{
		_ofs.open(fname.c_str());
		_stream = &(_ofs);
	}

	_displayFileLine = FromString<unsigned>(parsec->getVal("DisplayFileLine")) ? true : false;
}

void Logger :: _log(const std::string& msg,const unsigned level,char *file, int line)
{
	if(level<= _level)
	{
		if(_displayFileLine)
			(*_stream)<<truncDir(file)<<" - "<<line<<" : "<<msg<<endl;
		else
			(*_stream)<<msg<<endl;
	}
}

// Statics

void Logger :: init(const ParserSection * parsec)
{
	if(!_init)
	{
		_instance = new Logger(parsec);
		_init = true;
	}
}

void Logger :: log(const std::string& msg,const unsigned level,char *file, int line)
{
	assert(_init);
	_instance->_log(msg,level,file,line);
}

void Logger :: destroy()
{
	if(_init)
	{
		delete _instance;
		_init = false;
	}
}