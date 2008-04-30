#pragma once
#include "../../utility/Singleton.h"
#include <string>
#include <vector>
class Enemyship;
class ParserSection;



/**
 * Keeps a list of enemy types (uses prototypes).
 * Can translate a type to a name and vice versa.
 * Can spawn an enemyship of a specified type.
 *
 * On Level_Load or app::load you read an enm type (a string)
 * then ask the factory "ok what int is that?"
 * and you use that int to store data in a vector of I dunno
 * models
 * sounds
 * whatever
 * and when you get Enemy_Destroyed
 * you do playsound(m_sounds[enm->getType()])
 *
 * Or if you want to do alternative functionality,
 * you can store a list of method pointers in a vector.
 */
class EnemyFactory : public Singleton< EnemyFactory >
{
public:

	int getTypeFromName(const std::string& name) const;
	const std::string& getNameFromType(int type) const;

	const std::string& getEnemyClass(int type) const { return m_enemyClasses[type]; }
	bool isEnemyClass(int type, const std::string& c) const { return m_enemyClasses[type] == c; }

	Enemyship* createEnemyship(int type) const;


	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();

private:
	typedef std::vector<Enemyship*> EnemyVector;
	typedef std::vector<std::string> EnemyClasses;

	EnemyVector m_enemyPrototypes;
	EnemyClasses m_enemyClasses;


	friend Singleton< EnemyFactory >;
	EnemyFactory();
	virtual ~EnemyFactory();
};
