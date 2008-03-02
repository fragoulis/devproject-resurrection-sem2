#pragma once
#include "../utility/Singleton.h"
#include <string>
#include <vector>
class Enemyship;
class ParserSection;



/**
 * Keeps a list of enemy types.
 * Can translate a type to a name and vice versa.
 * Can spawn an enemyship of a specified type.
 *
 */
class EnemyTypes : public Singleton< EnemyTypes >
{
public:

	int getTypeFromName(const std::string& name) const;
	const std::string& getNameFromType(int type) const;

	Enemyship* createEnemyship(int type) const;


	void onApplicationLoad(const ParserSection&);
	void onApplicationUnload();


private:
	typedef std::vector<std::string> StringVector;
	typedef std::vector<Enemyship*> EnemyVector;

	StringVector m_typeNames;
	EnemyVector m_enemyPrototypes;


	friend Singleton< EnemyTypes >;
	EnemyTypes();
	virtual ~EnemyTypes();
};
