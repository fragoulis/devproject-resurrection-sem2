#include "EnemyTypes.h"
#include "Objects/Enemyship.h"
#include "../gfxutils/ConfParser/ConfParser.h"
using namespace std;

EnemyTypes :: EnemyTypes()
{
}

EnemyTypes :: ~EnemyTypes()
{
}


EnemyType EnemyTypes :: getTypeFromName(const std::string& name) const
{
	//for (StringVector::const_iterator i = m_typeNames.begin(); i != m_typeNames.end(); ++i)
	for (int i = 0; i != m_typeNames.size(); ++i)
	{
		if (m_typeNames[i] == name) return i;
	}
	assert(0);
	return -1;
}

const string& EnemyTypes :: getNameFromType(EnemyType type) const
{
	return m_typeNames[type];
}

Enemyship* EnemyTypes :: createEnemyship(int type) const
{
	Enemyship* prototype = m_enemyPrototypes[type];
	assert(prototype != 0);
	Enemyship* newship = new Enemyship(*prototype);
	return newship;
}


template< class T >
std::ostream& operator << (std::ostream& os, const std::vector<T>& v)
{
	os << "[\n";
	for (std::vector<T>::const_iterator i = v.begin(); i != v.end(); ++i) {
		os << *i << ", \n";
	}
	os << "]";
	return os;
}


void EnemyTypes :: onApplicationLoad(const ParserSection& ps)
{
	const ParserSection* psFiles = ps.getSection("files");
	string filename = string("config/") + psFiles->getVal("EnemyTypesFile");

	ConfParser cp(filename);
	const ParserSection* psMain = cp.getSection("main");
	m_typeNames = psMain->getValVector("types");
	
	for (int i = 0; i != m_typeNames.size(); ++i)
	{
		const ParserSection* psType = cp.getSection(m_typeNames[i]);
		Enemyship* es = new Enemyship(i);
		es->loadSettings(*psType);
	}
}

void EnemyTypes :: onApplicationUnload()
{
}
