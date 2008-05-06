#include <ConfParser/ParserSection.h>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <Misc/Logger.h>

using namespace CKLib;
using namespace std;

// ParserSection

const ParserSection * ParserSection :: getSection(const std::string& secname) const
{
	vector<string> sechier = _getSectionHier(secname);
	vector<string> :: iterator sechier_it = sechier.begin();
	const ParserSection * parsec = this;
	while(sechier_it != sechier.end())
	{
		parsec = parsec->getChild(*sechier_it);
		if(parsec == NULL)
			break;
		sechier_it++;
	}
	return parsec;
}

ParserSection * ParserSection :: getCreateSection(const std::string& secname)
{
	vector<string> sechier = _getSectionHier(secname);
	vector<string> :: iterator sechier_it = sechier.begin();
	ParserSection * parsec = this;
	while(sechier_it != sechier.end())
	{
		parsec = parsec->getCreateChild(*sechier_it);		
		sechier_it++;
	}
	return parsec;
}

const ParserSection * ParserSection :: getChild(const std::string& secname) const
{
	vector<ParserSection>::const_iterator child_it = _children.begin();
	while(child_it != _children.end())
	{
		if(child_it->getName() == secname)
			return &(*child_it);
		++child_it;
	}
	return NULL;
}

ParserSection * ParserSection :: getCreateChild(const std::string& secname)
{
	vector<ParserSection>::iterator child_it = _children.begin();
	while(child_it != _children.end())
	{
		if(child_it->getName() == secname)
			return &(*child_it);
		++child_it;
	}
	_children.push_back(ParserSection(secname));
	return &(_children[_children.size() - 1]);
}

const std::string& ParserSection :: getName() const
{
	return _name;
}

void ParserSection :: addVal(const std::string& tag,const std::string& val)
{
	for(size_t i=0;i<_data.size();++i)
	{
		if(tag == _data[i]._tag)
		{
			_data[i]._val.push_back(val);
			return;
		}
	}
	_data.push_back(SectionData());
	_data[_data.size()-1]._tag = tag;
	_data[_data.size()-1]._val.push_back(val);
}

void ParserSection :: addChild(const std::string& name)
{
	_children.push_back(ParserSection(name));
}

const std::string ParserSection :: getVal(const std::string& tag) const
{
	// For each tag/val
	for(unsigned i=0;i<_data.size();++i)
		if(_data[i]._tag == tag)
		{
			// assert there is only 1 value & return
			assert(_data[i]._val.size() == 1);
			return _data[i]._val[0];
		}
	return string();
}

const std::vector<std::string> ParserSection :: getValVector(const std::string& tag) const
{
	// For each tag/val
	for(unsigned i=0;i<_data.size();++i)
		if(_data[i]._tag == tag)
			return _data[i]._val;
	return vector<string>();
}

std::vector<std::string> ParserSection :: _getSectionHier(const std::string& section_block)
{
	vector<string> sectionhier;
	size_t start_pos = 0,i=0;
	while(i < section_block.size())
	{
		if(section_block[i] == ':')
		{
			sectionhier.push_back(section_block.substr(start_pos,i - start_pos));
			start_pos = i + 1;
		}
		++i;
	}
	sectionhier.push_back(section_block.substr(start_pos,i - start_pos));
	return sectionhier;
}

const std::vector<const ParserSection * > ParserSection :: getChildren() const
{
	vector<const ParserSection * > ch;
	for(size_t i=0;i<_children.size();++i)
		ch.push_back(&(_children[i]));
	return ch;
}

void ParserSection :: printStructure() const
{
	cout<<"Section Name : "<<getName()<<endl;
	vector<SectionData>::const_iterator data_it = _data.begin();
	while(data_it != _data.end())
	{
		cout<<data_it->_tag<<" = "<<data_it->_val[0]<<endl;
		for(size_t i=1;i<data_it->_val.size();++i)
			cout<<"\t\t"<<data_it->_val[i]<<endl;
		++data_it;
	}
	vector<ParserSection>::const_iterator child_it = _children.begin();
	cout<<"########## "<<getName()<<" children ############"<<endl;
	while(child_it != _children.end())
	{
		child_it->printStructure();
		++child_it;
	}
	cout<<"########## end of "<<getName()<<" children ############"<<endl;
}