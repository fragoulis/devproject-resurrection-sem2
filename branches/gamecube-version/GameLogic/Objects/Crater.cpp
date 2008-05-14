#include "Crater.h"
#include "../../gfxutils/ConfParser/ParserSection.h"
#include "../../gfxutils/Misc/utils.h"
#include "../WorldObjectTypeManager.h"
#include "../Enemies/EnemyFactory.h"
#include "../Enemies/Enemyship.h"
#include "../Enemies/EnemyCarrier.h"
#include "../GameLogic.h"


Crater :: Crater()
{
	m_state = ALIVE;
	m_radius = 0.0f;
	m_ebombType = EBOMB_TYPE_UNKNOWN;
	m_protector = 0;
	m_timeTillNextEvent = 0.0f;

	EventManager::instance().registerEventListener<Enemy_Destroyed>(this);
}

Crater :: ~Crater()
{
	EventManager::instance().unRegisterEventListener<Enemy_Destroyed>(this);
}

void Crater :: update(float dt)
{
	m_timeTillNextEvent -= dt;
	if (m_timeTillNextEvent < 0.0f && !isProtected() && m_protectorType != 0)
	{
		const Point3& pos = getPosition();
		m_protector = GameLogic::instance().spawnEnemy(m_protectorType, m_protectorEnergyType, pos.getX(), pos.getZ());
		if (m_protectorCarrierEnergyTypeSpawns != ENERGY_TYPE_UNKNOWN)
		{
			EnemyCarrier* carrier = dynamic_cast<EnemyCarrier*>(m_protector);
			carrier->setEneryTypeSpawns(m_protectorCarrierEnergyTypeSpawns);
		}
	}
}

void Crater :: onEvent(Enemy_Destroyed& evt)
{
	if (evt.getValue() == m_protector) m_protector = 0;
	m_timeTillNextEvent = m_protectorRespawnDelay;
}
void Crater :: loadSettings(const ParserSection& ps)
{
	m_radius = FromString<float>(ps.getVal("Radius"));
	m_affectedAreaRadius = FromString<float>(ps.getVal("AffectedAreaRadius"));
	m_ebombType = EbombTypeFromString(ps.getVal("EbombType"));

	if (ps.valExists("ProtectorType"))
	{
		m_protectorRespawnDelay = FromString<float>(ps.getVal("ProtectorRespawnDelay"));
		m_protectorType = EnemyFactory::instance().getTypeFromName(ps.getVal("ProtectorType"));
		m_protectorEnergyType = EnergyTypeFromString(ps.getVal("ProtectorEnergyType"));

		if (EnemyFactory::instance().getEnemyClass(m_protectorType) == "Carrier")
		{
			m_protectorCarrierEnergyTypeSpawns = EnergyTypeFromString(ps.getVal("ProtectorCarrierEnergyTypeSpawns"));
		}
		else
		{
			m_protectorCarrierEnergyTypeSpawns = ENERGY_TYPE_UNKNOWN;
		}
	}
	else
	{
		m_protectorType = 0;
		m_protectorRespawnDelay = 0.0f;
	}
	


	WorldObject::loadSettings(ps);

	setType(WorldObjectTypeManager::instance().getTypeFromName("Crater"));
}

