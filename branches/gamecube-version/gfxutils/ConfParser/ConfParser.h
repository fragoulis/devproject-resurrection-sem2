#ifndef _RES_CONF_PARSER_H_
#define _RES_CONF_PARSER_H_
#include <string>
#include <iosfwd>
#include "../ConfParser/ParserSection.h"



/*
	ConfParser : 
	
	Class that parses a configuration file into an internal tree structure
*/
class ConfParser
{
	// Error enums
	enum parser_error_t
	{
		CONFPARSER_NO_ERROR = 0,
		CONFPARSER_MAIN_ERROR,
		CONFPARSER_HEADER_ERROR,
		CONFPARSER_VALUE_ERROR,
		CONFPARSER_TAG_ERROR,
		CONFPARSER_VECVALUE_ERROR,
		CONFPARSER_COMMA_ERROR,
	};

	// Parser status enums
	enum parser_status_t
	{
		CONFPARSER_MAIN,
		CONFPARSER_HEADER,
		CONFPARSER_TAG,
		CONFPARSER_VALUE,
		CONFPARSER_VECVALUE,
		CONFPARSER_COMMA,
	};
	
	// Parser token type enums
	enum parser_token_t
	{
		CONFPARSER_TOKEN_TAG,
		CONFPARSER_TOKEN_VALUE,
		CONFPARSER_TOKEN_HEADER,
	};

	// Token struct , [type, value pair] 
	struct ParserToken
	{
		parser_token_t type;
		std::string	   token;
		ParserToken(){}
		ParserToken(const parser_token_t& t,const std::string& tok){type = t;token = tok;}
		void clearBlanks();		// Clear blanks depending on type
	};

	ParserSection _rootSection;			// root section of config file
	bool _validConfFile;				// boolean flag ( although it would break on the hundred asserts)

	// Blank Clearing
	void _removeLeadBlanks(std::string& line) const;		// removes leading blanks from a line
	void _removeTrailBlanks(std::string& line) const;		// removes trailing blanks from a line

	// Parse functions
	// Changed by Joep: ifstream --> istream
	// Don't make parsers directly take ifstreams!
	void _parseFile(std::istream& ifs);					// main dunction, called on ctor
	parser_error_t _parseLine(parser_status_t& status,		// parses a single line for tokens
					std::string& line, 
					std::vector<ParserToken>& tokenlist) const;

	// Parse by type functions
	bool _parseHeader(std::string& line,std::vector<ParserToken>& tokenlist) const;		// parses a header string
	bool _parseTag(std::string& line,std::vector<ParserToken>& tokenlist) const;		// parses a tag string
	bool _parseValue(std::string& line,std::vector<ParserToken>& tokenlist) const;		// parses a single-value string
	bool _parseVecValue(std::string& line,std::vector<ParserToken>& tokenlist) const;	// parses a multiple-value string

	// isSth functions - boolean checkers
	bool _isHeader(std::string& line) const;
	bool _isVecValue(std::string& line) const;
	bool _isComment(std::string& line) const;
	bool _isComma(std::string& line) const;

public :
	// ctor
	ConfParser(const std::string& fname);

	// returns the root section of the configuration
	const ParserSection& rootSection() const;
	// returns the a section declared by a header string ( No spaces allowed yet between ':')
	const ParserSection * getSection(const std::string& secname) const;
};




#endif