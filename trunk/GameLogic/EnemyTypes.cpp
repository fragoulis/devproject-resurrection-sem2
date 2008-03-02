#include "EnemyTypes.h"
#include "Objects/Enemyship.h"
#include "../gfxutils/ConfParser/ConfParser.h"
using namespace std;

EnemyTypes :: EnemyTypes()
{
}

EnemyTypes :: ~EnemyTypes()
{
	// TODO: remove this call when we fix unloading
	onApplicationUnload();
}


int EnemyTypes :: getTypeFromName(const std::string& name) const
{
	//for (StringVector::const_iterator i = m_typeNames.begin(); i != m_typeNames.end(); ++i)
	for (int i = 0; i != m_typeNames.size(); ++i)
	{
		if (m_typeNames[i] == name) return i;
	}
	assert(0);
	return -1;
}

const string& EnemyTypes :: getNameFromType(int type) const
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



void EnemyTypes :: onApplicationLoad(const ParserSection& ps)
{
	const ParserSection* psFiles = ps.getSection("files");
	string filename = string("config/") + psFiles->getVal("EnemyTypesFile");

	ConfParser cp(filename);
	const ParserSection* psMain = cp.getSection("main");
	m_typeNames = psMain->getValVector("types");

	m_enemyPrototypes.resize(m_typeNames.size());
	
	for (int i = 0; i != m_typeNames.size(); ++i)
	{
		const ParserSection* psType = cp.getSection(m_typeNames[i]);
		Enemyship* es = new Enemyship(i);
		es->loadSettings(*psType);
		m_enemyPrototypes[i] = es;
	}
}

void EnemyTypes :: onApplicationUnload()
{
	for (int i = 0; i != m_enemyPrototypes.size(); i++)
	{
		delete m_enemyPrototypes[i];
	}
}
