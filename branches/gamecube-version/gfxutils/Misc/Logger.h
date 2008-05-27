#ifndef _RES_LOGGER_H_
#define _RES_LOGGER_H_

#include <string>
#include <fstream>
#include <iostream>

#ifdef DEBUG
	#define CKLOG(a,b) Logger::log((a),(b),__FILE__,__LINE__)
#else
	#define CKLOG(a,b)
#endif

/*
	Logger class 
*/

class ParserSection;
class Logger
{
private :
	std::ostream  * _stream;
	unsigned	  _level;		// Descending priority
	std::ofstream _ofs;
	bool		  _isStreamFile;
	bool		  _displayFileLine;

	static Logger * _instance;
	static bool		_init;

	void _log(const std::string& msg,const unsigned level,char *file, int line);
	void _parseConfiguration(const ParserSection * parsec);

	// prohibited funcs 
	Logger(const ParserSection * parsec);									// ctor
	~Logger();																// dtor
	Logger(const Logger& logger){}											// copy ctor
	void operator = (const Logger& logger){}								// assign op

public :

	// Singleton Trio Stooges
	static void init(const ParserSection * parsec = 0);
	static void log(const std::string& msg,const unsigned level,char *file, int line);
	static void destroy();
};



#endif