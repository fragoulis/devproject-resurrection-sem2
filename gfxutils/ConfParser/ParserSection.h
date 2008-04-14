//*****************************************************************************
//**
//**   File:               ParserSection.h
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 14:34:51 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 126 $
//**
//*****************************************************************************

#pragma once

#include <string>
#include <vector>



/*
	ParserSection : 
	
	Class that holds a section's data, name & potential subsections
*/
class ParserSection
{
public:
	// Parser Section data - holds a [tag, value] pair
	class SectionData
	{
		public : 
			std::string _tag;					// data tag
			std::vector<std::string> _val;		// data value
	};

private:
	std::string     _name;					// Section name
	std::vector<SectionData> _data;			// Section data
	std::vector<ParserSection> _children;	// Subsections

	// from a [ parent : child ] string, returns the strings as a vector
	static std::vector<std::string> _getSectionHier(const std::string& section_block);

	public : 

	ParserSection(){}
	ParserSection(const std::string& name) :_name(name){}
	
	const std::string& getName() const;
	const ParserSection * getSection(const std::string& secname) const;		// returns section, a:b:c format
	ParserSection * getCreateSection(const std::string& secname);			// returns section, creates if needed
	const ParserSection * getChild(const std::string& secname) const;		// returns subsection (search depth 1)
	ParserSection * getCreateChild(const std::string& secname);				// returns subsection, creates if needed
	const std::vector<const ParserSection * > getChildren() const;
	const std::vector<SectionData>& getSectionData() const {return _data;}
	const std::string getVal(const std::string& tag) const;					// get first value
	const std::vector<std::string> getValVector(const std::string& tag) const;	// get all values

	bool valExists(const std::string& tag) const; // check if value exists

	void addVal(const std::string& tag,const std::string& val);			// adds a value
	void addChild(const std::string& name);								// adds a child

	void printStructure() const;										// prints the structure recursively
};

