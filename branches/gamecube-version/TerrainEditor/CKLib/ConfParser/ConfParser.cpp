#include <ConfParser/ConfParser.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace CKLib;
using namespace std;

// ParserToken

void ConfParser ::ParserToken :: clearBlanks()
{
	switch(type)
	{
		case CONFPARSER_TOKEN_TAG : 
		case CONFPARSER_TOKEN_HEADER :
			// Remove all blanks
			{
				size_t pos=0;
				while(pos < token.size())
				{
					if((token[pos] == ' ') || (token[pos] == '\t'))
						token.erase(pos--,1);
					++pos;
				}
				break;
			}
		case CONFPARSER_TOKEN_VALUE : 
			// If NOT string literal
			if(token[0] != '\"')
			{
				// make all tabs spaces
				replace(token.begin(),token.end(),'\t',' ');
				// Erase all consecutive blanks
				size_t pos = 0;
				while(pos < (token.size()-1))
				{
					if((token[pos] == ' ') && (token[pos+1] == ' '))
						token.erase(pos,1);
					else
						++pos;
				}
			}
			else
			{
				token.erase(token.size()-1,1);
				token.erase(0,1);
			}
			break;
	}
}

// ConfParser

void ConfParser :: _removeLeadBlanks(std::string& line) const
{
	// Remove leading blanks
	size_t pos = line.find_first_not_of(" \t");
	if(pos != string::npos)
		line.erase(0,pos);
}

void ConfParser :: _removeTrailBlanks(std::string& line) const
{
	// Remove trailing blanks
	size_t pos = line.find_last_not_of(" \t");
	if(pos != string::npos)
		line.erase(pos+1,line.size() - pos);	
}

ConfParser :: ConfParser(const std::string& fname)
{
	_validConfFile = true;
	// Open file & start reading loop
	ifstream ifs(fname.c_str());
	_parseFile(ifs);
	ifs.close();
	assert(_validConfFile);
}

void ConfParser :: _parseFile(ifstream& ifs)
{
	string line;
	unsigned linenum = 0;
	parser_error_t error = CONFPARSER_NO_ERROR;
	parser_status_t status = CONFPARSER_MAIN;
	vector<ParserToken> tokenlist;
	while(ifs.good())
	{
		getline(ifs,line);
		if((error = _parseLine(status,line,tokenlist)) != CONFPARSER_NO_ERROR)
		{
			cout<<"Parsing error type "<<error<<" at line : "<<linenum<<endl;
			return;
		}
		linenum++;
	}
	
	ParserSection * cursection = &_rootSection;
	string last_tag;
	vector<ParserToken>::iterator token_it = tokenlist.begin();
	while(token_it != tokenlist.end())
	{
		// Clean tokenlist of useless spaces
		_removeLeadBlanks(token_it->token);
		_removeTrailBlanks(token_it->token);
		token_it->clearBlanks();
		// make the sections
		switch(token_it->type)
		{
			case CONFPARSER_TOKEN_HEADER :
				cursection = _rootSection.getCreateSection(token_it->token);
				break;

			case CONFPARSER_TOKEN_TAG :
				last_tag = token_it->token;
				break;
			case CONFPARSER_TOKEN_VALUE :
				cursection->addVal(last_tag,token_it->token);
				break;
		}
		++token_it;
	}
}

ConfParser::parser_error_t ConfParser :: _parseLine(parser_status_t& status, 
							  std::string& line, 
							  std::vector<ParserToken>& tokenlist) const
{
	_removeTrailBlanks(line);
	while(!line.empty())
	{
		_removeLeadBlanks(line);
		switch(status)
		{
			case CONFPARSER_MAIN : 
				if(_isComment(line))
					return CONFPARSER_NO_ERROR;
				else if(_isHeader(line))
				{
					if(_parseHeader(line,tokenlist))
						status = CONFPARSER_HEADER;
					else
						return CONFPARSER_HEADER_ERROR;
				}
				else
					return CONFPARSER_MAIN_ERROR;
				break;

			case CONFPARSER_HEADER : 
				if(_isComment(line))
					return CONFPARSER_NO_ERROR;
				else
				{
					if(_parseTag(line,tokenlist))
						status = CONFPARSER_TAG;
					else 
						return CONFPARSER_TAG_ERROR;
				}
				break;

			case CONFPARSER_TAG : 
				if(_isVecValue(line))
				{
					if(_parseVecValue(line,tokenlist))
						status = CONFPARSER_VECVALUE;
					else
						return CONFPARSER_VECVALUE_ERROR;
				}
				else
				{
					if(_parseValue(line,tokenlist))
						status = CONFPARSER_VALUE;
					else
						return CONFPARSER_VALUE_ERROR;
				}
				break;

			case CONFPARSER_VALUE :
				if(_isComment(line))
					return CONFPARSER_NO_ERROR;
				else if(_isHeader(line))
				{
					if(_parseHeader(line,tokenlist))
						status = CONFPARSER_HEADER;
					else
						return CONFPARSER_HEADER_ERROR;
				}
				else
				{
					if(_parseTag(line,tokenlist))
						status = CONFPARSER_TAG;
					else 
						return CONFPARSER_TAG_ERROR;
				}
				break;

			case CONFPARSER_VECVALUE :
				if(_isComment(line))
					return CONFPARSER_NO_ERROR;
				else if(_isComma(line))
				{
					line.erase(0,1);
					_removeLeadBlanks(line);
					status = CONFPARSER_COMMA;
				}
				else if(_isHeader(line))
				{
					if(_parseHeader(line,tokenlist))
						status = CONFPARSER_HEADER;
					else
						return CONFPARSER_HEADER_ERROR;
				}
				else
				{
					if(_parseTag(line,tokenlist))
						status = CONFPARSER_TAG;
					else 
						return CONFPARSER_TAG_ERROR;
				}
				break;

			case CONFPARSER_COMMA : 
				if(_parseVecValue(line,tokenlist))
					status = CONFPARSER_VECVALUE;
				else
					return CONFPARSER_VECVALUE_ERROR;
				break;
		}
	}
	return CONFPARSER_NO_ERROR;
}



// Parse by type functions
bool ConfParser :: _parseHeader(std::string& line,std::vector<ParserToken>& tokenlist) const
{
	size_t start_pos = 0, end_pos = 0;
	end_pos = line.find_first_of(']');
	if(end_pos == string::npos)
		return false;
	tokenlist.push_back(ParserToken(CONFPARSER_TOKEN_HEADER,line.substr(start_pos + 1,end_pos - start_pos - 1)));
	line.erase(0,end_pos+1);
	return true;
}

bool ConfParser :: _parseTag(std::string& line,std::vector<ParserToken>& tokenlist) const
{
	size_t start_pos = 0, end_pos = 0;
	end_pos = line.find_first_of('=');
	if(end_pos == string::npos)
		return false;
	tokenlist.push_back(ParserToken(CONFPARSER_TOKEN_TAG,line.substr(start_pos,end_pos - start_pos - 1)));
	line.erase(0,end_pos+1);
	return true;
}

bool ConfParser :: _parseValue(std::string& line,std::vector<ParserToken>& tokenlist) const
{
	tokenlist.push_back(ParserToken(CONFPARSER_TOKEN_VALUE,line));
	line.clear();
	return true;
}

bool ConfParser :: _parseVecValue(std::string& line,std::vector<ParserToken>& tokenlist) const
{
	size_t start_pos = 0, end_pos = 0;
	end_pos = line.find_first_of('>');
	if(end_pos == string::npos)
		return false;
	tokenlist.push_back(ParserToken(CONFPARSER_TOKEN_VALUE,line.substr(start_pos + 1,end_pos - start_pos - 1)));
	line.erase(0,end_pos+1);
	return true;
}

// isSth functions
bool ConfParser :: _isHeader(std::string& line) const
{return (line[0] == '[');}

bool ConfParser :: _isVecValue(std::string& line) const
{return (line[0] == '<');}

bool ConfParser :: _isComment(std::string& line) const
{return (line[0] == '#');}

bool ConfParser :: _isComma(std::string& line) const
{return (line[0] == ',');}


const ParserSection& ConfParser :: rootSection() const
{
	return  _rootSection;
}

const ParserSection * ConfParser :: getSection(const std::string& secname) const
{
	return _rootSection.getSection(secname);
}