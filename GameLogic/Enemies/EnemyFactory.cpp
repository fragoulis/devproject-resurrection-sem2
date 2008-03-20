//*****************************************************************************
//**
//**   File:               EnemyFactory.cpp
//**   Author:             $Author: Joep.Moritz $
//**   Last Change:        $Date: 2008-03-09 16:41:23 +0000 (Sun, 09 Mar 2008) $
//**   Last Revision:      $Revision: 128 $
//**
//*****************************************************************************


#include "EnemyFactory.h"
#include "Enemyship.h"
#include "../../gfxutils/ConfParser/ConfParser.h"
#include "../../utility/deleters.h"
#include "../WorldObjectTypeManager.h"
using namespace std;

EnemyFactory :: EnemyFactory()
{
}

EnemyFactory :: ~EnemyFactory()
{
	// TODO: remove this call when we fix unloading
	onApplicationUnload();
}


int EnemyFactory :: getTypeFromName(const std::string& name) const
{
	return WorldObjectTypeManager::instance().getTypeFromName(name);
}

const string& EnemyFactory :: getNameFromType(int type) const
{
	return WorldObjectTypeManager::instance().getNameFromType(type);
}

Enemyship* EnemyFactory :: createEnemyship(int type) const
{
	Enemyship* prototype = m_enemyPrototypes[type];
	assert(prototype != 0);
	Enemyship* newship = new Enemyship(*prototype);
	return newship;
}



void EnemyFactory :: onApplicationLoad(const ParserSection& ps)
{
	// get filename for enemy factory
	const ParserSection* psFiles = ps.getSection("EnemyFactory");
	string filename = string("config/") + psFiles->getVal("file");

	// open enemy factory file and get data
	ConfParser cp(filename);
	const ParserSection& psRoot = cp.rootSection();
	typedef std::vector<const ParserSection *> PSVector;
	PSVector psTypes = psRoot.getChildren();

	int typeCount = WorldObjectTypeManager::instance().getTypeCount();
	m_enemyPrototypes.resize(typeCount);

	// loop over all sections
	// each section is an enemy type
	// and the name should correspond to a world object type
	for (PSVector::iterator it = psTypes.begin(); it != psTypes.end(); it++)
	{
		const ParserSection* ps = *it;
		int type = WorldObjectTypeManager::instance().getTypeFromName(ps->getName());
		Enemyship* es = new Enemyship(type);
		es->loadSettings(*ps);
		m_enemyPrototypes[type] = es;
	}
}

void EnemyFactory :: onApplicationUnload()
{
	deleteVector(m_enemyPrototypes);
}
